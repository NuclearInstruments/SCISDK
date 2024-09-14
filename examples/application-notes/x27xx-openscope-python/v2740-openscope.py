
from scisdk.scisdk import SciSDK
from scisdk.scisdk_defines import *
import sys
import os
#import numpy as np
print('test imports true')
count = 10
sdk = SciSDK()
res = 0
res = sdk.AddNewDevice("192.168.102.153", "V2740", "../library/RegisterFile.json", "board0")
if res:
    print('Script exit due to connection error: ' + str(res))
    exit()
print("we made it to connection")
sdk.ExecuteCommand("board0:/boardapi/felib/cmd/reset", "")
sdk.SetParameterString("board0:/boardapi/readout.datatype", "scope")
sdk.SetParameterString("board0:/boardapi/data_processing","decode")
###
print("now out into param settings")
### parameter setting seems to work fine
sdk.SetParameterString("board0:/boardapi/felib/ch/0..63/par/ChEnable", "true")
print("enabled channels")
sdk.SetParameterString("board0:/boardapi/felib/par/RecordLengthS", "50")
sdk.SetParameterString("board0:/boardapi/felib/par/PreTriggerS", "4")
sdk.SetParameterString("board0:/boardapi/felib/par/AcqTriggerSource", "SwTrg | TestPulse")
sdk.SetParameterString("board0:/boardapi/felib/par/TestPulsePeriod", "100000")
sdk.SetParameterString("board0:/boardapi/felib/par/TestPulseWidth", "1000")
print("finished setting params")
### these two commands also cause seg faults

###
print("started acq")
# general digitizer buffer
ret, ddb = sdk.AllocateBuffer("board0:/boardapi",50)
print("going to read out data now...")
### the next segment of code may not be wholely correct yet, but if it arrives here than it is a close next step to saving out actual data.
sdk.ExecuteCommand("board0:/boardapi/felib/cmd/armacquisition", "")
sdk.ExecuteCommand("board0:/boardapi/felib/cmd/swstartacquisition", "")

while (count > 0):

    # Read data
    rep, ddb = sdk.ReadData("board0:/boardapi", ddb)
    if (rep == 0):
        count -= 1
        # Save data to a text file
        with open("example.txt", "w") as output:
            # header
            output.write("CH      time     data\n")
            # for i in range(ddb.n_channels):
            #     output.write(f"    CH{i:02d}")
            # output.write("\n")
            # data itself
            for j in range(ddb.n_channels):
                for i in range(ddb.n_samples[j]):
                    output.write(f"{j:8d}\t")
                    output.write(f"{i:8d}\t")
                    output.write(str(ddb.waveform[j][i]) + "  ")
                    output.write("\n")