import os
from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *

sdk = SciSDK()

# add new device
res = sdk.AddNewDevice("usb:13250","dt1260", "./oscilloscope.json","board0")

if res != 0:
    print(sdk.s_error(res))
    print ("Script exit due to connetion error")
    exit()

# set oscilloscope parameters on the board
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing","decode")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level",1500)
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode","self")
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0)
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150)
res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0)
res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking")

# allocate buffer (sciSDK detects automatically buffer type)
res, buf = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0")
if res == 0:
    res = sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", "")
    # read data
    res, buf = sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", buf)
    
    if res == 0:
        # store analog data inside a file
        analog_str = ""
        for i in range(0,buf.info.samples_analog):
            analog_str = analog_str + str(buf.analog[i]) + "\n"
        analog_file = open("analog.txt", "w")
        analog_file.write(analog_str)
        analog_file.close()

        # store digital data inside a file
        digital_str = ""
        # read digital data from channel 1
        for i in range(0,buf.info.samples_digital):
            digital_str = digital_str + str(buf.digital[i]) + "\n"
        digital_file = open("digital1.txt", "w")
        digital_file.write(digital_str)
        digital_file.close()

        digital_str = ""
        # read digital data from channel 2
        for i in range(buf.info.samples_digital,buf.info.samples_digital*2):
            digital_str = digital_str + str(buf.digital[i]) + "\n"
        digital_file = open("digital2.txt", "w")
        digital_file.write(digital_str)
        digital_file.close()

        digital_str = ""
        # read digital data from channel 3
        for i in range(buf.info.samples_digital*2,buf.info.samples_digital*3):
            digital_str = digital_str + str(buf.digital[i]) + "\n"
        digital_file = open("digital3.txt", "w")
        digital_file.write(digital_str)
        digital_file.close()

        digital_str = ""
        # read digital data from channel 4
        for i in range(buf.info.samples_digital*3,buf.info.samples_digital*4):
            digital_str = digital_str + str(buf.digital[i]) + "\n"
        digital_file = open("digital4.txt", "w")
        digital_file.write(digital_str)
        digital_file.close()

    res = sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", 1, buf)
    sdk.DetachDevice("board0")
    res = sdk.FreeLib()