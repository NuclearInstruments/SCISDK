import numpy as np
import pyqtgraph as pg
from pyqtgraph.Qt import QtGui, QtCore, QtWidgets
from PyQt5.QtWidgets import QApplication, QVBoxLayout, QHBoxLayout, QPushButton, QLineEdit, QLabel, QWidget
from scisdk.scisdk import SciSDK
import math
import scipy.optimize as opt
import ctypes

tau = 100.0
centroid = 0.0
sigma_fit = 0.0
decimator = 1
# Initialize sdk library
sdk = SciSDK()

# Add the DT1260 device to the sdk
res = sdk.AddNewDevice("192.168.102.220:8888", "DT5560", "RegisterFile.json", "board0")

if res != 0:
    print("Error adding device")
    exit()


RESET_THRS=500
POL=1
RESET_LEN = 5000
HIST = 25
INIB = 50
THRS = 800
AN_OFS = 0
TRIGGER_K = 5
TRIGGER_M = 7
TRAP_K = 100
TRAP_M = 110
DECONV_M = 300
TRAP_GAIN = 5000
BLLEN = 7
BLINIB = 250
SAMP_POS = 107
TRG_MODE = 0

# Set register values
res = sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
res = sdk.SetRegister("board0:/Registers/MONSEL", 3)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.RESET_THRS", RESET_THRS)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.POL", POL)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.RESET_LEN",  RESET_LEN)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.HIST",    HIST)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.INIB",   INIB)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.THRS",  THRS)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.AN_OFS",  AN_OFS)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRIGGER_K", TRIGGER_K)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRIGGER_M", TRIGGER_M)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_K", TRAP_K)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_M", TRAP_M)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.DECONV_M", int(256.0/(math.exp(8/DECONV_M)-1)))
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_GAIN", TRAP_GAIN)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.BLLEN", BLLEN)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.BLINIB", BLINIB)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.SAMP_POS", SAMP_POS)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRG_MODE", TRG_MODE)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.PUR_MODE", 2)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.PUR_INIB", 50)

res = sdk.SetRegister("board0:/Registers/RUN_CFG", 1)

# ## SPECTRUM

# Set spectrum parameters
sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.rebin", "0")
sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.limitmode", "freerun")

# Execute command reset
sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "")

# Execute command start
sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")

# Allocate buffer
res, obSpectrum = sdk.AllocateBuffer("board0:/MMCComponents/Spectrum_0")


### OSCILLOSCOPE

res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing","decoded")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", 2000)
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode","ext")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 2000)
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150)
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", decimator)
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "non-blocking")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.timeout", 3000)

res, bufOscilloscope = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0")


if res != 0:
    res, err = sdk.s_error(res)
    print("Allocate spectrum buffer error:", err)
    exit(-1)

def gaussian(x, a, x0, sigma):
    return a * np.exp(-(x-x0)**2/(2*sigma**2))

app = pg.mkQApp("Spectrum_PyQtgraph")

# We create the ParameterTree
children = [
    dict(name='Monitor', type='list', limits=['Delta', 'Trig-Trap', 'Trap', 'Trap-BL', 'BL' ], value='Trap-BL'),
    dict(name='Reset trigger', type='int', value=RESET_THRS),
    dict(name='Polarity', type='list', limits=['Positive', 'Negative'], value='Positive'),
    dict(name='Reset len', type='int', value=RESET_LEN),
    dict(name='Hist', type='int', value=HIST),
    dict(name='Inib', type='int', value=INIB),
    dict(name='Threshold', type='int', value=THRS),
    dict(name='Analog offset', type='int', value=AN_OFS),
    dict(name='Trigger K', type='int', value=TRIGGER_K),
    dict(name='Triggem M', type='int', value=TRIGGER_M),
    dict(name='Trap K', type='int', value=TRAP_K),
    dict(name='Trap M', type='int', value=TRAP_M),
    dict(name='Tau', type='int', value=DECONV_M),
    dict(name='Trap gain', type='int', value=TRAP_GAIN),
    dict(name='Baseline len', type='int', value=BLLEN),
    dict(name='Baseline inib', type='int', value=BLINIB),
    dict(name='Sample pos', type='int', value=SAMP_POS),
    dict(name='Trigger mode', type='int', value=TRG_MODE),
]


# Create parameter tree
params = pg.parametertree.Parameter.create(name='Parameters', type='group', children=children)
pt = pg.parametertree.ParameterTree(showHeader=True )
pt.setParameters(params)
pt.setFixedWidth(400)

