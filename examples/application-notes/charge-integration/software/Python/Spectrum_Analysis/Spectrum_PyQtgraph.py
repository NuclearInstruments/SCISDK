import numpy as np
import pyqtgraph as pg
from pyqtgraph.Qt import QtGui, QtCore, QtWidgets
from PyQt5.QtWidgets import QApplication, QVBoxLayout, QHBoxLayout, QPushButton, QLineEdit, QLabel, QWidget, QTextEdit, QTreeWidgetItem, QTreeWidget
from scisdk.scisdk import SciSDK
import math
import scipy.optimize as opt
import ctypes
from pyqtgraph.parametertree import Parameter, ParameterTree, parameterTypes


centroid = 0.0
sigma_fit = 0.0
resolution = 0.0
energy_peak = 0.0
x0_fit = 0.0
centroid2 = 0.0
sigma_fit2 = 0.0
resolution2 = 0.0
energy_peak2 = 0.0
x0_fit2 = 0.0

a = 0.0
b = 0.0

decimator = 1
Signal = False
Baseline = False
Baseline_run = False
Triggered_signal = False
Trigger = False
Energy = False
Gate_integration = False

line1 = False
line1_region = None
line2 = False
line2_region = None

# Initialize sdk library
sdk = SciSDK()

# Add the DT1260 device to the sdk
res = sdk.AddNewDevice("usb:28686", "DT1260", "RegisterFile.json", "board0")

if res != 0:
    print("Error adding device")
    exit()

# Set Parameters
polarity = 1
trigger_threshold = 2200
number_of_samples = 7
time_hold_baseline = 400
time_integration = 250
pre_integration = 5
gain = 3000
offset_integration = 0

# Set register values
res = sdk.SetRegister("board0:/Registers/pol", polarity)
res = sdk.SetRegister("board0:/Registers/trgthrs", trigger_threshold)
res = sdk.SetRegister("board0:/Registers/Nsample", number_of_samples)
res = sdk.SetRegister("board0:/Registers/time_hold_bs", time_hold_baseline)
res = sdk.SetRegister("board0:/Registers/time_integr", time_integration)
res = sdk.SetRegister("board0:/Registers/pre_integr", pre_integration)
res = sdk.SetRegister("board0:/Registers/gain", gain)
res = sdk.SetRegister("board0:/Registers/Offset_int", offset_integration)

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

# Definition of the gaussian function for the fit
def gaussian(x, a, x0, sigma):
    return a * np.exp(-(x-x0)**2/(2*sigma**2))

########################################################################################################################
####### GUI ############################################################################################################

app = pg.mkQApp("Spectrum_PyQtgraph")

# Create parameters tree
par = [
    dict(name='Polarity', type='list', limits=['Positive', 'Negative'], value='Positive'),
    dict(name='Trigger threshold', type='int', value=trigger_threshold),
    dict(name='Number of sample', type='int', value=number_of_samples),
    dict(name='Time hold baseline', type='int', value=time_hold_baseline),
    dict(name='Time integration', type='int', value=time_integration),
    dict(name='Pre-integration', type='int', value=pre_integration),
    dict(name='Gain', type='int', value=gain, limits=[10, 65500]),
    dict(name='Offset spectrum', type='int', value=offset_integration),
    dict(name='Save State', type= 'action'),
]
params = Parameter.create(name='Parameters', type='group', children=par)

# Create channels tree
channel = [
    dict(name='Signal', type='bool', value=Signal),
    dict(name='Baseline', type='bool', value=Baseline),
    dict(name='Baseline hold interval', type='bool', value=Baseline_run),
    dict(name='Triggered signal', type='bool', value=Triggered_signal),
    dict(name='Trigger', type='bool', value=Trigger),
    dict(name='Energy', type='bool', value=Energy),
    dict(name='Gate integration', type='bool', value=Gate_integration),   
]
params_channel = Parameter.create(name='Oscilloscope Channels', type='group', children=channel)

# Create lines tree
fin_an1 = [
    dict(name='Centroid', type='str', value=centroid, readonly=True),
    dict(name='Sigma', type='str', value=sigma_fit, readonly=True),
    dict(name='Resolution', type='str', value=resolution, readonly=True),
]

