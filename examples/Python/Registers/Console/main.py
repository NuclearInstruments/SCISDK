from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *

sdk = SciSDK()

# add new device
res = sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0")

if not res == 0:
    print("Program exit due connection error")
    exit()

# write register
res = sdk.SetParameterInteger("board0:/MMCComponents/CREG_1.register_2", 12)
if res == 0:
    print("Register value has been successully set")
else:
    print("Error while trying to write register value")

# read register
res, value = sdk.GetParameterInteger("board0:/MMCComponents/CREG_0.register_2")
if res == 0:
    print("Register value is", value)
else:
    print("Error while trying to read register value")

# read a multichannel register
print("Values from multichannel register:")
for i in range(6):
    res, value = sdk.GetParameterInteger("board0:/MMCComponents/REGFILE_0.register_" + str(i))
    print(value)

# detach device
sdk.DetachDevice("board0")