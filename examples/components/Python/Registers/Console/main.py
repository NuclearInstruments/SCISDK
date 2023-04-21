from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *

sdk = SciSDK()

#DT1260
res = sdk.AddNewDevice("usb:28645","dt1260", "./DT1260RegisterFile.json","board0")
#DT5560
#res = sdk.AddNewDevice("192.168.50.10:8888","DT5560", "./DT5560RegisterFile.json","board0")
#DT5550
#res = sdk.AddNewDevice("usb:11000","DT5550", "./DT5550RegisterFile.json","board0")
#V2740
#res = sdk.AddNewDevice("192.168.50.10","V2740", "./V2740RegisterFile.json","board0")


if not res == 0:
    print("Program exit due connection error")
    exit()

regA=100
regB=151
regC = 0
sdk.SetRegister("board0:/Registers/A", regA)
sdk.SetRegister("board0:/Registers/B", regB)
err, regC = sdk.GetRegister("board0:/Registers/C")
print("Register C = (A+B) value is ", regC)

# write and readback channel registers (from 0 to 5)
for i in range(6):
    res = sdk.SetParameterInteger("board0:/MMCComponents/CREG_0.CH" + str(i), i+10)
    if res == 0:
        print("Register CREG_0.CH" + str(i) + " value has been successully set to " + str(i+10) + "")
    else:
        print("Error while trying to write register value")

for i in range(6):
    res, value = sdk.GetParameterInteger("board0:/MMCComponents/CREG_1.CH" + str(i))
    if res == 0:
        print("Register CREG_0.CH" + str(i) + " value is", value)
    else:
        print("Error while trying to read register value")

# set register file REG_0...3 and readback 5..6th registers
res = sdk.SetParameterInteger("board0:/MMCComponents/REGFILE_0.REG_0", 10)
res = sdk.SetParameterInteger("board0:/MMCComponents/REGFILE_0.REG_1", 20)
res = sdk.SetParameterInteger("board0:/MMCComponents/REGFILE_0.REG_2", 50)
res = sdk.SetParameterInteger("board0:/MMCComponents/REGFILE_0.REG_3", 5)

for i in range(2):
    res, value = sdk.GetParameterInteger("board0:/MMCComponents/REGFILE_0.REG_" + str(i+4))
    if res == 0:
        print("Register REGFILE_0.REG_" + str(i+5) + " value is", value)
    else:
        print("Error while trying to read register value")

# detach device
sdk.DetachDevice("board0")