fin_an2 = [
    dict(name='Centroid', type='str', value=centroid2, readonly=True),
    dict(name='Sigma', type='str', value=sigma_fit2, readonly=True),
    dict(name='Resolution', type='str', value=resolution2, readonly=True),   
]

line = [
    dict(name='Line 1', type='bool', value=line1),
    dict(name='Line 2', type='bool', value=line2),
]

params_line = Parameter.create(name='Lines', type='group', children=line)

line1_param = params_line.child('Line 1')
line1_an_param = Parameter.create(name='Parameters', type='group', children=fin_an1, expanded=False)
line1_param.addChild(line1_an_param)
line2_param = params_line.child('Line 2')
line2_an_param = Parameter.create(name='Parameters', type='group', children=fin_an2, expanded=False)
line2_param.addChild(line2_an_param)

#Create calibration tree
peaks = [
    dict(name='Peak 1', type='str', value=energy_peak, suffix= 'keV', siPrefix=True),
    dict(name='Peak 2', type='str', value=energy_peak2, suffix='keV', siPrefix=True),
    dict(name='Calibrate', type= 'action'),
]
param_peak = Parameter.create(name='Peaks (keV)', type='group', children=peaks)

children = [
    params,
    params_channel,
    params_line,
    param_peak,
]

# Create total tree
my_parameter = Parameter.create(name='Parameters', type='group', children=children)
pt = ParameterTree() #showHeader=True 
pt.setParameters(my_parameter)
pt.setFixedWidth(450)

##################################################################################################################

# Create start, stop, and reset buttons
start_btn = QPushButton('Start')
stop_btn = QPushButton('Stop')
reset_btn = QPushButton('Reset')

##################################################################################################################
# Create plot widget
pw2 = pg.PlotWidget(title = "Spectrum", labels={'left': 'Counts', 'bottom': 'Channels'})
pw3 = pg.PlotWidget(title = "Oscilloscope")

# Define function in the spectrum plot
p1 = pw2.plot()
p2 = pw2.plot()
p3 = pw2.plot()

# Define function in the oscilloscope plot
o1 = pw3.plot()
o2 = pw3.plot()
o3 = pw3.plot()
o4 = pw3.plot()
o2d = pw3.plot()
o3d = pw3.plot()
o4d = pw3.plot()

legend = pg.LegendItem((100,60), offset=(400,30))
legend.setParentItem(pw3.graphicsItem())
##########################################################################################################################

# Create the layout to hold the acquisition buttons
button_layout = QHBoxLayout()
button_layout.addWidget(start_btn)
button_layout.addWidget(stop_btn)
button_layout.addWidget(reset_btn)

# Create the layout to hold the plots
plot_layout = QVBoxLayout()
plot_layout.addLayout(button_layout)
plot_layout.addWidget(pw2)
plot_layout.addWidget(pw3)


# Create horizontal layout to add parameter tree and vertical layout
h_layout = QHBoxLayout()
h_layout.addWidget(pt)
h_layout.addLayout(plot_layout)

# Create QWidget to hold the horizontal layout
widget = QWidget()
widget.setLayout(h_layout)

# Show widget
widget.show()


def update_line(param, line_an_par, line_num):
    global line1, line2, line2_region, line1_region
    value = param.value()
    if line_num == 1:
        line1 = value
        line_an_par.setOpts(expanded=True)
        if value:
            line1_region = pg.LinearRegionItem([7500, 8500], bounds=[0,16384], movable=True)
            pw2.addItem(line1_region)
            label1 = pg.InfLineLabel(line1_region.lines[1], "Line 1", position=0.95, rotateAxis=(1,0), anchor=(1, 1))
            #line1_region.sigRegionChangeFinished.connect(lambda: region_changed(1))
        else:
            pw2.removeItem(line1_region)
            line1_region = None

    if line_num == 2:
        line2 = value
        line_an_par.setOpts(expanded=True)
        if value:
            line2_region = pg.LinearRegionItem([2500, 3500], bounds=[0,16384], movable=True) # (initial values, limits, is movable)
            pw2.addItem(line2_region)
            label2 = pg.InfLineLabel(line2_region.lines[1], "Line 2", position=0.95, rotateAxis=(1,0), anchor=(1, 1))
        else:
            pw2.removeItem(line2_region)
            line2_region = None    


