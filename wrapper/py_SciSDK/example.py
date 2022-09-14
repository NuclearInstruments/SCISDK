import os
from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import OscilloscopeDecodedBuffer

sdk = SciSDK()

# add new device
res = sdk.AddNewDevice("usb:13250","dt1260", "./oscilloscope.json","board0")
print("res =",res)
if res != 0:
    print ("Script exit due to connetion error")
    exit()
# res, ret_string = sdk.GetComponentList("board0", "all", True)
# print("res =",res)
# print(ret_string)

res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", 1325)
print("res =",res)

res, val = sdk.GetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level")
print("res =",res)
print("val =",val)

res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing","decode")
print("res =",res)

res, val = sdk.GetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing")
print("res =",res)
print("val =",val)

buf = OscilloscopeDecodedBuffer()
res = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", 1, buf)
print("res =",res)

sdk.DetachDevice("board0")

