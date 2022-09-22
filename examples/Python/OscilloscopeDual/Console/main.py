from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *

sdk = SciSDK()

# add new device
res = sdk.AddNewDevice("usb:13250" ,"dt1260", "RegisterFile.json", "board0")
if res != 0:
    print ("Script exit due to connetion error")
    exit()

# set board parameters
sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self")
sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", 3000)
sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0)
sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150)
sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0)
sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode")
# sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "raw")
sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking")
sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.timeout", 1000)

# allocate buffer (automatically detects buffer type)
res, buf = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0")

if res == 0:
    sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", "")
    # read data
    res, buf = sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", buf)

    # read channels and store data in one file for each channel
    for i in range(buf.info.channels):
        str_tmp = ""
        for j in range(buf.info.samples_analog):
            str_tmp += str(buf.analog[j + i * buf.info.samples_analog]) + "\n"

        file = open("analog"+str(i)+".txt", "w")
        file.write(str_tmp)
        file.close()

    sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", buf)

else:
    print(sdk.s_error(res))

# detach device
sdk.DetachDevice("board0")