# Connect parameter change signal to update_channel function

line1_param.sigValueChanged.connect(lambda param: update_line(param, line1_an_param,1))
line2_param.sigValueChanged.connect(lambda param: update_line(param, line2_an_param,2))


####################################################################################################################
############# UPDATE SPECTRUM ######################################################################################

def update_plot():
    global obSpectrum, energy_peak, energy_peak2, x0_fit, x0_fit2, a, b
    # Read data from the spectrum analyzer
    res, obSpectrum = sdk.ReadData("board0:/MMCComponents/Spectrum_0",obSpectrum)
    if res == 0:
        xar = []
        yar = []
        for index in range(obSpectrum.info.valid_bins):
            xar.append(index)
            yar.append(obSpectrum.data[index])
        p1.setData(x=xar, y=yar)
        
        if line1:
            roi = line1_region.getRegion() # get the region of interest       
            center = (roi[1] + roi[0]) / 2

            if center != 0.0:
                min_bin = int( roi[0] )
                max_bin = int( roi[1] )
                x_fit = xar[min_bin:max_bin]
                y_fit = yar[min_bin:max_bin]

                # Extract the fitted parameters
                popt, pcov = opt.curve_fit(gaussian, x_fit, y_fit, p0=[max(y_fit), center, 10])
                a_fit, x0_fit, sigma_fit = popt
                resolution = sigma_fit/x0_fit*100

                #Plot the fitted curve
                ygaus = gaussian(x_fit, *popt)
                p2.setData(x=x_fit, y=ygaus, pen=None, symbol='o', symbolPen=None, symbolSize=2, symbolBrush=('r'))                

                # update the centroid parameter value
                line1_an_param.param('Centroid').setOpts(value='{:,.3f}'.format(round(x0_fit,5)))
                line1_an_param.param('Sigma').setOpts(value='{:,.3f}'.format(round(sigma_fit,5)))
                line1_an_param.param('Resolution').setOpts(value='{:,.3f}'.format(resolution))


        if line2:
            roi2 = line2_region.getRegion() # get the region of interest
            center2 = (roi2[1] + roi2[0]) / 2
            if center2 != 0.0:
                min_bin2 = int( roi2[0] )
                max_bin2 = int( roi2[1] )
                x_fit2 = xar[min_bin2:max_bin2]
                y_fit2 = yar[min_bin2:max_bin2]

                # Extract the fitted parameters
                popt2, pcov2 = opt.curve_fit(gaussian, x_fit2, y_fit2, p0=[max(y_fit2), center2, 10])
                a_fit2, x0_fit2, sigma_fit2 = popt2
                resolution2 = sigma_fit2/x0_fit2*100

                #Plot the fitted curve
                ygaus2 = gaussian(x_fit2, *popt2)
                p3.setData(x=x_fit2, y=ygaus2, pen=None, symbol='o', symbolPen=None, symbolSize=2, symbolBrush=('r'))

                # update the centroid parameter value
                line2_an_param.param('Centroid').setOpts(value='{:,.3f}'.format(round(x0_fit2,5)))
                line2_an_param.param('Sigma').setOpts(value='{:,.3f}'.format(round(sigma_fit2,5)))
                line2_an_param.param('Resolution').setOpts(value='{:,.3f}'.format(resolution2))
        
        
        if a != 0 and b != 0:
            x0_fit_keV = x0_fit * a + b
            x0_fit2_keV = x0_fit2 * a + b
            sigma_keV = sigma_fit * a
            sigma2_keV = sigma_fit2 * a
            resolution_keV = sigma_keV/x0_fit_keV*100
            resolution2_keV = sigma2_keV/x0_fit2_keV*100

            # update the centroid parameter value with keV conversion
            line1_an_param.param('Centroid').setOpts(value='{:,.3f} ({:,.3f} keV)'.format(round(x0_fit,5), round(x0_fit_keV,5)))
            line2_an_param.param('Centroid').setOpts(value='{:,.3f} ({:,.3f} keV)'.format(round(x0_fit2,5), round(x0_fit2_keV,5)))
            line1_an_param.param('Sigma').setOpts(value='{:,.3f} ({:,.3f} keV)'.format(round(sigma_fit,5), round(sigma_keV,5)))
            line2_an_param.param('Sigma').setOpts(value='{:,.3f} ({:,.3f} keV)'.format(round(sigma_fit2,5), round(sigma2_keV,5)))
            line1_an_param.param('Resolution').setOpts(value='{:,.3f} ({:,.3f} keV)'.format(resolution, resolution_keV))
            line2_an_param.param('Resolution').setOpts(value='{:,.3f} ({:,.3f} keV)'.format(resolution2, resolution2_keV))
        else:
            # update the centroid parameter value without keV conversion
            line1_an_param.param('Centroid').setOpts(value='{:,.3f}'.format(round(x0_fit,5)))
            line2_an_param.param('Centroid').setOpts(value='{:,.3f}'.format(round(x0_fit2,5)))

            

