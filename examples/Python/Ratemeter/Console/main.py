import os
from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *

sdk = SciSDK()

# add new device
res = sdk.AddNewDevice("usb:13250","dt1260", "./RegisterFile.json","board0")

if res != 0:
    print(sdk.s_error(res))
    print ("Script exit due to connetion error")
    exit()


# allocate buffer (sciSDK detects automatically buffer type)
res, buf = sdk.AllocateBuffer("board0:/MMCComponents/RateMeter_0")
if res == 0:
    # read data
    res, buf = sdk.ReadData("board0:/MMCComponents/RateMeter_0", buf)

    # print data
    for i in range(0, buf.info.nchannels):
        print("Ratemeter channer",i,buf.data[i])

    res = sdk.FreeBuffer("board0:/MMCComponents/RateMeter_0", 1, buf)
    sdk.DetachDevice("board0")
    res = sdk.FreeLib()