import SciSDK from "./src/SciSDK";
import { SciSDKListRawBuffer, SciSDKOscilloscopeDecodedBuffer, SciSDKOscilloscopeRawBuffer } from "./src/SciSDKDefines";

var sdk = new SciSDK();
var [status_code, str_value] = sdk.GetLibraryVersion();
console.log("SciSDK version: " + str_value);
status_code = sdk.AddNewDevice("usb:28645", "dt1260", "C:\\git\\scisdk\\examples\\components\\Java\\List\\DT1260RegisterFile.json", "board0");

if (!status_code) {
    // sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "raw");
    // sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", 1500);
    // sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self");
    // sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0);
    // sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150);
    // sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0);
    // sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking");

    // // allocate oscilloscope buffer
    // var buffer = new SciSDKOscilloscopeRawBuffer();
    // status_code = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", buffer);

    // sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", buffer);

    // // for(let i=0; i<buffer.data.length;i++){
    // //     console.log(buffer.data[i])
    // // }

    sdk.SetParameterString("board0:/MMCComponents/List_0.thread", "false");
    sdk.SetParameterInteger("board0:/MMCComponents/List_0.timeout", 500);
    sdk.SetParameterString("board0:/MMCComponents/List_0.acq_mode", "blocking");

    sdk.ExecuteCommand("board0:/MMCComponents/List_0.stop", "");
    sdk.ExecuteCommand("board0:/MMCComponents/List_0.start", "");

    setTimeout(() => {
        var buf = new SciSDKListRawBuffer();
        status_code = sdk.AllocateBufferSize("board0:/MMCComponents/List_0", buf, 1024);
        if (!status_code) {
            status_code = sdk.ReadData("board0:/MMCComponents/List_0", buf);
            if (!status_code) {
                for (let i = 0; i < buf.info.valid_samples/8; i++) {
                    // var chunk = buf.data.slice(i*8, (i+1)*8);
                    // var ch1 = chunk[0] + (chunk[1] << 8);
                    // var ch0 = (chunk[2] << 8) + (chunk[3] << 0);
                    // var counter = chunk[4] + (chunk[5] << 8) + (chunk[6] << 16) + (chunk[7] << 24);
                    // console.log(ch1 + ", " + ch0 + ", " + counter)
                    // console.log(chunk)
                    console.log((i+1)*8)
                }
                
                console.log("Valid samples " + buf.info.valid_samples/8)
            } else {
                console.log("Read data " + status_code)
            }
        } else {
            console.log("Allocate buffer " + status_code)
        }

        sdk.DetachDevice("board0");
        sdk.FreeLibrary();
    }, 2000);


} else {
    console.log("Cannot attach DT1260 board");
}
