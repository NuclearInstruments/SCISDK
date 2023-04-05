from scisdk.scisdk import SciSDK
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import Button
from matplotlib.widgets import TextBox


# Initialize sdk library
sdk = SciSDK()

# Add the DT1260 device to the sdk
res = sdk.AddNewDevice("usb:13250", "DT1260", "RegisterFile.json", "board0")

if res != 0:
    print("Error adding device")
    exit()

# Set register values
res = sdk.SetRegister("board0:/Registers/pol", 1)
res = sdk.SetRegister("board0:/Registers/trgthrs", 2000)
res = sdk.SetRegister("board0:/Registers/Nsample", 7)
res = sdk.SetRegister("board0:/Registers/time_hold_bs", 100)
res = sdk.SetRegister("board0:/Registers/time_integr", 76)
res = sdk.SetRegister("board0:/Registers/pre_integr", 5)
res = sdk.SetRegister("board0:/Registers/gain", 30000)
res = sdk.SetRegister("board0:/Registers/Offset_int", 0)

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

# Plot data
fig = plt.figure("Spectrum Data")
ax1 = fig.add_subplot(1, 1, 1)
ax1.set_position([0.15, 0.08, 0.8, 0.8])


def updateGraph(i, obSpectrum):
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

def set_gain(text):
    res = sdk.SetRegister("board0:/Registers/gain", int(text))

def set_offset(text):
    res = sdk.SetRegister("board0:/Registers/Offset_int", int(text))

def set_trgthrs(text):
    res = sdk.SetRegister("board0:/Registers/trgthrs", int(text))

def set_Nsample(text):
    res = sdk.SetRegister("board0:/Registers/Nsample", int(text))

def set_time_hold_bs(text):
    res = sdk.SetRegister("board0:/Registers/time_hold_bs", int(text))

def set_time_integr(text):
    res = sdk.SetRegister("board0:/Registers/time_integr", int(text))

def set_pre_integr(text):
    res = sdk.SetRegister("board0:/Registers/pre_integr", int(text))

gain_box = TextBox(plt.axes([0.07, 0.8, 0.04, 0.04]), 'Gain: ', initial='30000')
gain_box.on_submit(set_gain)

offset_box = TextBox(plt.axes([0.07, 0.7, 0.04, 0.04]), 'Offset: ', initial='0')
offset_box.on_submit(set_offset)

trgthrs_box = TextBox(plt.axes([0.07, 0.6, 0.04, 0.04]), 'Trigger: ', initial='2000')
trgthrs_box.on_submit(set_trgthrs)

nsample_box = TextBox(plt.axes([0.07, 0.5, 0.04, 0.04]), 'N sample: ', initial='7')
nsample_box.on_submit(set_Nsample)

time_hold_bs_box = TextBox(plt.axes([0.07, 0.4, 0.04, 0.04]), 'Time hold bl: ', initial='100')
time_hold_bs_box.on_submit(set_time_hold_bs)

time_integr_box = TextBox(plt.axes([0.07, 0.3, 0.04, 0.04]), 'Time integration: ', initial='76')
time_integr_box.on_submit(set_time_integr)

pre_integr_box = TextBox(plt.axes([0.07, 0.2, 0.04, 0.04]), 'Pre trigger: ', initial='5')
pre_integr_box.on_submit(set_pre_integr)


plt.show()

sdk.FreeBuffer("board0:/MMCComponents/Spectrum_0", obSpectrum)
