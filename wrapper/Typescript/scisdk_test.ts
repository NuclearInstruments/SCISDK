import SciSDK from "./src/SciSDK";

var sdk = new SciSDK();
var [status_code, str_value] = sdk.GetLibraryVersion();
console.log("SciSDK version: " + str_value);
status_code = sdk.AddNewDevice("usb:28645", "dt1260", "C:\\git\\scisdk\\examples\\components\\Java\\Oscilloscope\\DT1260RegisterFile.json", "board0");

if (!status_code) {
    sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode");
    sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", 1500);
    sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self");
    sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0);
    sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150);
    sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0);
    sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking");

    // allocate oscilloscope buffer
    var [status_code, buffer] = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0");
    sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", buffer);

    sdk.DetachDevice("board0");
} else {
    console.log("Cannot attach DT1260 board");
}

sdk.FreeLibrary();