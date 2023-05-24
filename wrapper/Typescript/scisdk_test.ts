import SciSDK from "./src/SciSDK";

var sdk = new SciSDK();
var [status_code, value] = sdk.GetLibraryVersion();
console.log("SciSDK version: " + value);
status_code = sdk.AddNewDevice("usb:28645", "dt1260", "C:\\git\\scisdk\\examples\\components\\Java\\Registers\\DT1260RegisterFile.json", "board0");
console.log(status_code);


sdk.FreeLibrary();