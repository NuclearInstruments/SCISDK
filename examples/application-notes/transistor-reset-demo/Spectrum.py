from scisdk.scisdk import SciSDK
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import Button
from matplotlib.widgets import TextBox
import math
import scipy.optimize as opt
import numpy as np


tau = 1000.0
centroid = 0.0
sigma_fit = 0.0

# Initialize sdk library
sdk = SciSDK()

# Add the DT1260 device to the sdk
res = sdk.AddNewDevice("192.168.102.220:8888", "DT5560", "./RegisterFile.json", "board0")

if res != 0:
    print("Error adding device")
    exit()

# Set register values
res = sdk.SetRegister("board0:/Registers/RUNREG", 0)

res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.RESET_THRS", 500)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.POL", 1)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.RESET_LEN", 5000)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.HIST", 25)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.INIB", 50)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.THRS", 800)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.AN_OFS", 0)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRIGGER_K", 5)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRIGGER_M", 7)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_K", 100)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_M", 110)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.DECONV_M", int(256.0/(math.exp(8/tau)-1)))
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_GAIN", 3000)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.BLLEN", 7)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.BLINIB", 150)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.SAMP_POS", 107)
res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRG_MODE", 0)

res = sdk.SetRegister("board0:/Registers/RUNREG", 1)


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

if res != 0:
    res, err = sdk.s_error(res)
    print("Allocate spectrum buffer error:", err)
    exit(-1)

def gaussian(x, a, x0, sigma):
    return a * np.exp(-(x-x0)**2/(2*sigma**2))

# Plot data
fig = plt.figure("Spectrum Data")
ax1 = fig.add_subplot(1, 1, 1)
ax1.set_position([0.15, 0.08, 0.8, 0.8])



def updateGraph(i, obSpectrum):
    global sigma_fit
    res, obSpectrum = sdk.ReadData("board0:/MMCComponents/Spectrum_0",
                                   obSpectrum)
    if res == 0:
        xar = []
        yar = []
        for index in range(obSpectrum.info.valid_bins):
            xar.append(index)
            yar.append(obSpectrum.data[index])
        ax1.clear()
        ax1.plot(xar, yar)

        if centroid != 0.0:
            a_guess = max(yar)
            x0_guess = centroid  # initial guess for centroid (mean)
            sigma_guess = 10  # initial guess for standard deviation

            min_bin = int(centroid - 300)
            max_bin = int(centroid + 300)
            x_fit = xar[min_bin:max_bin]
            y_fit = yar[min_bin:max_bin]

            popt, pcov = opt.curve_fit(gaussian, x_fit, y_fit, p0=[a_guess, x0_guess, sigma_guess])
            # Extract the fitted parameters
            a_fit, x0_fit, sigma_fit = popt
            ax1.axvline(x=x0_fit, color='g', label='Centroid')
            # plot the fitted curve
            ax1.plot(x_fit, gaussian(x_fit, *popt), 'r-', label='fit')
            sigma_box.set_val(str(sigma_fit))
            centroid_box.set_val(str(x0_fit))

ani = animation.FuncAnimation(fig, updateGraph, fargs=[obSpectrum, ],
                              interval=100)


def stop(event):
    ani.event_source.stop()
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "")


def start(event):
    ani.event_source.start()
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")


def reset(event):
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "")
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "")
    sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")


stop_button = Button(plt.axes([0.85, 0.9, 0.1, 0.075]), 'Stop')
stop_button.on_clicked(stop)

start_button = Button(plt.axes([0.75, 0.9, 0.1, 0.075]), 'Start')
start_button.on_clicked(start)

reset_button = Button(plt.axes([0.65, 0.9, 0.1, 0.075]), 'Reset')
reset_button.on_clicked(reset)


def set_centroid(text):
    global centroid 
    centroid= int(text)
    
    
centroid_in_box = TextBox(plt.axes([0.07, 0.9, 0.1, 0.04]), 'Input Centroid: ')
centroid_in_box.on_submit(set_centroid)

sigma_box = TextBox(plt.axes([0.27, 0.9, 0.1, 0.04]), 'Sigma: ')
sigma_box.set_active(False)

centroid_box = TextBox(plt.axes([0.47, 0.9, 0.1, 0.04]), 'Centroid: ')
centroid_box.set_active(False)


def set_reset_thrs(text):
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.RESET_THRS", int(text))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)