peak_1_par = param_peak.child('Peak 1')
peak_2_par = param_peak.child('Peak 2')

def calibrate():
    global energy_peak, energy_peak2, x0_fit, x0_fit2, a, b
    energy_peak = peak_1_par.value()
    energy_peak2 = peak_2_par.value()

    param_peak.param("Peak 1").sigValueChanged.connect(lambda param: update_energy_peak(param, 0))
    param_peak.param("Peak 2").sigValueChanged.connect(lambda param: update_energy_peak(param, 1))

    if energy_peak != 0 and energy_peak2 != 0:
        energy1 = float(energy_peak)
        energy2 = float(energy_peak2)
        peak1 = float(x0_fit)
        peak2 = float(x0_fit2)

        a = (energy2 - energy1)/(peak2 - peak1)
        b = (energy1*peak2 - energy2*peak1)/(peak2 - peak1)

param_peak.param("Calibrate").sigActivated.connect(calibrate)

def update_energy_peak(param, energy_num):
    global energy_peak, energy_peak2
    value = param.value()
    if energy_num == 0:
        energy_peak = value
    elif energy_num == 1:
        energy_peak2 = value
    else:
        pass


         
# Create a timer object and set its interval
timer = QtCore.QTimer()
timer.timeout.connect(update_plot)
timer.start(50) # Emit the timeout() signal every 50 ms

####################################################################################################################
############# UPDATE OSCILLOSCOPE ##################################################################################


