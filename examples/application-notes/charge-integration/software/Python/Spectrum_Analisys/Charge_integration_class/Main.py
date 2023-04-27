import numpy as np
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore
from PyQt5.QtWidgets import QVBoxLayout, QHBoxLayout, QPushButton, QWidget
from scisdk.scisdk import SciSDK
import scipy.optimize as opt
from pyqtgraph.parametertree import Parameter, ParameterTree
from pyqtgraph.parametertree import parameterTypes as pTypes

import Region_class as rc
import Oscilloscope_parameter_class as opc

# Initialize sdk library
sdk = SciSDK()

# Add the DT1260 device to the sdk
res = sdk.AddNewDevice("usb:13250", "DT1260", "RegisterFile.json", "board0")

if res != 0:
    print("Error adding device")
    exit()

# Set Parameters
registers = { 
    "polarity": 1, 
    "trigger_threshold": 2200,
    "number_of_samples": 7,
    "time_hold_baseline": 400,
    "time_integration": 250,
    "pre_integration": 5,
    "gain": 3000,
    "offset_integration": 0
    }

decimator =1

# Set register values
res = sdk.SetRegister("board0:/Registers/pol", registers["polarity"])
res = sdk.SetRegister("board0:/Registers/trgthrs", registers["trigger_threshold"])
res = sdk.SetRegister("board0:/Registers/Nsample", registers["number_of_samples"])
res = sdk.SetRegister("board0:/Registers/time_hold_bs", registers["time_hold_baseline"])
res = sdk.SetRegister("board0:/Registers/time_integr", registers["time_integration"])
res = sdk.SetRegister("board0:/Registers/pre_integr", registers["pre_integration"])
res = sdk.SetRegister("board0:/Registers/gain", registers["gain"])
res = sdk.SetRegister("board0:/Registers/Offset_int", registers["offset_integration"])

### SPECTRUM

# Set spectrum parameters
sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.rebin", "0")
sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.limitmode", "freerun")

# Execute command reset
sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "")

# Execute command start
sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")

# Allocate buffer
res, obSpectrum = sdk.AllocateBuffer("board0:/MMCComponents/Spectrum_0")

if res != 0:
    res, err = sdk.s_error(res)
    print("Allocate spectrum buffer error:", err)
    exit(-1)

### OSCILLOSCOPE

# set oscilloscope parameters
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing","decoded")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", 2000)
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode","ext")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 2000)
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150)
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", decimator)
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.timeout", 3000)

res, bufOscilloscope = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0")

if res != 0:
    res, err = sdk.s_error(res)
    print("Allocate oscilloscope buffer error:", err)
    exit(-1)

########################################################################################################################
####### GUI ############################################################################################################

app = pg.mkQApp("Spectrum_PyQtgraph")

# Create the spectrum plot
pw2 = pg.PlotWidget(title = "Spectrum", labels={'left': 'Counts', 'bottom': 'Channels'})
p1 = pw2.plot()

# Create the parameter Region
params_line = rc.Region('Lines', 2, pw2)

params_line.addChild(dict(name='Add a Line', type='action'))
params_line.param("Add a Line").sigActivated.connect(lambda: params_line.addLine(pw2))

# Create the parameter Peaks
params_peak = rc.Peaks(params_line)

### Buttons
# Create the oscilloscope button: to open the oscilloscope window
oscilloscope_btn = QPushButton('Oscilloscope')

# Create the acquisition buttons
start_btn = QPushButton('Start')
stop_btn = QPushButton('Stop')
reset_btn = QPushButton('Reset')

# Create the layout to hold the acquisition buttons
button_layout = QHBoxLayout()
button_layout.addWidget(start_btn)
button_layout.addWidget(stop_btn)
button_layout.addWidget(reset_btn)

# Create the layout to hold the plots
plot_layout = QVBoxLayout()
plot_layout.addWidget(pw2)
plot_layout.addLayout(button_layout)

# Create total tree
children = [
    params_line,
    params_peak,
]
my_parameter = Parameter.create(name='Parameters', type='group', children=children)
pt = ParameterTree()
pt.setParameters(my_parameter)
pt.setFixedWidth(450)

# Create the parameters layout
v_layout = QVBoxLayout()
v_layout.addWidget(pt)
v_layout.addWidget(oscilloscope_btn)

# Create horizontal layout to add parameter tree and vertical layout
h_layout = QHBoxLayout()
h_layout.addLayout(v_layout)
h_layout.addLayout(plot_layout)

