from unicodedata import decimal
import os
from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *
from struct import *


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

res = sdk.SetParameterString("board0:/MMCComponents/List_0.thread", "false")
res = sdk.SetParameterInteger("board0:/MMCComponents/List_0.timeout", 500)
res = sdk.SetParameterString("board0:/MMCComponents/List_0.acq_mode", "blocking")

# allocate buffer raw, size 1024
res, buf = sdk.AllocateBuffer("board0:/MMCComponents/List_0", 1024)

res = sdk.ExecuteCommand("board0:/MMCComponents/List_0.stop", "")

res = sdk.ExecuteCommand("board0:/MMCComponents/List_0.start", "")

while True:
    res, buf = sdk.ReadData("board0:/MMCComponents/List_0", buf)
    if res == 0:
        for i in range(0, int(buf.info.valid_samples/8)):
            print(unpack('<hhL', buf.data[i*8:(i+1)*8]))
            #print (buf.data[i])