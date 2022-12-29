from random import randint
from unicodedata import decimal
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import os
from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *

fig = plt.figure("Time of Flight Spectrum Data")
ax1 = fig.add_subplot(1,1,1)

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

if not res == 0:
    print("Program exit due connection error")
    exit()

# configure firmware register
sdk.SetRegister("board0:/Registers/delay", 300)

# set board parameters
ret = sdk.SetParameterInteger("board0:/MMCComponents/TOF_0.binwidth", 3)
sdk.SetParameterString("board0:/MMCComponents/TOF_0.start_delay", "0")
sdk.SetParameterString("board0:/MMCComponents/TOF_0.limitmode", "freerun")
sdk.SetParameterString("board0:/MMCComponents/TOF_0.limit", "100")

# execute command reset
sdk.ExecuteCommand("board0:/MMCComponents/TOF_0.reset", "")

# execute command start
sdk.ExecuteCommand("board0:/MMCComponents/TOF_0.start", "")


# allocate buffer
res, buf = sdk.AllocateBuffer("board0:/MMCComponents/TOF_0")


def updateGraph(i, buffer): # function that provides to plot new data on graph
    res, buffer = sdk.ReadData("board0:/MMCComponents/TOF_0", buffer)# read data from board
    if res == 0:
        xar = []
        yar = []
        for index in range(buffer.info.valid_bins):
            xar.append(index)
            yar.append(buffer.data[index])
        ax1.clear()
        ax1.plot(xar,yar)

# update graph every 50ms
ani = animation.FuncAnimation(fig, updateGraph, fargs=[buf,],interval=100)
# updateGraph(None, buf, decimator)
plt.show()