# Create QWidget to hold the horizontal layout
widget = QWidget()
widget.setLayout(h_layout)

# Show widget
widget.show()

###############################################################################
###### PLOT SPECTRUM ##########################################################

# Set the function for the fit
def gaussian(x, a, x0, sigma):
    return a * np.exp(-(x-x0)**2/(2*sigma**2))

# Update the spectrum plot
def update_plot():
    global obSpectrum
    res, obSpectrum = sdk.ReadData("board0:/MMCComponents/Spectrum_0",obSpectrum)
    if res == 0:
        xar = []
        yar = []
        for index in range(obSpectrum.info.valid_bins):
            xar.append(index)
            yar.append(obSpectrum.data[index])
        p1.setData(x=xar, y=yar)
    
    # Make the fit
    for line in params_line.lines:
        if line.value():
            if line.getPlot() is None:
                line.setPlot(pw2.plot())
            roi = line.getInterval().getRegion()
            center = (roi[0] + roi[1])/2
            if center != 0.0:
                min_bin = int(roi[0])
                max_bin = int(roi[1])
                x_fit = xar[min_bin:max_bin]
                y_fit = yar[min_bin:max_bin]

                # Extract the fitted parameters
                popt, pcov = opt.curve_fit(gaussian, x_fit, y_fit, p0=[max(y_fit), center, 10])
                a_fit, x0_fit, sigma_fit = popt
                resolution = sigma_fit/x0_fit*100
                line.setAnalysis(x0_fit, sigma_fit, resolution)

                # Plot the fitted curve
                ygauss = gaussian(x_fit, *popt)
                line.plot.setData(x=x_fit, y=ygauss, pen=None, symbol='o', symbolPen=None, symbolSize=2, symbolBrush=('r'))

                # Display the fitted parameters
                if params_peak.a != 0 and params_peak.b != 0:
                    x0_fit_keV = x0_fit * params_peak.a + params_peak.b
                    sigma_fit_keV = sigma_fit * params_peak.a
                    resolution_keV = sigma_fit_keV/x0_fit_keV*100

                    line.getAnalysis().setCentroid_keV(round(x0_fit_keV,3))
                    line.getAnalysis().setSigma_keV(round(sigma_fit_keV,5))
                    line.getAnalysis().setResolution_keV(round(resolution_keV,5))
        else:
            if line.getPlot() is not None:
                line.getPlot().clear()
                line.setPlot(None)


# Create a timer object and set its interval
timer = QtCore.QTimer()
timer.timeout.connect(update_plot)
timer.start(50) # Emit the timeout() signal every 50 ms

###############################################################################
###### START, STOP & RESET BUTTON (SPECTRUM) ##################################
def stop():
    timer.stop()
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "")

def start():
    timer.start()
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")

def reset():
    timer.stop()
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "")
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "")

    timer.start()
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")

stop_btn.clicked.connect(stop)
start_btn.clicked.connect(start)
reset_btn.clicked.connect(reset)



###############################################################################
###### OSCILLOSCOPE ###########################################################

# Generate the oscilloscope window
new_win = pg.GraphicsLayoutWidget(title="Oscilloscope", show=False)
new_win.resize(1000,600)

# Create the oscilloscope plot + channels plot
pw3 = pg.PlotWidget(title = "Oscilloscope")
o1 = pw3.plot()
o2 = pw3.plot()
o3 = pw3.plot()
o4 = pw3.plot()
o2d = pw3.plot()
o3d = pw3.plot()
o4d = pw3.plot()

legend = pg.LegendItem((100,60), offset=(500, 20))
legend.setParentItem(pw3.graphicsItem())

# Create a timer object and set its interval for the oscilloscope
timer2 = QtCore.QTimer()
timer2.timeout.connect(lambda: update_oscilloscope())
timer2.start(500) # Emit the timeout() signal every 500 ms

# Create the channels and registers parameters
params_channel = opc.Channels_class(legend=legend, o1=o1, o2=o2, o3=o3, o4=o4, o2d=o2d, o3d=o3d, o4d=o4d)
params_reg = opc.Registers_class(registers, sdk)

children_osc = [
    params_reg,
    params_channel,
]

