import { Pointer, alloc, types, ref, refType } from 'ref-napi';
import { SciSDKInterface } from './SciSDKInterface';
import { OscilloscopeDecodedBuffer } from './SciSDKDefines';


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
        return SciSDKInterface.SCISDK_AddNewDevice(devicePath, deviceModel, JSONFwFilePath, name, this.handle);
    }

    DetachDevice(name: string): number {
        return SciSDKInterface.SCISDK_DetachDevice(name, this.handle);
    }

    SetParameterString(path: string, value: string): number {
        return SciSDKInterface.SCISDK_SetParameterString(path, value, this.handle);
    }

    SetParameterInteger(path: string, value: number): number {
        if (!Number.isInteger(value)) {
            throw new Error("Value is not an integer number");
        }
        return SciSDKInterface.SCISDK_SetParameterInteger(path, value, this.handle);
    }

    SetParameterUInteger(path: string, value: number): number {
        if (!Number.isInteger(value) || value < 0) {
            throw new Error("Value is not an unsigned integer number");
        }
        return SciSDKInterface.SCISDK_SetParameterUInteger(path, value, this.handle);
    }

    SetParameterDouble(path: string, value: number): number {
        return SciSDKInterface.SCISDK_SetParameterDouble(path, value, this.handle);
    }

    GetParameterString(path: string): NumberStringTuple {
        var ret_ptr: Pointer<any> = <Pointer<any>>alloc(types.CString);
        let res = SciSDKInterface.SCISDK_GetParameterString(path, ret_ptr, this.handle);
        return [res, ret_ptr.readPointer().readCString()];
    }

    GetParameterInteger(path: string): NumberNumberTuple {
        let ret_ptr: Pointer<number> = alloc(types.uint32);
        let res = SciSDKInterface.SCISDK_GetParameterInteger(path, ret_ptr, this.handle);
        return [res, ret_ptr.readUInt32LE()];
    }

    GetParameterUInteger(path: string): NumberNumberTuple {
        let ret_ptr: Pointer<number> = alloc(types.uint32);
        let res = SciSDKInterface.SCISDK_GetParameterUInteger(path, ret_ptr, this.handle);
        return [res, ret_ptr.readUInt32LE()];
    }

    GetParameterDouble(path: string): NumberNumberTuple {
        let ret_ptr: Pointer<number> = alloc(types.uint32);
        let res = SciSDKInterface.SCISDK_GetParameterDouble(path, ret_ptr, this.handle);
        return [res, ret_ptr.readUInt32LE()];
    }

    SetRegister(path: string, value: number): number {
        if (!Number.isInteger(value) || value < 0) {
            throw new Error("Value is not an unsigned integer number");
        }

        return SciSDKInterface.SCISDK_SetRegister(path, value, this.handle);
    }

    GetRegister(path: string): NumberNumberTuple {
        let ret_ptr: Pointer<number> = alloc(types.uint32);
        let res = SciSDKInterface.SCISDK_GetRegister(path, ret_ptr, this.handle);
        return [res, ret_ptr.readUInt32LE()];
    }

    AllocateBuffer(path: string): NumberObjectTuple {
        var tmp = new OscilloscopeDecodedBuffer;
        var test1 = alloc(OscilloscopeDecodedBuffer, tmp).ref();
        let res = SciSDKInterface.SCISDK_AllocateBuffer(path, 1, test1, this.handle);
        return [res, test1.deref().deref()];
    }

    ReadData(path: string, buffer: any) {
        let res = SciSDKInterface.SCISDK_ReadData(path, (<Pointer<typeof OscilloscopeDecodedBuffer>>buffer).ref(), this.handle);
        // console.log(res);.
        // buffer.analog
        for(let i=0;i<100;i++){
            console.log(buffer.analog[i])
        }
        console.log(buffer.trigger_position)
        return 0;
    }

    GetLibraryVersion(): NumberStringTuple {
        var ret_ptr: Pointer<any> = <Pointer<any>>alloc(types.CString);
        let res = SciSDKInterface.SCISDK_GetLibraryVersion(ret_ptr, this.handle);
        return [res, ret_ptr.readPointer().readCString()];
    }

}

interface NumberStringTuple extends Array<string | number> { 0: number; 1: string }
interface NumberNumberTuple extends Array<number> { 0: number; 1: number }
interface NumberObjectTuple extends Array<number | object> { 0: number; 1: object }