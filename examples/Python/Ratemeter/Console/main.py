import os
from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *

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
    print(sdk.s_error(res))
    print ("Script exit due to connetion error")
    exit()


# allocate buffer (sciSDK detects automatically buffer type)
res, buf = sdk.AllocateBuffer("board0:/MMCComponents/RateMeter_0")
if res == 0:
    # read data
    res, buf = sdk.ReadData("board0:/MMCComponents/RateMeter_0", buf)

    print("-------------------")
    # print data
    for i in range(0, buf.info.nchannels):
        print("Ratemeter channer",i,buf.data[i])

    res = sdk.FreeBuffer("board0:/MMCComponents/RateMeter_0", buf)
    sdk.DetachDevice("board0")
    res = sdk.FreeLib()