import numpy as np
import pyqtgraph as pg
from pyqtgraph.Qt import QtGui, QtCore, QtWidgets
from PyQt5.QtWidgets import QApplication, QVBoxLayout, QHBoxLayout, QPushButton, QLineEdit, QLabel, QWidget, QTextEdit, QTreeWidgetItem, QTreeWidget
from scisdk.scisdk import SciSDK
import math
import scipy.optimize as opt
import ctypes
from pyqtgraph.parametertree import Parameter, ParameterTree
from pyqtgraph.parametertree import parameterTypes as pTypes

# Initialize sdk library
sdk = SciSDK()

# Add the DT1260 device to the sdk
res = sdk.AddNewDevice("usb:13250", "DT1260", "RegisterFile.json", "board0")

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

########################################################################################################################
####### GUI ############################################################################################################

app = pg.mkQApp("Spectrum_PyQtgraph")

###############################################################################
###### REGION CLASS############################################################

class Analysis(pTypes.GroupParameter):
    def __init__(self):
        self.centroid = 0.0
        self.centroid_keV = 0.0
        self.sigma = 0.0
        self.sigma_keV = 0.0
        self.resolution = 0.0
        self.resolution_keV = 0.0
        pTypes.GroupParameter.__init__(self, name='Parameters', type='group', children=[
            dict(name='Centroid', type='str', value=self.centroid, readonly=True),
            dict(name='Sigma', type='str', value=self.sigma, readonly=True),
            dict(name='Resolution', type='str', value=self.resolution, readonly=True),
        ])

    def setCentroid(self, centroid):
        self.centroid = centroid
        if self.centroid_keV == 0.0:
            self.child('Centroid').setValue('{:,.3f}'.format(centroid))
        else:
            self.child('Centroid').setValue('{:,.3f} (%s keV)'.format(centroid) % self.centroid_keV)
    def setCentroid_keV(self, centroid_keV):
        self.centroid_keV = centroid_keV
        self.child('Centroid').setValue('{:,.3f} (%s keV)'.format(self.centroid) % centroid_keV)
    def setSigma(self, sigma):
        self.sigma = sigma
        if self.sigma_keV == 0.0:
            self.child('Sigma').setValue('{:,.5}'.format(sigma))
        else:
            self.child('Sigma').setValue('{:,.5} (%s keV)'.format(sigma) % self.sigma_keV)
    def setSigma_keV(self, sigma_keV):
        self.sigma_keV = sigma_keV
        self.child('Sigma').setValue('{:,.5} (%s keV)'.format(self.sigma) % sigma_keV)
    def setResolution(self, resolution):
        self.resolution = resolution
        if self.resolution_keV == 0.0:
            self.child('Resolution').setValue('{:,.5}'.format(resolution))
        else:
            self.child('Resolution').setValue('{:,.5} (%s)'.format(resolution) % self.resolution_keV)
    def setResolution_keV(self, resolution_keV):
        self.resolution_keV = resolution_keV
        self.child('Resolution').setValue('{:,.5} (%s)'.format(self.resolution) % resolution_keV)
    def getCentroid(self):
        return self.centroid
    def getCentroid_keV(self):
        return self.centroid_keV
    def getSigma(self):
        return self.sigma
    def getResolution(self):
        return self.resolution
    
class Line(pTypes.SimpleParameter):
    def __init__(self, name, type, value):
        self.analysis = Analysis()
        self.interval = None
        self.num = 0
        self.plot = None
        self.peak = 0
        pTypes.SimpleParameter.__init__(self, name=name, type=type, value=value, children = [self.analysis], expanded=False)
  
    def setAnalysis(self, centroid, sigma, resolution):
        self.analysis.setCentroid(centroid)
        self.analysis.setSigma(sigma)
        self.analysis.setResolution(resolution)
    def setInterval(self, interval):
        self.interval = interval
    def setNum(self, num):  
        self.num = num
    def setPlot(self, plot):
        self.plot = plot
    def setPeak(self, peak):
        self.peak = peak
        print(f"setPeak called {self.peak}")
        self.analysis.setCentroid_keV(peak)
    def getAnalysis(self):
        return self.analysis
    def getInterval(self):
        return self.interval
    def getNum(self):
        return self.num
    def getPlot(self):
        return self.plot
    def getPeak(self):
        return self.peak


class Region(pTypes.GroupParameter):
    def __init__(self, name, num_lines):
        self.num_lines = num_lines
        self.lines = []
        for i in range(1, num_lines+1):
            line = Line(name=f'Line {i}', type='bool', value=False)
            line.setNum(i)
            #line.setPeak(i)
            line.sigValueChanged.connect(lambda line=line: self.updateLines(line))
            self.lines.append(line)
        pTypes.GroupParameter.__init__(self, name=name, type='group', children=self.lines)

    # Add a line
    def addLine(self):
        self.num_lines += 1
        line = Line(name=f'Line {self.num_lines}', type='bool', value=False)
        line.setNum(self.num_lines)
        line.sigValueChanged.connect(lambda line=line: self.updateLines(line))
        self.addChild(line)
        self.lines.append(line)
    
    def updateLines(self, line):
        value = line.value()
        if value:
            line.setOpts(expanded=value)

            line.setInterval(pg.LinearRegionItem([0, 1000], bounds=[0,16384], movable=True))
            pw2.addItem(line.getInterval())
            label = pg.InfLineLabel(line.getInterval().lines[1], f'line {line.getNum()}' , position=0.95, rotateAxis=(1,0), anchor=(1, 1))
        else:
            line.setOpts(expanded=value)
            pw2.removeItem(line.getInterval())