def set_reset_len(text):
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.RESET_LEN", int(text))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)

def set_hist(text):
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.HIST", int(text))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)

def set_inib(text):
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.INIB", int(text))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)

def set_thrs(text):
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.THRS", int(text))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)

def set_an_ofs(text):
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.AN_OFS", int(text))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)

def set_trigger_k(text):
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRIGGER_K", int(text))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)

def set_trigger_m(text):
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRIGGER_M", int(text))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)

def set_trap_k(text):
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_K", int(text))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)

def set_trap_m(text):
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_M", int(text))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)

def set_deconv_m(text):
    tau = int(text)
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.DECONV_M", int(256.0/(math.exp(8/tau)-1)))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)

def set_trap_gain(text):
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRAP_GAIN", int(text))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)

def set_bllen(text):
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.BLLEN", int(text))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)

def set_blinib(text):
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.BLINIB", int(text))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)

def set_samp_pos(text):
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.SAMP_POS", int(text))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)

def set_trg_mode(text):
    res = sdk.SetRegister("board0:/Registers/RUNREG", 0)
    res = sdk.SetParameterInteger("board0:/MMCComponents/RG_0.TRG_MODE", int(text))
    res = sdk.SetRegister("board0:/Registers/RUNREG", 1)



reset_thrs_box = TextBox(plt.axes([0.07, 0.8, 0.04, 0.04]), 'Reset_thrs: ', initial='500')
reset_thrs_box.on_submit(set_reset_thrs)

reset_len_box = TextBox(plt.axes([0.07, 0.75, 0.04, 0.04]), 'Reset_len: ', initial='5000')
reset_len_box.on_submit(set_reset_len)

hist_box = TextBox(plt.axes([0.07, 0.70, 0.04, 0.04]), 'Hist: ', initial='25')
hist_box.on_submit(set_hist)

inib_box = TextBox(plt.axes([0.07, 0.65, 0.04, 0.04]), 'Inib: ', initial='50')
inib_box.on_submit(set_inib)

thrs_box = TextBox(plt.axes([0.07, 0.60, 0.04, 0.04]), 'Thrs: ', initial='800')
thrs_box.on_submit(set_thrs)

an_ofs_box = TextBox(plt.axes([0.07, 0.55, 0.04, 0.04]), 'An_ofs: ', initial='0')
an_ofs_box.on_submit(set_an_ofs)

trigger_k_box = TextBox(plt.axes([0.07, 0.50, 0.04, 0.04]), 'Trigger_k: ', initial='5')
trigger_k_box.on_submit(set_trigger_k)

trigger_m_box = TextBox(plt.axes([0.07, 0.45, 0.04, 0.04]), 'Trigger_m: ', initial='7')
trigger_m_box.on_submit(set_trigger_m)

trap_k_box = TextBox(plt.axes([0.07, 0.40, 0.04, 0.04]), 'Trap_k: ', initial='100')
trap_k_box.on_submit(set_trap_k)

trap_m_box = TextBox(plt.axes([0.07, 0.35, 0.04, 0.04]), 'Trap_m: ', initial='110')
trap_m_box.on_submit(set_trap_m)

deconv_m_box = TextBox(plt.axes([0.07, 0.30, 0.04, 0.04]), 'Tau: ', initial = int(256.0/(math.exp(8/tau)-1)))
deconv_m_box.on_submit(set_deconv_m)

trap_gain_box = TextBox(plt.axes([0.07, 0.25, 0.04, 0.04]), 'trap_gain: ', initial='3000')
trap_gain_box.on_submit(set_trap_gain)

bllen_box = TextBox(plt.axes([0.07, 0.20, 0.04, 0.04]), 'Bllen: ', initial='7')
bllen_box.on_submit(set_bllen)

blinib_box = TextBox(plt.axes([0.07, 0.15, 0.04, 0.04]), 'blinib: ', initial='150')
blinib_box.on_submit(set_blinib)

samp_pos_box = TextBox(plt.axes([0.07, 0.10, 0.04, 0.04]), 'Samp_pos: ', initial='107')
samp_pos_box.on_submit(set_samp_pos)

trg_mode_box = TextBox(plt.axes([0.07, 0.05, 0.04, 0.04]), 'Trg_mode: ', initial='0')
trg_mode_box.on_submit(set_trg_mode)   
    

plt.show()

sdk.FreeBuffer("board0:/MMCComponents/Spectrum_0", obSpectrum)
