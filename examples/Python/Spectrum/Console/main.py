from time import sleep
from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *

sdk = SciSDK()

# add new device
res = sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0")

if not res == 0:
    print("Program exit due connection error")
    exit()

# set board parameters
sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.rebin", "0")
sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.limitmode", "freerun")
sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.limit", "100")

# execute command reset
sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "")

# execute command start
sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")

# sdk.SetRegister("board0:/Registers/spe", 100)
# sleep(4)

# # allocate buffer
# res, buf = sdk.AllocateBuffer("board0:/MMCComponents/Spectrum_0")

# if res == 0:
#     # read data
#     res, buf = sdk.ReadData("board0:/MMCComponents/Spectrum_0", buf)

#     if res == 0:
#         str_tmp = ""
#         for i in range(buf.info.total_bins):
#             str_tmp += str(buf.data[i]) + "\n"

#         # write data into a file
#         file = open("output.txt", "w")
#         file.write(str_tmp)
#         file.close()
#     else:
#         # print error description
#         res, err = sdk.s_error(res)
#         print("Read data error:", err)

#     sdk.FreeBuffer("board0:/MMCComponents/Spectrum_0", buf)

# else:
#     # print error description
#     res, err = sdk.s_error(res)
#     print("Allocate buffer error:", err)