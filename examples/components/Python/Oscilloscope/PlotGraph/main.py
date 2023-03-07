from random import randint
from unicodedata import decimal
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import os
from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *

fig = plt.figure("Oscilloscope analog data - channel 0")
ax1 = fig.add_subplot(1,1,1)

# initialize scisdk library
sdk = SciSDK()
# add new device

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


# set oscilloscope parameters
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing","decode")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", 30000)
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode","self")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0)
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150)
decimator = 1
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", decimator)
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.timeout", 3000)
# allocate buffer for oscilloscope
res, buf = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0")

def updateGraph(i, buffer, decimator): # function that provides to plot new data on graph
    res, buffer = sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", buffer)# read data from board
    if res == 0:
        xar = []
        yar = []
        for index in range(buffer.info.samples_analog):
            xar.append(index * decimator)
            yar.append(buffer.analog[index])
        ax1.clear()
        ax1.plot(xar,yar)

# update graph every 50ms
ani = animation.FuncAnimation(fig, updateGraph, fargs=[buf, decimator,],interval=100)
# updateGraph(None, buf, decimator)
plt.show()