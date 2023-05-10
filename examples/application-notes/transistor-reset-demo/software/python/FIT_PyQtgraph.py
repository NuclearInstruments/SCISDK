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
res = sdk.AddNewDevice("192.168.102.220:8888", "DT5560", "RegisterFile.json", "board0")

if res != 0:
    print("Error adding device")
    exit()


### SPECTRUM

# Allocate buffer
res, obSpectrum = sdk.AllocateBuffer("board0:/MMCComponents/Spectrum_0")

if res != 0:
    res, err = sdk.s_error(res)
    print("Allocate spectrum buffer error:", err)
    exit(-1)



def gaussian(x, a, x0, sigma):
    return a * np.exp(-(x-x0)**2/(2*sigma**2))

########################################################################################################################
####### GUI ############################################################################################################

app = pg.mkQApp("Spectrum_PyQtgraph")



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
    dict(name='Add a Line', type='action')
]

params_line = Parameter.create(name='Lines', type='group', children=line)

line1_param = params_line.child('Line 1')
line1_an_param = Parameter.create(name='Parameters', type='group', children=fin_an1, expanded=False)
line1_param.addChild(line1_an_param)
line2_param = params_line.child('Line 2')
line2_an_param = Parameter.create(name='Parameters', type='group', children=fin_an2, expanded=False)
line2_param.addChild(line2_an_param)

peaks = [
    dict(name='Peak 1', type='str', value=energy_peak, suffix= 'keV', siPrefix=True),
    dict(name='Peak 2', type='str', value=energy_peak2, suffix='keV', siPrefix=True),
    dict(name='Calibrate', type= 'action'),
]
param_peak = Parameter.create(name='Peaks', type='group', children=peaks)

children = [
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


# Define function in the spectrum plot
p1 = pw2.plot()
p2 = pw2.plot()
p3 = pw2.plot()

legend = pg.LegendItem((100,60), offset=(400,30))

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



# Create horizontal layout to add parameter tree and vertical layout
h_layout = QHBoxLayout()
#h_layout.addWidget(tree)
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
            #line2_region.sigRegionChangeFinished.connect(lambda: region_changed(2))
        else:
            pw2.removeItem(line2_region)
            line2_region = None
    if line_num == 3:
        line3 = value
        line_an_par.setOpts(expanded=True)
        if value:
            line3_region = pg.LinearRegionItem([0, 1000], bounds=[0,16384], movable=True) # (initial values, limits, is movable)
            pw2.addItem(line3_region)
            label3 = pg.InfLineLabel(line3_region.lines[1], "Line 3", position=0.95, rotateAxis=(1,0), anchor=(1, 1))
        else:
            pw2.removeItem(line3_region)
            line3_region = None
    if line_num == 4:
        line4 = value
        line_an_par.setOpts(expanded=True)
        if value:
            line4_region = pg.LinearRegionItem([0, 1000], bounds=[0,16384], movable=True) # (initial values, limits, is movable)
            pw2.addItem(line4_region)
            label4 = pg.InfLineLabel(line4_region.lines[1], "Line 4", position=0.95, rotateAxis=(1,0), anchor=(1, 1))
        else:
            pw2.removeItem(line4_region)
            line4_region = None
    


# Connect parameter change signal to update_channel function

line1_param.sigValueChanged.connect(lambda param: update_line(param, line1_an_param,1))
line2_param.sigValueChanged.connect(lambda param: update_line(param, line2_an_param,2))


# def region_changed(num):
#     if num == 1:
#         print("Line 1 extremes: " + str(line1_region.getRegion()))
#     if num == 2:
#         print("Line 2 extremes: " + str(line2_region.getRegion()))

centr = 0.0
sigma = 0.0
resol = 0.0
def add_a_line():
    global i
    new_line =  dict(name=f'Line {i}', type='bool', value=False)
    params_line.addChild(new_line)
    new_line_param = params_line.child(f'Line {i}')
    fin_an = [
        dict(name='Centroid', type='str', value=centr, readonly=True),
        dict(name='Sigma', type='str', value=sigma, readonly=True),
        dict(name='Resolution', type='str', value=resol, readonly=True),   
    ]
    line_an_param = Parameter.create(name='Parameters', type='group', children=fin_an, expanded=False)
    new_line_param.addChild(line_an_param)
    if i == 3:
        new_line_param.sigValueChanged.connect(lambda param: update_line(param, line_an_param, 3))
    if i == 4:
        new_line_param.sigValueChanged.connect(lambda param: update_line(param, line_an_param, 4))
    i += 1

i = 3
params_line.param("Add a Line").sigActivated.connect(add_a_line)




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
        # handle error
        pass


         
# Create a timer object and set its interval
timer = QtCore.QTimer()
timer.timeout.connect(update_plot)
timer.start(50) # Emit the timeout() signal every 50 ms


##############################################################################################################
####### Start, stop and reset buttons ########################################################################

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

if __name__ == '__main__':
    pg.exec()
