import { Pointer, alloc, types } from 'ref-napi';
import { SciSDKInterface } from './SciSDKInterface';

export default class SciSDK {

    private handle: any = null;

    constructor() {
        // init library
        this.handle = SciSDKInterface.SCISDK_InitLib();
    }

    FreeLibrary(): number {
        return SciSDKInterface.SCISDK_FreeLib(this.handle);
    }

    AddNewDevice(devicePath: string, deviceModel: string, JSONFwFilePath: string, name: string): number {
        let device_path_buffer = Buffer.alloc(devicePath.length);
        device_path_buffer.fill(0);
        device_path_buffer.write(devicePath);
        return SciSDKInterface.SCISDK_AddNewDevice(deviceModel, deviceModel, JSONFwFilePath, name, this.handle);
    }

    DetachDevice(name: string): number {
        return 0;
    }

    GetLibraryVersion(): NumberStringTuple {
        var rer_ptr: Pointer<any> = <Pointer<any>>alloc(types.CString);
        let res = SciSDKInterface.SCISDK_GetLibraryVersion(rer_ptr, this.handle);
        return [res, rer_ptr.readPointer().readCString()];
    }

}

interface NumberStringTuple extends Array<string | number> { 0: number; 1: string }