def update_oscilloscope():
    global bufOscilloscope
    res, bufOscilloscope= sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", bufOscilloscope)

    if res == 0:
        offset_channel_dig = bufOscilloscope.info.samples_digital * bufOscilloscope.info.tracks_digital_per_channel
        offset_channel_an = bufOscilloscope.info.samples_analog

        x = np.arange(0, offset_channel_an, decimator)

        # slice bufOscilloscope.analog from 0 to bufOscilloscope.info.samples_analog
        y0 = bufOscilloscope.analog[0:offset_channel_an] # Signal
        y1 = bufOscilloscope.analog[offset_channel_an: offset_channel_an*2] # Baseline
        y2 = bufOscilloscope.analog[offset_channel_an*2: offset_channel_an*3] # Triggered signal
        y3 = bufOscilloscope.analog[offset_channel_an*3: offset_channel_an*4] # Energy

        
        if Signal == 1:
            o1.setData(x=x, y=y0, pen = pg.mkPen(color='w', width=1))
            
        if Signal == 0:
            o1.setData(x=[], y=[])
        if Baseline == 1:
            o2.setData(x=x, y=y1, pen = pg.mkPen(color='g', width=1))
            
        if Baseline == 0:
            o2.setData(x=[], y=[])
            o2d.setData(x=[], y=[])
        if Triggered_signal == 1:
            o3.setData(x=x, y=y2, pen = pg.mkPen(color='r', width=1))
            
        if Triggered_signal == 0:
            o3.setData(x=[], y=[])
        if Energy == 1:
            o4.setData(x=x, y=y3, pen = pg.mkPen(color='y', width=1))
            
        if Energy == 0:
            o4.setData(x=[], y=[])

        medium_bl = np.mean(y1)
        y12 = bufOscilloscope.digital[offset_channel_dig * 1 + offset_channel_an: offset_channel_dig*1 + offset_channel_an*2] # Baseline run/hold      
        yy_run_bl = [] 
        xx_run_bl = []
        for u in range(len(y12)):
            if y12[u] == 0:
                xx_run_bl.append(u)
                yy_run_bl.append(2000)
            else:
                xx_run_bl.append(u)
                yy_run_bl.append(medium_bl)
        if Baseline_run == 1:
            o2d.setData(x=xx_run_bl, y=yy_run_bl, pen=pg.mkPen(color='g', width=2))
        if Baseline_run == 0:
            o2d.setData(x=[], y=[])

        y21 = bufOscilloscope.digital[offset_channel_dig * 2: offset_channel_dig*2 + offset_channel_an*1] # Trigger
        yy_trig = []
        xx_trig = []
        for u in range(len(y21)):
            if y21[u] == 1:
                xx_trig.append(u)
                yy_trig.append(y0[u])
            else:
                xx_trig.append(u)
                yy_trig.append(medium_bl)
        if Trigger == 1:
            o3d.setData(x=xx_trig, y=yy_trig, pen = pg.mkPen(color='r', width=2))
        if Trigger == 0:
            o3d.setData(x=[], y=[])
        
        y31 = bufOscilloscope.digital[offset_channel_dig * 3 + offset_channel_an: offset_channel_dig*3 + offset_channel_an*2] # Gate integration
        yy_gate = []
        xx_gate = []
        for u in range(len(y31)):
            if y31[u] == 1:
                xx_gate.append(u)
                yy_gate.append(2010)
            else:
                xx_gate.append(u)
                yy_gate.append(medium_bl)
        if Gate_integration == 1:
            o4d.setData(x=xx_gate, y=yy_gate, pen= pg.mkPen(color='y', width=2))
        if Gate_integration == 0:
            o4d.setData(x=[], y=[])

        
timer2 = QtCore.QTimer()
timer2.timeout.connect(update_oscilloscope)
timer2.start(500) # Emit the timeout() signal every 500 ms

############################################################################################################################
######### UPDATE PARAMETERS ################################################################################################

polarity_par = params.child('Polarity')
trigger_threshold_par = params.child('Trigger threshold')
N_sample_par = params.child('Number of sample')
time_hold_baseline_par = params.child('Time hold baseline')
time_integration_par = params.child('Time integration')
pre_integration_par = params.child('Pre-integration')
gain_par = params.child('Gain')
offset_integration_par = params.child('Offset spectrum')

def save():
    global polarity,trigger_threshold, number_of_samples, gain, time_hold_baseline, time_integration, pre_integration, offset_integration

    polarity = polarity_par.value()
    trigger_threshold = trigger_threshold_par.value()
    number_of_samples = N_sample_par.value()
    time_hold_baseline = time_hold_baseline_par.value()
    time_integration = time_integration_par.value()
    pre_integration = pre_integration_par.value()
    gain = gain_par.value()
    offset_integration = offset_integration_par.value()

    if polarity == 'Positive':
        res = sdk.SetRegister("board0:/Registers/pol", 1)
    else:
        res = sdk.SetRegister("board0:/Registers/pol", 0)
    if res != 0:
        print("Error setting polarity")

    res = sdk.SetRegister("board0:/Registers/trgthrs", trigger_threshold)
    if res != 0:
        print("Error setting trigger threshold")

    res = sdk.SetRegister("board0:/Registers/Nsample", number_of_samples)
    if res != 0:
        print("Error setting number of samples")
    
    res = sdk.SetRegister("board0:/Registers/time_hold_bs", time_hold_baseline)
    if res != 0:
        print("Error setting time hold baseline")
    
    res = sdk.SetRegister("board0:/Registers/time_integr", time_integration)
    if res != 0:
        print("Error setting time integration")

    res = sdk.SetRegister("board0:/Registers/pre_integr", pre_integration)
    if res != 0:
        print("Error setting pre-integration")
    
    res = sdk.SetRegister("board0:/Registers/gain", gain)
    if res != 0:
        print("Error setting gain")
    
    res = sdk.SetRegister("board0:/Registers/Offset_int", offset_integration)
    if res != 0:
        print("Error setting offset spectrum")



