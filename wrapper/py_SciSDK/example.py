import os
from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *

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

# res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", 1325)
# print("res =",res)

# res, val = sdk.GetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level")
# print("res =",res)
# print("val =",val)

res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing","decode")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level",1500)
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode","self")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0)
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150)
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0)
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking")

# print("res =",res)

res, val = sdk.GetParameterString("board0:/MMCComponents/Oscilloscope_0.buffer_type")
print("res =",res)
print("val =",val)

buf = OscilloscopeDecodedBuffer()

res, buf = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0")
print("res =",res)

# print("command res =", sdk.ExecuteCommmand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", ""))

res, buf = sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", buf)
print("res =",res)

# print(sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", 1, buf))

res, params_str = sdk.GetParameterDescription("board0:/MMCComponents/Oscilloscope_0.acq_mode")
print(params_str)

for i in range(0,buf.info.samples_analog):
    print(buf.analog[i])

res = sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", 1, buf)
# print(res)

sdk.DetachDevice("board0")
res = sdk.FreeLib()
# print(res)