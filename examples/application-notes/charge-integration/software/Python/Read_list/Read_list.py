from scisdk.scisdk import SciSDK
from struct import unpack
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


# ## LIST
# Allocate buffer raw, size 1024
buffer_info = sdk.AllocateBuffer("board0:/MMCComponents/List_0", 1024)
if buffer_info is None:
    print("Error allocating buffer")
    exit(-1)
else:
    res, buf = buffer_info

# Set parameter of the list
res = sdk.SetParameterString("board0:/MMCComponents/List_0.thread", "false")
res = sdk.SetParameterString("board0:/MMCComponents/List_0.acq_mode",
                             "blocking")
res = sdk.SetParameterInteger("board0:/MMCComponents/List_0.timeout", 500)

if res != 0:
    print("Error allocating list buffer")
    exit(-1)
    
res = sdk.ExecuteCommand("board0:/MMCComponents/List_0.start", "")

# Read the data buffered
res, buf = sdk.ReadData("board0:/MMCComponents/List_0", buf)

# Print an array of values
if res == 0:
    print(buf.info.valid_samples)
    data_to_write = ""
    for i in range(0, int(buf.info.valid_samples/4)):
        data_to_write += str(i) + "\t" + str(unpack('<I', buf.data[i*4:(i+1)*4])[0]) + "\n"
        # print(data_to_write)
    print("Data read")
    file = open("./list.txt", "w")
    file.write(data_to_write)
    file.close()
    print("Data saved in list.txt")
if res != 0:
    res, err = sdk.s_error(res)
    print("Read data error:", err)
