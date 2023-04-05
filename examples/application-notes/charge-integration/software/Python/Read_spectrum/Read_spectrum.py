from scisdk.scisdk import SciSDK
from time import sleep

# Initialize sdk library
sdk = SciSDK()

# Add the DT1260 device to the sdk
res = sdk.AddNewDevice("usb:13250", "DT1260", "RegisterFile.json", "board0")

if res != 0:
    print("Error adding device")
    exit()

# Set register values
res = sdk.SetRegister("board0:/Registers/pol", 1)
res = sdk.SetRegister("board0:/Registers/trgthrs", 2000)
res = sdk.SetRegister("board0:/Registers/Nsample", 7)
res = sdk.SetRegister("board0:/Registers/time_hold_bs", 100)
res = sdk.SetRegister("board0:/Registers/time_integr", 76)
res = sdk.SetRegister("board0:/Registers/pre_integr", 5)
res = sdk.SetRegister("board0:/Registers/gain", 30000)
res = sdk.SetRegister("board0:/Registers/Offset_int", 0)

# ## SPECTRUM

# Set spectrum parameters
sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.rebin", "0")
sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.limitmode", "freerun")
print("Spectrum parameters set")

# Execute command reset
sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "")

# Execute command start
sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "")

# Waiting for data
for i in range(50):
    sleep(1)
    print("Waiting for data " + str(i) + " s")

# Allocate buffer
res, obSpectrum = sdk.AllocateBuffer("board0:/MMCComponents/Spectrum_0")

if res != 0:
    res, err = sdk.s_error(res)
    print("Allocate spectrum buffer error:", err)
    exit(-1)

# Read data
res, obSpectrum = sdk.ReadData("board0:/MMCComponents/Spectrum_0",
                               obSpectrum)

if res == 0:
    str_to_write = ""
    for i in range(obSpectrum.info.total_bins):
        str_to_write += str(i) + "\t" + str(obSpectrum.data[i]) + "\n"

    # Write data into a file
    file = open("./spectrum_read.txt", "w")
    file.write(str_to_write)
    file.close()
else:
    res, err = sdk.s_error(res)
    print("Read data error:", err)

sdk.FreeBuffer("board0:/MMCComponents/Spectrum_0", obSpectrum)
