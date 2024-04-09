from random import randint
from unicodedata import decimal
import matplotlib.pyplot as plt
import os
from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *



sdk = SciSDK()

print("SDK version: ", sdk.GetLibraryVersion())
#DT5560
res = sdk.AddNewDevice("130.246.55.76:8888","R5560", "RegisterFile.json","board0")


if not res == 0:
    print("Program exit due connection error")
    exit()

res = sdk.SetParameterString("board0:/MMCComponents/Hist2D_0.limitmode", "freerun")
res = sdk.SetParameterString("board0:/MMCComponents/Hist2D_0.limit", "100")

# execute command reset
#sdk.ExecuteCommand("board0:/MMCComponents/Hist2D_0.reset", "")

# execute command start
#sdk.ExecuteCommand("board0:/MMCComponents/Hist2D_0.start", "")


# allocate buffer
res, buf = sdk.AllocateBuffer("board0:/MMCComponents/Hist2D_0")



res, buffer = sdk.ReadData("board0:/MMCComponents/Hist2D_0", buf)# read data from board
S = [0 for i in range(buffer.info.binsY)] 
for i in range(buffer.info.binsY):
    S[i] = [0 for j in range(buffer.info.binsX)]
if res == 0:
    for i in range(buffer.info.binsY):
        for j in range(buffer.info.binsX):
            S[i][j] = buffer.data[i*buffer.info.binsX+j]

#plot heatmap
plt.imshow(S, cmap='jet', interpolation='nearest')
plt.show()