params_line = Region('Lines', 2)

params_line.addChild(dict(name='Add a Line', type='action'))
params_line.param("Add a Line").sigActivated.connect(params_line.addLine)

###############################################################################
###### PEAKS CLASS ############################################################

class Peak(pTypes.ListParameter):
    def __init__(self, num, region):
        self.energy_peak = 0
        self.region = region
        self.selected_line = None
        values = ["Choose a line"] + [line.name() for line in region.lines]
        pTypes.ListParameter.__init__(
            self,
            name=f"Peak {num}",
            limits=values,
            type="list",
            children=[dict(name='Energy: ', type="str", value=self.energy_peak)],
            expanded=False
        )

        self.sigValueChanged.connect(self.on_list_param_change)
        region.param('Add a Line').sigActivated.connect(self.update_limits)

    def update_limits(self):
        values = ["Choose a line"] + [line.name() for line in self.region.lines]
        self.setLimits(values)
    
    def on_list_param_change(self, param, changes):
        if param.value() == "Choose a line":
            self.setOpts(expanded=False)
            self.energy_peak = 0
            self.children()[0].setValue(self.energy_peak)
            self.selected_line = None

        else:
            self.setOpts(expanded=True)

            for line in self.region.lines:
                if line.name() == param.value():
                    self.energy_peak = line.getPeak()
                    self.children()[0].setValue(self.energy_peak)
                    self.selected_line = line
    
    def change_peak_value(self, param):
        self.energy_peak = param.value()
        print(f"change_peak_value called {self.energy_peak}")
        if self.selected_line is not None:
            self.selected_line.setPeak(self.energy_peak)
            self.children()[0].setValue(self.energy_peak)

class Peaks(pTypes.GroupParameter):
    def __init__(self, params_line):
        self.a = 0
        self.b = 0
        pTypes.GroupParameter.__init__(
            self, 
            name="Calibration", 
            type='group', 
            children=[Peak(1, params_line), Peak(2, params_line), dict(name='Calibrate', type= 'action'),]
            )
        self.param("Calibrate").sigActivated.connect(self.calibrate)

        #params_line.sigLineAdded.connect(self.update_limits)


        self.peak1 = self.param("Peak 1").child('Energy: ')
        self.peak2 = self.param("Peak 2").child('Energy: ')

        self.peak1.sigValueChanged.connect(lambda param: self.param('Peak 1').change_peak_value(param))
        self.peak2.sigValueChanged.connect(lambda param: self.param('Peak 2').change_peak_value(param))
    
    def calibrate(self):
        energy1 = float(self.peak1.value())
        energy2 = float(self.peak2.value())
        x0Fit1 = float(self.param("Peak 1").selected_line.getAnalysis().getCentroid())
        x0Fit2 = float(self.param("Peak 2").selected_line.getAnalysis().getCentroid())
        #print(f"calibrate called {energy1} {energy2} {x0Fit1} {x0Fit2}")

        if energy1 != 0 and energy2 != 0:
            self.a = (energy2 - energy1)/(x0Fit2 - x0Fit1)
            self.b = (energy1*x0Fit2 - energy2*x0Fit1)/(x0Fit2 - x0Fit1)


params_peak = Peaks(params_line)

children = [
    params_line,
    params_peak,
]

# Create total tree
my_parameter = Parameter.create(name='Parameters', type='group', children=children)
pt = ParameterTree() #showHeader=True 
pt.setParameters(my_parameter)
pt.setFixedWidth(450)


pw2 = pg.PlotWidget(title = "Spectrum", labels={'left': 'Counts', 'bottom': 'Channels'})
p1 = pw2.plot()

# Create horizontal layout to add parameter tree and vertical layout
h_layout = QHBoxLayout()
h_layout.addWidget(pt)
h_layout.addWidget(pw2)

# Create QWidget to hold the horizontal layout
widget = QWidget()
widget.setLayout(h_layout)

# Show widget
widget.show()

###############################################################################
###### PLOT SPECTRUM ##########################################################
def gaussian(x, a, x0, sigma):
    return a * np.exp(-(x-x0)**2/(2*sigma**2))

def update_plot():
    global obSpectrum
    # Read data from the spectrum analyzer
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
                #print(line.getPeak())
                

                #Plot the fitted curve
                ygauss = gaussian(x_fit, *popt)
                line.plot.setData(x=x_fit, y=ygauss, pen=None, symbol='o', symbolPen=None, symbolSize=2, symbolBrush=('r'))

                if params_peak.a != 0 and params_peak.b != 0:
                    # print(params_peak.a, params_peak.b)
                    x0_fit_keV = x0_fit * params_peak.a + params_peak.b
                    sigma_fit_keV = sigma_fit * params_peak.a
                    resolution_keV = sigma_fit_keV/x0_fit_keV*100

                    line.getAnalysis().setCentroid_keV(round(x0_fit_keV,3))
                    line.getAnalysis().setSigma_keV(round(sigma_fit_keV,5))
                    line.getAnalysis().setResolution_keV(round(resolution_keV,5))


# Create a timer object and set its interval
timer = QtCore.QTimer()
timer.timeout.connect(update_plot)
timer.start(50) # Emit the timeout() signal every 50 ms


if __name__ == '__main__':
    pg.exec()