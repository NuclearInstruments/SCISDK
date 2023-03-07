from time import sleep
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

if not res == 0:
    print("Program exit due connection error")
    exit()

sdk.SetRegister("board0:/Registers/delay", 300)

# set board parameters
ret = sdk.SetParameterInteger("board0:/MMCComponents/TOF_0.binwidth", 3)
sdk.SetParameterString("board0:/MMCComponents/TOF_0.start_delay", "0")
sdk.SetParameterString("board0:/MMCComponents/TOF_0.limitmode", "freerun")
sdk.SetParameterString("board0:/MMCComponents/TOF_0.limit", "100")

# execute command reset
sdk.ExecuteCommand("board0:/MMCComponents/TOF_0.reset", "")

# execute command start
sdk.ExecuteCommand("board0:/MMCComponents/TOF_0.start", "")



for i in range(10):
    print ("Wait " + str(i) + "s/10s ...")
    sleep(1)

# allocate buffer
res, buf = sdk.AllocateBuffer("board0:/MMCComponents/TOF_0")

if res == 0:
    # read data
    res, buf = sdk.ReadData("board0:/MMCComponents/TOF_0", buf)

    if res == 0:
        str_tmp = ""
        for i in range(buf.info.total_bins):
            str_tmp += str(buf.data[i]) + "\n"

        # write data into a file
        file = open("output.txt", "w")
        file.write(str_tmp)
        file.close()
    else:
        # print error description
        res, err = sdk.s_error(res)
        print("Read data error:", err)

    sdk.FreeBuffer("board0:/MMCComponents/TOF_0", buf)

else:
    # print error description
    res, err = sdk.s_error(res)
    print("Allocate buffer error:", err)