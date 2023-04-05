import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import Button
from matplotlib.widgets import CheckButtons
from matplotlib.widgets import TextBox
from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *

fig, ax = plt.subplots(4, 1, gridspec_kw={'height_ratios': [2, 1, 1, 1]})
ax[1].set_ylim(-0.1, 1.1)
ax[2].set_ylim(-0.1, 1.1)
ax[3].set_ylim(-0.1, 1.1)

# initialize scisdk library
sdk = SciSDK()

#DT1260
res = sdk.AddNewDevice("usb:13250", "DT1260", "RegisterFile.json", "board0")

if res != 0:
    print("Error adding device")
    exit()

# Set Parameters
polarity = 1
trigger_threshold = 2000
Nsample = 7
time_hold_bs = 100
time_integr = 76
pre_integr = 5
gain = 30000
Offset_int = 0

# Set register values
res = sdk.SetRegister("board0:/Registers/pol", polarity)
res = sdk.SetRegister("board0:/Registers/trgthrs", trigger_threshold)
res = sdk.SetRegister("board0:/Registers/Nsample", Nsample)
res = sdk.SetRegister("board0:/Registers/time_hold_bs", time_hold_bs)
res = sdk.SetRegister("board0:/Registers/time_integr", time_integr)
res = sdk.SetRegister("board0:/Registers/pre_integr", pre_integr)
res = sdk.SetRegister("board0:/Registers/gain", gain)
res = sdk.SetRegister("board0:/Registers/Offset_int", Offset_int)

# Set oscilloscope parameters
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing","decode")
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode","ext")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150)
decimator = 1
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", decimator)
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.timeout", 3000)

# Allocate buffer for oscilloscope
res, buf = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0")

global channel0, channel1, channel2, channel3
channel0 = 0
channel1 = 0
channel2 = 0
channel3 = 0

# Plot oscilloscope data
def updateGraph(i, buffer, decimator):
    res, buffer = sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", buffer)
    if res == 0:
        x, y0, y1, y2, y3 = [], [], [], [], []
        y11, y12, y21, y31, y32, y33 = [], [], [], [], [], []

        offset_channel_dig = buffer.info.samples_digital * buffer.info.tracks_digital_per_channel
        offset_channel_an = buffer.info.samples_analog        

        for index in range(buffer.info.samples_analog):
            x.append(index * decimator)
            y0.append(buffer.analog[index])
            y1.append(buffer.analog[index + offset_channel_an * 1])
            y2.append(buffer.analog[index + offset_channel_an * 2])
            y3.append(buffer.analog[index + offset_channel_an * 3])
            y11.append(buffer.digital[index + offset_channel_dig * 1])
            y12.append(buffer.digital[index + offset_channel_dig * 1 + buffer.info.samples_digital])
            y21.append(buffer.digital[index + offset_channel_dig * 2])
            y31.append(buffer.digital[index + offset_channel_dig * 3])
            y32.append(buffer.digital[index + offset_channel_dig * 3 + buffer.info.samples_digital])
            y33.append(buffer.digital[index + offset_channel_dig * 3 + buffer.info.samples_digital*2])

        ax[0].clear()
        ax[1].clear()
        ax[2].clear()
        ax[3].clear()

        if channel0 == 1:
            ax[0].plot(x, y0, label="Signal", color="blue")
        if channel1 == 1:
            ax[0].plot(x, y1, label="Baseline", color="orange")
            ax[1].plot(x, y11, label="Valid BL", color="orange")
            ax[2].plot(x, y12, label="Run/hold BL", color="orange")
        if channel2 == 1:
            ax[0].plot(x, y2, label="Triggered signal", color="green")
            ax[1].plot(x, y21, label="Trigger", color="green")
        if channel3 == 1:
            ax[0].plot(x, y3, label="Charge", color="red")
            ax[1].plot(x, y31, label="Valid Charge", color="red")
            ax[2].plot(x, y32, label="Time integr", color="red")
            ax[3].plot(x, y33, label="Pileup", color="red")
        
        ax[0].legend(bbox_to_anchor=(1.01, 1), loc='upper left', borderaxespad=0.)
        ax[1].legend(bbox_to_anchor=(1.01, 1), loc='upper left', borderaxespad=0.)
        ax[2].legend(bbox_to_anchor=(1.01, 1), loc='upper left', borderaxespad=0.)
        ax[3].legend(bbox_to_anchor=(1.01, 1), loc='upper left', borderaxespad=0.)

ani = animation.FuncAnimation(fig, updateGraph, fargs=[buf, decimator,],interval=100)

def stop(event):
    ani.event_source.stop()
    sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.stop", "")


def start(event):
    ani.event_source.start()
    sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.start", "")


stop_button = Button(plt.axes([0.7, 0.9, 0.1, 0.08]), 'Stop')
stop_button.on_clicked(stop)

start_button = Button(plt.axes([0.8, 0.9, 0.1, 0.08]), 'Start')
start_button.on_clicked(start)

labels = ["Signal", "Baseline", "Triggered signal", "Charge"]

check = CheckButtons(plt.axes([0.2, 0.9, 0.4, 0.08]), labels, [False, False, False, False])


def func(label):
    index = labels.index(label)
    global channel0, channel1, channel2, channel3
    if check.get_status()[index]:
        if index == 0:
            channel0 = 1
        if index == 1:
            channel1 = 1
        if index == 2:
            channel2 = 1
        if index == 3:
            channel3 = 1
    else:
        if index == 0:
            channel0 = 0
        if index == 1:
            channel1 = 0
        if index == 2:
            channel2 = 0
        if index == 3:
            channel3 = 0

check.on_clicked(func)

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

gain_box = TextBox(plt.axes([0.05, 0.8, 0.04, 0.04]), 'Gain: ', initial='30000')
gain_box.on_submit(set_gain)

offset_box = TextBox(plt.axes([0.05, 0.7, 0.04, 0.04]), 'Offset: ', initial='0')
offset_box.on_submit(set_offset)

trgthrs_box = TextBox(plt.axes([0.05, 0.6, 0.04, 0.04]), 'Trigger: ', initial='2000')
trgthrs_box.on_submit(set_trgthrs)

nsample_box = TextBox(plt.axes([0.05, 0.5, 0.04, 0.04]), 'N sample: ', initial='7')
nsample_box.on_submit(set_Nsample)

time_hold_bs_box = TextBox(plt.axes([0.05, 0.4, 0.04, 0.04]), 'Time hold \n baseline: ', initial='100')
time_hold_bs_box.on_submit(set_time_hold_bs)

time_integr_box = TextBox(plt.axes([0.05, 0.3, 0.04, 0.04]), 'Time \n integration: ', initial='76')
time_integr_box.on_submit(set_time_integr)

pre_integr_box = TextBox(plt.axes([0.05, 0.2, 0.04, 0.04]), 'Pre trigger: ', initial='5')
pre_integr_box.on_submit(set_pre_integr)

plt.show()
