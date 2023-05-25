import SciSDK from "./src/SciSDK";
import { SciSDKCustomPacketDecodedBuffer, SciSDKListRawBuffer, SciSDKOscilloscopeDecodedBuffer, SciSDKOscilloscopeRawBuffer, SciSDKRateMeterRawBuffer } from "./src/SciSDKDefines";

var sdk = new SciSDK();
var [status_code, str_value] = sdk.GetLibraryVersion();
console.log("SciSDK version: " + str_value);
status_code = sdk.AddNewDevice("usb:28645", "dt1260", "C:\\git\\scisdk\\examples\\components\\Java\\Ratemeter\\DT1260RegisterFile.json", "board0");

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

    // sdk.SetParameterString("board0:/MMCComponents/List_0.thread", "false");
    // sdk.SetParameterInteger("board0:/MMCComponents/List_0.timeout", 500);
    // sdk.SetParameterString("board0:/MMCComponents/List_0.acq_mode", "blocking");

    // sdk.ExecuteCommand("board0:/MMCComponents/List_0.stop", "");
    // sdk.ExecuteCommand("board0:/MMCComponents/List_0.start", "");

    // var buf = new SciSDKListRawBuffer();
    // status_code = sdk.AllocateBufferSize("board0:/MMCComponents/List_0", buf, 1024);
    // if (!status_code) {
    //     status_code = sdk.ReadData("board0:/MMCComponents/List_0", buf);
    //     if (!status_code) {
    //         for (let i = 0; i < buf.data.length / 8; i++) {
    //             var chunk = buf.data.slice(i*8, (i+1)*8);
    //             var ch1 = chunk[0] + (chunk[1] << 8);
    //             var ch0 = (chunk[2] << 8) + (chunk[3] << 0);
    //             var counter = chunk[4] + (chunk[5] << 8) + (chunk[6] << 16) + (chunk[7] << 24);
    //             console.log(ch1 + ", " + ch0 + ", " + counter)
    //         }
    //         console.log("Length " + buf.data.length)
    //         console.log("Valid samples " + buf.info.buffer_size)
    //     } else {
    //         console.log("Read data " + status_code)
    //     }
    // } else {
    //     console.log("Allocate buffer " + status_code)
    // }

    // sdk.DetachDevice("board0");
    // sdk.FreeLibrary();

    // let res = sdk.SetParameterString("board0:/MMCComponents/CP_0.thread", "false");
    // res = sdk.SetParameterInteger("board0:/MMCComponents/CP_0.timeout", 500);
    // res = sdk.SetParameterString("board0:/MMCComponents/CP_0.acq_mode", "non-blocking");
    // res = sdk.SetParameterString("board0:/MMCComponents/CP_0.check_align_word", "check_align_word");
    // res = sdk.SetParameterString("board0:/MMCComponents/CP_0.data_processing", "decode");

    // sdk.ExecuteCommand("board0:/MMCComponents/CP_0.start", "")
    // setTimeout(() => {

    //     var buf = new SciSDKCustomPacketDecodedBuffer();

    //     res = sdk.AllocateBufferSize("board0:/MMCComponents/CP_0", buf, 100);

    //     if (res == 0) {
    //         res = sdk.ReadData("board0:/MMCComponents/CP_0", buf);
    //         if (res == 0) {
    //             for (let i = 0; i < buf.info.valid_data; i++) {
    //                 const element =  buf.data[i];
    //                 console.log(element.row[0].toString(16))
    //             }
    //         }
    //         else {
    //             console.log("Cannot read data " + res)
    //         }
    //     }
    //     else {
    //         console.log("Cannot allocate buffer " + res)
    //     }
    // }, 500);

    var buf = new SciSDKRateMeterRawBuffer();
    if (sdk.AllocateBuffer("board0:/MMCComponents/RateMeter_0", buf) == 0) {
        let res= sdk.ReadData("board0:/MMCComponents/RateMeter_0", buf)
        if (res == 0) {
            for (let i = 0; i < buf.data.length; i++) {
                const element = buf.data[i];
                console.log(element)
            }
        } else {
            console.log("Cannot read data " + res)
        }
    } else {
        console.log("Cannot allocate buffer")
    }

} else {
    console.log("Cannot attach DT1260 board");
}
console.log("end")