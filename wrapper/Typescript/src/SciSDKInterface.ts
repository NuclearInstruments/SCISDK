import { Library } from "ffi-napi";


export const SciSDKInterface = Library(process.platform == "win32" ? "SciSDK_DLL.dll" : "SciSDK_DLL.so", {
    "SCISDK_InitLib": ['void*', []],
    "SCISDK_FreeLib": ['int', ['void*']],
    "SCISDK_AddNewDevice": ['int', ['string', 'string', 'string', 'string', 'void*']],
    "SCISDK_GetLibraryVersion": ['int', ['char**', 'void*']],
});
