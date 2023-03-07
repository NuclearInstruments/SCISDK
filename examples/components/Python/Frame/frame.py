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

# set oscilloscope parameters

res = sdk.SetParameterString("board0:/MMCComponents/ImageReadout_0.thread", "false")
res = sdk.SetParameterInteger("board0:/MMCComponents/ImageReadout_0.timeout", 500)
res = sdk.SetParameterString("board0:/MMCComponents/ImageReadout_0.acq_mode", "non-blocking")
res = sdk.SetParameterString("board0:/MMCComponents/ImageReadout_0.trigger_mode", "trig")
res = sdk.SetParameterString("board0:/MMCComponents/ImageReadout_0.data_processing", "decode")

# allocate buffer raw, size 1024
res, buf = sdk.AllocateBuffer("board0:/MMCComponents/ImageReadout_0", 100)

res = sdk.ExecuteCommand("board0:/MMCComponents/ImageReadout_0.start", "")

while True:
    res, buf = sdk.ReadData("board0:/MMCComponents/ImageReadout_0", buf)
    if res == 0:
        for i in range(0, int(buf.info.valid_data)):
            print(f'{buf.data[i].pixel[0]:<8}', f'{buf.data[i].pixel[1]:<8}', f'{buf.data[i].pixel[2]:<8}')
            print(f'{buf.data[i].pixel[3]:<8}', f'{buf.data[i].pixel[4]:<8}', f'{buf.data[i].pixel[5]:<8}')    
            print(f'{buf.data[i].pixel[6]:<8}', f'{buf.data[i].pixel[7]:<8}', f'{buf.data[i].pixel[8]:<8}')
            print("")
    