# When the oscilloscope button is clicked, show the oscilloscope window
def show_new_window():
    my_parameter1 = Parameter.create(name='Parameters', type='group', children=children_osc)
    pt1 = ParameterTree() 
    pt1.setParameters(my_parameter1)
    pt1.setFixedWidth(300)

    new_win_layout = QHBoxLayout()
    new_win_layout.addWidget(pt1)
    new_win_layout.addWidget(pw3)
    new_win.setLayout(new_win_layout)

    new_win.show()

oscilloscope_btn.clicked.connect(show_new_window)

# Update the oscilloscope plot
def update_oscilloscope():
    global bufOscilloscope
    res, bufOscilloscope= sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", bufOscilloscope)

    if res == 0:
        offset_channel_dig = bufOscilloscope.info.samples_digital * bufOscilloscope.info.tracks_digital_per_channel
        offset_channel_an = bufOscilloscope.info.samples_analog

        # set x-axis
        x = np.arange(0, offset_channel_an, decimator)

        # set y for the analog channels and plot them if selected
        y0 = bufOscilloscope.analog[0:offset_channel_an] # Signal
        y1 = bufOscilloscope.analog[offset_channel_an: offset_channel_an*2] # Baseline
        y2 = bufOscilloscope.analog[offset_channel_an*2: offset_channel_an*3] # Triggered signal
        y3 = bufOscilloscope.analog[offset_channel_an*3: offset_channel_an*4] # Energy

        if params_channel.get_channel_value('Signal') == True:
            o1.setData(x=x, y=y0, pen = pg.mkPen(color='w', width=1))
        else:
            o1.setData(x=[], y=[])
        
        if params_channel.get_channel_value('Baseline') == True:
            o2.setData(x=x, y=y1, pen = pg.mkPen(color='g', width=1))
        else:
            o2.setData(x=[], y=[])
        
        if params_channel.get_channel_value('Triggered signal') == True:
            o3.setData(x=x, y=y2, pen = pg.mkPen(color='r', width=1))
        else:
            o3.setData(x=[], y=[])
        
        if params_channel.get_channel_value('Energy') == True:
            o4.setData(x=x, y=y3, pen = pg.mkPen(color='y', width=1))
        else:
            o4.setData(x=[], y=[])

        # set y for the digital channels and plot them if selected
        medium_bl = np.mean(y1) # medium baseline

        # Baseline run/hold
        y12 = bufOscilloscope.digital[offset_channel_dig * 1 + offset_channel_an: offset_channel_dig*1 + offset_channel_an*2]       
        yy_run_bl = [] 
        xx_run_bl = []
        for u in range(len(y12)):
            if y12[u] == 0:
                xx_run_bl.append(u)
                yy_run_bl.append(2000)  # 2000 is choose as arbitrary value, just to make the plot visible
            else:
                xx_run_bl.append(u)
                yy_run_bl.append(medium_bl) # when 0 is set like medium baseline
        if params_channel.get_channel_value('Baseline hold interval') == True:
            o2d.setData(x=xx_run_bl, y=yy_run_bl, pen=pg.mkPen(color='g', width=2))
        else:
            o2d.setData(x=[], y=[])

         # Trigger
        y21 = bufOscilloscope.digital[offset_channel_dig * 2: offset_channel_dig*2 + offset_channel_an*1]
        yy_trig = []
        xx_trig = []
        for u in range(len(y21)):
            if y21[u] == 1:
                xx_trig.append(u)
                yy_trig.append(y0[u])   # the trigger is high like signal
            else:
                xx_trig.append(u)
                yy_trig.append(medium_bl)   # when 0 is set like medium baseline
        if params_channel.get_channel_value("Trigger") == True:
            o3d.setData(x=xx_trig, y=yy_trig, pen = pg.mkPen(color='r', width=2))
        else:
            o3d.setData(x=[], y=[])
        
        # Gate integration
        y31 = bufOscilloscope.digital[offset_channel_dig * 3 + offset_channel_an: offset_channel_dig*3 + offset_channel_an*2] 
        yy_gate = []
        xx_gate = []
        for u in range(len(y31)):
            if y31[u] == 1:
                xx_gate.append(u)
                yy_gate.append(2010)    # 2010 is choose as arbitrary value, just to make the plot visible
            else:
                xx_gate.append(u)
                yy_gate.append(medium_bl)  # when 0 is set like medium baseline
        if params_channel.get_channel_value("Gate integration") == True:
            o4d.setData(x=xx_gate, y=yy_gate, pen= pg.mkPen(color='y', width=2))
        else:
            o4d.setData(x=[], y=[])



if __name__ == '__main__':
    pg.exec()