# Create start, stop, and reset buttons
start_btn = QPushButton('Start')
stop_btn = QPushButton('Stop')
reset_btn = QPushButton('Reset')

# Create plot widget
pw2 = pg.PlotWidget()
pw3 = pg.PlotWidget()


# Add plot widget and buttons to vertical layout
plot_layout = QVBoxLayout()
button_layout = QHBoxLayout()
button_layout.addWidget(start_btn)
button_layout.addWidget(stop_btn)
button_layout.addWidget(reset_btn)
plot_layout.addLayout(button_layout)
plot_layout_osc = QVBoxLayout()
plot_layout.addLayout(button_layout)
plot_layout.addWidget(pw2)
plot_layout.addWidget(pw3)



lr = pg.LinearRegionItem([7500, 8500], bounds=[0,16384], movable=True)
pw2.addItem(lr)
p1 = pw2.plot()
p2 = pw2.plot()
o1 = pw3.plot()
o2 = pw3.plot()
o1d = pw3.plot()
o2d = pw3.plot()
o3d = pw3.plot()
o4d = pw3.plot()
# Create horizontal layout to add parameter tree and vertical layout
h_layout = QHBoxLayout()
h_layout.addWidget(pt)
h_layout.addLayout(plot_layout)

# Create QWidget to hold the horizontal layout
widget = QWidget()
widget.setLayout(h_layout)

text_fit = pg.TextItem("")
pw2.addItem(text_fit)

# Show widget
widget.show()


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
        #pw2.clear() 
        #pw2.addItem(pg.PlotDataItem(xar, yar))
        p1.setData(x=xar, y=yar)
        
        roi = lr.getRegion()
        center = (roi[1] + roi[0]) / 2

        if center != 0.0:
            a_guess = max(yar)
            x0_guess = center  # initial guess for centroid (mean)
            sigma_guess = 10  # initial guess for standard deviation

            min_bin = int( roi[0])
            max_bin = int( roi[1])
            x_fit = xar[min_bin:max_bin]
            y_fit = yar[min_bin:max_bin]

            popt, pcov = opt.curve_fit(gaussian, x_fit, y_fit, p0=[a_guess, x0_guess, sigma_guess])
            a_fit, x0_fit, sigma_fit = popt
            resolution = sigma_fit/x0_fit*100
            print("a_fit: ", a_fit, "x0_fit: ", x0_fit, "sigma_fit: ", sigma_fit, "   resoultion: ", resolution)
            ygusss = gaussian(x_fit, *popt)
            p2.setData(x=x_fit, y=ygusss, pen=None, symbol='o', symbolPen=None, symbolSize=2, symbolBrush=('r')) 
            text_fit.setText("res: " + str(round(resolution,2)))
            text_fit.setPos(x0_fit,a_fit)
            # Extract the fitted parameters
           
            #ax1.axvline(x=x0_fit, color='g', label='Centroid')
            # plot the fitted curve
            #ax1.plot(x_fit, gaussian(x_fit, *popt), 'r-', label='fit')
            #sigma_box.set_val(str(sigma_fit))
            #entroid_box.set_val(str(x0_fit))


# Create a timer object and set its interval
timer = QtCore.QTimer()
timer.timeout.connect(update_plot)
#update_plot(obSpectrum)

timer.start(50) # Update the plot every 10 ms




