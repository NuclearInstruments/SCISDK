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

res = sdk.SetParameterString("board0:/MMCComponents/CP_0.thread", "false")
res = sdk.SetParameterInteger("board0:/MMCComponents/CP_0.timeout", 500)
res = sdk.SetParameterString("board0:/MMCComponents/CP_0.acq_mode", "non-blocking")
res = sdk.SetParameterString("board0:/MMCComponents/CP_0.check_align_word", "check_align_word")
res = sdk.SetParameterString("board0:/MMCComponents/CP_0.data_processing", "decode")

# allocate buffer raw, size 1024
res, buf = sdk.AllocateBuffer("board0:/MMCComponents/CP_0", 100)

res = sdk.ExecuteCommand("board0:/MMCComponents/CP_0.start", "")

while True:
    res, buf = sdk.ReadData("board0:/MMCComponents/CP_0", buf)
    if res == 0:
        for i in range(0, int(buf.info.valid_data)):
            for n in range(0, buf.data[i].n):
                print(buf.data[i].row[n])
            print("header:      ", hex(buf.data[i].row[0]))
            print("timestamp:   ",buf.data[i].row[1] + (buf.data[i].row[2] << 32) )
            print("counter 0:   ",buf.data[i].row[3])
            print("counter 1:   ",buf.data[i].row[4])
            print("counter 2:   ",buf.data[i].row[5])
            print("counter 3:   ",buf.data[i].row[6])
            print("-----------------")
    
