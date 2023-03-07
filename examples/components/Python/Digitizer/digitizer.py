from random import randint
from unicodedata import decimal
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import os
from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *

fig = plt.figure("Digitizer analog data - channel 0")
ax1 = fig.add_subplot(1,1,1)

# initialize scisdk library
sdk = SciSDK()

#DT1260
res = sdk.AddNewDevice("usb:10500","dt1260", "./DT1260RegisterFile.json","board0")
#DT5560
#res = sdk.AddNewDevice("192.168.50.10:8888","DT5560", "./DT5560RegisterFile.json","board0")
#DT5550
#res = sdk.AddNewDevice("usb:11000","DT5550", "./DT5550RegisterFile.json","board0")
#V2740
#res = sdk.AddNewDevice("192.168.50.10","V2740", "./V2740RegisterFile.json","board0")

if res != 0:
    print ("Script exit due to connetion error")
    exit()

enabled_channels = 1
# set oscilloscope parameters
res = sdk.SetParameterString("board0:/MMCComponents/Digitizer_0.data_processing","decode")
res = sdk.SetParameterInteger("board0:/MMCComponents/Digitizer_0.enabledch", enabled_channels)
res = sdk.SetParameterInteger("board0:/MMCComponents/Digitizer_0.acq_len", 8000)
res = sdk.SetParameterString("board0:/MMCComponents/Digitizer_0.acq_mode", "blocking")
res = sdk.SetParameterInteger("board0:/MMCComponents/Digitizer_0.timeout", 2000)
res = sdk.ExecuteCommand("board0:/MMCComponents/Digitizer_0.start", "")
# allocate buffer for oscilloscope
res, buf = sdk.AllocateBuffer("board0:/MMCComponents/Digitizer_0")

def updateGraph(i, buffer): # function that provides to plot new data on graph
    res, buffer = sdk.ReadData("board0:/MMCComponents/Digitizer_0", buffer)# read data from board
    if res == 0:
        xar = []
        yar = []
        for index in range(buffer.info.valid_samples):
            xar.append(index)
            yar.append(buffer.analog[index])
        ax1.clear()
        ax1.plot(xar,yar)

# update graph every 50ms
ani = animation.FuncAnimation(fig, updateGraph, fargs=[buf],interval=200)
# updateGraph(None, buf, decimator)
plt.show()