def update_oscilloscope():
    global bufOscilloscope
    res, bufOscilloscope= sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", bufOscilloscope)
    if res == 0:
        x, y0, y1, y2, y3 = [], [], [], [], []
        y11, y12, y21, y31, y32, y33 = [], [], [], [], [], []

        offset_channel_dig = bufOscilloscope.info.samples_digital * bufOscilloscope.info.tracks_digital_per_channel
        offset_channel_an = bufOscilloscope.info.samples_analog

        x = np.arange(0, bufOscilloscope.info.samples_analog, decimator)
        # slice bufOscilloscope.analog from 0 to bufOscilloscope.info.samples_analog
        y0 = bufOscilloscope.analog[0:bufOscilloscope.info.samples_analog]
        y1 = bufOscilloscope.analog[bufOscilloscope.info.samples_analog: bufOscilloscope.info.samples_analog*2]
        y_trinib = bufOscilloscope.digital[bufOscilloscope.info.samples_digital*0:bufOscilloscope.info.samples_digital*1]
        y_trigger = bufOscilloscope.digital[bufOscilloscope.info.samples_digital*3:bufOscilloscope.info.samples_digital*4]
        y_bl = bufOscilloscope.digital[offset_channel_dig*1 + bufOscilloscope.info.samples_digital*0: offset_channel_dig*1 + bufOscilloscope.info.samples_digital*1]
        y_es = bufOscilloscope.digital[offset_channel_dig*1 + bufOscilloscope.info.samples_digital*1: offset_channel_dig*1 + bufOscilloscope.info.samples_digital*2]
        y1 = np.array(y1).astype(np.int16)
        yy_trigger = [] 
        xx_trigger = []
        for u in range(len(y_trigger)):
            if y_trigger[u] == 1:
                xx_trigger.append(u)
                yy_trigger.append(y0[u])

        yy_es = [] 
        xx_es = []
        for u in range(len(y_es)):
            if y_es[u] == 1:
                xx_es.append(u)
                yy_es.append(y1[u])

        yy_trinib = [] 
        xx_trinib = []
        for u in range(len(y_trinib)):
            if y_trinib[u] == 1:
                xx_trinib.append(u)
                yy_trinib.append(1)
   
        yy_bl_inib = [] 
        xx_bl_inib = []
        for u in range(len(y_bl)):
            if y_bl[u] == 1:
                xx_bl_inib.append(u)
                yy_bl_inib.append(100)
        # convert y1 from uint16 to int16
        
        o1.setData(x=x, y=y0)
        o2.setData(x=x, y=y1)
        o1d.setData(x=xx_trigger, y=yy_trigger,  pen=None, symbol='o', symbolPen=None, symbolSize=4, symbolBrush=('m'  ) )
        o2d.setData(x=xx_es, y=yy_es,  pen=None, symbol='o', symbolPen=None, symbolSize=4, symbolBrush=('g'  ) )
        o3d.setData(x=xx_trinib, y=yy_trinib,  pen=None, symbol='x', symbolPen=None, symbolSize=8, symbolBrush=('r'  ) )
        o4d.setData(x=xx_bl_inib, y=yy_bl_inib,  pen=None, symbol='x', symbolPen=None, symbolSize=2, symbolBrush=('y'  ) )
#            y11.append((bufOscilloscope.digital[index + offset_channel_dig*1]) )
#            y12.append((bufOscilloscope.digital[index + offset_channel_dig*1 + bufOscilloscope.info.samples_digital]) )       
#            y21.append((bufOscilloscope.digital[index + offset_channel_dig*2]) )             
#            y31.append((bufOscilloscope.digital[index + offset_channel_dig*3]) )
#            y32.append((bufOscilloscope.digital[index + offset_channel_dig*3 + bufOscilloscope.info.samples_digital]) )
#            y33.append((bufOscilloscope.digital[index + offset_channel_dig*3 + bufOscilloscope.info.samples_digital*2]) )
        
timer2 = QtCore.QTimer()
timer2.timeout.connect(update_oscilloscope)
timer2.start(500) # Update the plot every 10 ms


def update_reset_trigger(param, sdk):
    reset_trigger = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.RESET_THRS", reset_trigger)
    if res != 0:
        print("Error setting reset trigger")

reset_trigger_param = params.child('Reset trigger')
reset_trigger_param.sigValueChanged.connect(lambda param: update_reset_trigger(param, sdk))

def update_polarity(param, sdk):
    polarity = param.value()
    if polarity == 'Positive':
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.POL", 1)
    else:
        res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.POL", 0)
    if res != 0:
        print("Error setting polarity")

polarity_param = params.child('Polarity')
polarity_param.sigValueChanged.connect(lambda param: update_polarity(param, sdk))

def update_oscmon(param, sdk):
    mon = param.value()
    monsel = 0
    if mon == 'Delta':
        monsel = 0
    if mon == 'Trig-Trap':
        monsel = 1
    if mon == 'Trap':
        monsel = 2
    if mon == 'Trap-BL':
        monsel = 3
    if mon == 'BL':
        monsel = 4
    res = sdk.SetRegister("board0:/Registers/MONSEL", (monsel) )
    if res != 0:
        print("Error setting Monitor")
    
oscmon_param = params.child('Monitor')
oscmon_param.sigValueChanged.connect(lambda param: update_oscmon(param, sdk))

def update_reset_len(param, sdk):
    reset_len = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.RESET_LEN", reset_len)
    if res != 0:
        print("Error setting reset len")

reset_len_param = params.child('Reset len')
reset_len_param.sigValueChanged.connect(lambda param: update_reset_len(param, sdk))