params.child('Save State').sigActivated.connect(save)
#params.param('Restore State').sigActivated.connect(restore)


def update_polarity(param):
    global polarity
    polarity = param.value()
polarity_par.sigValueChanged.connect(lambda param: update_polarity(param))

def update_trigger_threshold(param, sdk):
    global trigger_threshold
    trigger_threshold = params.value()
trigger_threshold_par.sigValueChanged.connect(lambda param: update_trigger_threshold(param))

def update_N_sample(param):
    global number_of_samples
    number_of_samples = param.value()
N_sample_par.sigValueChanged.connect(lambda param: update_N_sample(param))

def update_time_hold_baseline(param):
    global time_hold_baseline
    time_hold_baseline = param.value()
time_hold_baseline_par.sigValueChanged.connect(lambda param: update_time_hold_baseline(param))

def update_time_integration(param):
    global time_integration
    time_integration = param.value()
time_integration_par.sigValueChanged.connect(lambda param: update_time_integration(param))

def update_pre_integration(param):
    global pre_integration
    pre_integration = param.value()
pre_integration_par.sigValueChanged.connect(lambda param: update_pre_integration(param))

def update_gain(param):
    global gain
    gain = param.value()
gain_par.sigValueChanged.connect(lambda param: update_gain(param))

def update_offset_integration(param):
    global offset_integration
    offset_integration = param.value()
offset_integration_par.sigValueChanged.connect(lambda param: update_offset_integration(param))

##############################################################################################################
####### Start, stop and reset buttons ########################################################################

def stop():
    timer.stop()
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "")
    sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.stop", "")

def start():
    timer.start()
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")
    sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.start", "")

def reset():
    timer.stop()
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "")
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "")

    timer.start()
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")

stop_btn.clicked.connect(stop)
start_btn.clicked.connect(start)
reset_btn.clicked.connect(reset)


######################################################################################################
######### Channel buttons ############################################################################

def update_channel(param, channel_num):
    global Signal, Baseline, Triggered_signal, Energy, Baseline_run, Gate_integration, Trigger
    value = param.value()
    if channel_num == 0:
        Signal = value
        if value == True:
            legend.addItem(o1, 'Signal')
        else:
            legend.removeItem(o1)
    elif channel_num == 1:
        Baseline = value
        if value == True:
            legend.addItem(o2, 'Baseline')
        else:
            legend.removeItem(o2)
    elif channel_num == 2:
        Triggered_signal = value
        if value == True:
            legend.addItem(o3, 'Triggered signal')
        else:
            legend.removeItem(o3)
    elif channel_num == 3:
        Energy = value
        if value == True:
            legend.addItem(o4, 'Energy')
        else:
            legend.removeItem(o4)
    elif channel_num == 4:
        Baseline_run = value
        if value == True:
            legend.addItem(o2d, 'Baseline Gate')
        else:
            legend.removeItem(o2d)
    elif channel_num == 5:
        Gate_integration = value
        if value == True:
            legend.addItem(o4d, 'Gate integration')
        else:
            legend.removeItem(o4d)
    elif channel_num == 6:
        Trigger = value
        if value == True:
            legend.addItem(o3d, 'Trigger')
        else:
            legend.removeItem(o3d)
    else:
        legend.removeItem(o1)
        # handle error
        pass

# Connect parameter change signal to update_channel function
params_channel.child('Signal').sigValueChanged.connect(lambda param: update_channel(param, 0))
params_channel.child('Baseline').sigValueChanged.connect(lambda param: update_channel(param, 1))
params_channel.child('Baseline hold interval').sigValueChanged.connect(lambda param: update_channel(param, 4))
params_channel.child('Triggered signal').sigValueChanged.connect(lambda param: update_channel(param, 2))
params_channel.child('Energy').sigValueChanged.connect(lambda param: update_channel(param, 3))
params_channel.child('Gate integration').sigValueChanged.connect(lambda param: update_channel(param, 5))
params_channel.child('Trigger').sigValueChanged.connect(lambda param: update_channel(param, 6))


if __name__ == '__main__':
    pg.exec()