def update_hist(param, sdk):
    hist = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.HIST", hist)
    if res != 0:
        print("Error setting hist")

hist_param = params.child('Hist')
hist_param.sigValueChanged.connect(lambda param: update_hist(param, sdk))

def update_inib(param, sdk):
    inib = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.INIB", inib)
    if res != 0:
        print("Error setting inib")

inib_param = params.child('Inib')
inib_param.sigValueChanged.connect(lambda param: update_inib(param, sdk))

def update_thrs(param, sdk):
    thrs = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.THRS", thrs)
    if res != 0:
        print("Error setting thrs")

thrs_param = params.child('Threshold')
thrs_param.sigValueChanged.connect(lambda param: update_thrs(param, sdk))

def update_an_ofs(param, sdk):
    an_ofs = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.AN_OFS", an_ofs)
    if res != 0:
        print("Error setting an ofs")

an_ofs_param = params.child('Analog offset')
an_ofs_param.sigValueChanged.connect(lambda param: update_an_ofs(param, sdk))

def update_trigger_k(param, sdk):
    trigger_k = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRIGGER_K", trigger_k)
    if res != 0:
        print("Error setting trigger k")

trigger_k_param = params.child('Trigger K')
trigger_k_param.sigValueChanged.connect(lambda param: update_trigger_k(param, sdk))

def update_trigger_m(param, sdk):
    trigger_m = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRIGGER_M", trigger_m)
    if res != 0:
        print("Error setting trigger m")

trigger_m_param = params.child('Triggem M')
trigger_m_param.sigValueChanged.connect(lambda param: update_trigger_m(param, sdk))

def update_trap_k(param, sdk):
    trap_k = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_K", trap_k)
    if res != 0:
        print("Error setting trap k")

trap_k_param = params.child('Trap K')
trap_k_param.sigValueChanged.connect(lambda param: update_trap_k(param, sdk))

def update_trap_m(param, sdk):
    trap_m = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_M", trap_m)
    if res != 0:
        print("Error setting trap m")

trap_m_param = params.child('Trap M')
trap_m_param.sigValueChanged.connect(lambda param: update_trap_m(param, sdk))

def update_tau(param, sdk):
    tau = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.DECONV_M", int(256.0/(math.exp(8/tau)-1)))
    if res != 0:
        print("Error setting deconv m")

tau_param = params.child('Tau')
tau_param.sigValueChanged.connect(lambda param: update_tau(param, sdk))

def update_trap_gain(param, sdk):
    trap_gain = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_GAIN", trap_gain)
    if res != 0:
        print("Error setting trap gain")

trap_gain_param = params.child('Trap gain')
trap_gain_param.sigValueChanged.connect(lambda param: update_trap_gain(param, sdk))

def update_bllen(param, sdk):
    bllen = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.BLLEN", bllen)
    if res != 0:
        print("Error setting bllen")

bllen_param = params.child('Baseline len')
bllen_param.sigValueChanged.connect(lambda param: update_bllen(param, sdk))

def update_blinib(param, sdk):
    blinib = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.BLINIB", blinib)
    if res != 0:
        print("Error setting blinib")

blinib_param = params.child('Baseline inib')
blinib_param.sigValueChanged.connect(lambda param: update_blinib(param, sdk))

def update_samp_pos(param, sdk):
    samp_pos = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.SAMP_POS", samp_pos)
    if res != 0:
        print("Error setting samp pos")

samp_pos_param = params.child('Sample pos')
samp_pos_param.sigValueChanged.connect(lambda param: update_samp_pos(param, sdk))

def update_trg_mode(param, sdk):
    trg_mode = param.value()
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRG_MODE", trg_mode)
    if res != 0:
        print("Error setting trg mode")

trg_mode_param = params.child('Trigger mode')
trg_mode_param.sigValueChanged.connect(lambda param: update_trg_mode(param, sdk))

def stop():
    timer.stop()
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "")

def start():
    sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
    sdk.SetRegister("board0:/Registers/RUN_CFG", 1)

    timer.start()
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")

def reset():
    timer.stop()
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "")
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "")
    sdk.SetRegister("board0:/Registers/RUN_CFG", 0)
    sdk.SetRegister("board0:/Registers/RUN_CFG", 1)

    timer.start()
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")

# Connect buttons to actions
stop_btn.clicked.connect(stop)
start_btn.clicked.connect(start)
reset_btn.clicked.connect(reset)


if __name__ == '__main__':
    pg.exec()
    #app.exec_()