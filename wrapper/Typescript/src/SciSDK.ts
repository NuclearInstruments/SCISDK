import { Pointer, alloc, types, ref, refType } from 'ref-napi';
import { SciSDKInterface } from './SciSDKInterface';
import { SciSDKCustomPacketDecodedBuffer, SciSDKDigitizerDecodedBuffer, SciSDKDigitizerRawBuffer, SciSDKListRawBuffer, SciSDKOscilloscopeDecodedBuffer, SciSDKOscilloscopeDualDecodedBuffer, SciSDKOscilloscopeDualRawBuffer, SciSDKOscilloscopeRawBuffer, SciSDKRateMeterRawBuffer } from './SciSDKDefines';


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

    AllocateBuffer(path: string, buffer: any): number {
        let decoded = 0;
        if (buffer instanceof SciSDKOscilloscopeDecodedBuffer) {
            decoded = 1;
            buffer.cpointer = alloc(SciSDKOscilloscopeDecodedBuffer.cpointer_class, new SciSDKOscilloscopeDecodedBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKOscilloscopeRawBuffer) {
            decoded = 0;
            buffer.cpointer = alloc(SciSDKOscilloscopeRawBuffer.cpointer_class, new SciSDKOscilloscopeRawBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKOscilloscopeDualDecodedBuffer) {
            decoded = 1;
            buffer.cpointer = alloc(SciSDKOscilloscopeDualDecodedBuffer.cpointer_class, new SciSDKOscilloscopeDualDecodedBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKOscilloscopeDualRawBuffer) {
            decoded = 0;
            buffer.cpointer = alloc(SciSDKOscilloscopeDualRawBuffer.cpointer_class, new SciSDKOscilloscopeDualRawBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKDigitizerDecodedBuffer) {
            decoded = 1;
            buffer.cpointer = alloc(SciSDKDigitizerDecodedBuffer.cpointer_class, new SciSDKDigitizerDecodedBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKDigitizerRawBuffer) {
            decoded = 0;
            buffer.cpointer = alloc(SciSDKDigitizerRawBuffer.cpointer_class, new SciSDKDigitizerRawBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKCustomPacketDecodedBuffer) {
            decoded = 1;
            buffer.cpointer = alloc(SciSDKCustomPacketDecodedBuffer.cpointer_class, new SciSDKCustomPacketDecodedBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKListRawBuffer) {
            decoded = 0;
            buffer.cpointer = alloc(SciSDKListRawBuffer.cpointer_class, new SciSDKListRawBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKRateMeterRawBuffer) {
            decoded = 0;
            buffer.cpointer = alloc(SciSDKRateMeterRawBuffer.cpointer_class, new SciSDKRateMeterRawBuffer.cpointer_class).ref();
        }
        else {
            throw new Error("The type of the buffer is invalid");
        }

        let res = SciSDKInterface.SCISDK_AllocateBuffer(path, decoded, buffer.cpointer, this.handle);
        buffer.cpointer = buffer.cpointer.deref().deref()
        buffer.LoadData();
        return res;
    }

    AllocateBufferSize(path: string, buffer: any, size: number): number {
        let decoded = 0;
        if (buffer instanceof SciSDKOscilloscopeDecodedBuffer) {
            decoded = 1;
            buffer.cpointer = alloc(SciSDKOscilloscopeDecodedBuffer.cpointer_class, new SciSDKOscilloscopeDecodedBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKOscilloscopeRawBuffer) {
            decoded = 0;
            buffer.cpointer = alloc(SciSDKOscilloscopeRawBuffer.cpointer_class, new SciSDKOscilloscopeRawBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKOscilloscopeDualDecodedBuffer) {
            decoded = 1;
            buffer.cpointer = alloc(SciSDKOscilloscopeDualDecodedBuffer.cpointer_class, new SciSDKOscilloscopeDualDecodedBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKOscilloscopeDualRawBuffer) {
            decoded = 0;
            buffer.cpointer = alloc(SciSDKOscilloscopeDualRawBuffer.cpointer_class, new SciSDKOscilloscopeDualRawBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKDigitizerDecodedBuffer) {
            decoded = 1;
            buffer.cpointer = alloc(SciSDKDigitizerDecodedBuffer.cpointer_class, new SciSDKDigitizerDecodedBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKDigitizerRawBuffer) {
            decoded = 0;
            buffer.cpointer = alloc(SciSDKDigitizerRawBuffer.cpointer_class, new SciSDKDigitizerRawBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKListRawBuffer) {
            decoded = 0;
            buffer.cpointer = alloc(SciSDKListRawBuffer.cpointer_class, new SciSDKListRawBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKCustomPacketDecodedBuffer) {
            decoded = 1;
            buffer.cpointer = alloc(SciSDKCustomPacketDecodedBuffer.cpointer_class, new SciSDKCustomPacketDecodedBuffer.cpointer_class).ref();
        }
        else if (buffer instanceof SciSDKRateMeterRawBuffer) {
            decoded = 0;
            buffer.cpointer = alloc(SciSDKRateMeterRawBuffer.cpointer_class, new SciSDKRateMeterRawBuffer.cpointer_class).ref();
        }
        else {
            throw new Error("The type of the buffer is invalid");
        }

        let res = SciSDKInterface.SCISDK_AllocateBufferSize(path, decoded, buffer.cpointer, this.handle, size);
        buffer.cpointer = buffer.cpointer.deref().deref()
        buffer.LoadData();
        return res;
    }

    ReadData(path: string, buffer: any) {
        let res = -1;
        if (buffer instanceof SciSDKOscilloscopeDecodedBuffer) {
            res = SciSDKInterface.SCISDK_ReadData(path, (<Pointer<typeof SciSDKOscilloscopeDecodedBuffer.cpointer_class>>buffer.cpointer).ref(), this.handle);
        }
        else if (buffer instanceof SciSDKOscilloscopeRawBuffer) {
            res = SciSDKInterface.SCISDK_ReadData(path, (<Pointer<typeof SciSDKOscilloscopeRawBuffer.cpointer_class>>buffer.cpointer).ref(), this.handle);
        }
        else if (buffer instanceof SciSDKOscilloscopeDualDecodedBuffer) {
            res = SciSDKInterface.SCISDK_ReadData(path, (<Pointer<typeof SciSDKOscilloscopeDualDecodedBuffer.cpointer_class>>buffer.cpointer).ref(), this.handle);
        }
        else if (buffer instanceof SciSDKOscilloscopeDualRawBuffer) {
            res = SciSDKInterface.SCISDK_ReadData(path, (<Pointer<typeof SciSDKOscilloscopeDualRawBuffer.cpointer_class>>buffer.cpointer).ref(), this.handle);
        }
        else if (buffer instanceof SciSDKDigitizerDecodedBuffer) {
            res = SciSDKInterface.SCISDK_ReadData(path, (<Pointer<typeof SciSDKDigitizerDecodedBuffer.cpointer_class>>buffer.cpointer).ref(), this.handle);
        }
        else if (buffer instanceof SciSDKDigitizerRawBuffer) {
            res = SciSDKInterface.SCISDK_ReadData(path, (<Pointer<typeof SciSDKDigitizerRawBuffer.cpointer_class>>buffer.cpointer).ref(), this.handle);
        }
        else if (buffer instanceof SciSDKListRawBuffer) {
            res = SciSDKInterface.SCISDK_ReadData(path, (<Pointer<typeof SciSDKListRawBuffer.cpointer_class>>buffer.cpointer).ref(), this.handle);
        }
        else if (buffer instanceof SciSDKCustomPacketDecodedBuffer) {
            res = SciSDKInterface.SCISDK_ReadData(path, (<Pointer<typeof SciSDKCustomPacketDecodedBuffer.cpointer_class>>buffer.cpointer).ref(), this.handle);
        }
        else if (buffer instanceof SciSDKRateMeterRawBuffer) {
            res = SciSDKInterface.SCISDK_ReadData(path, (<Pointer<typeof SciSDKRateMeterRawBuffer.cpointer_class>>buffer.cpointer).ref(), this.handle);
        }
        else {
            throw new Error("The type of the buffer is invalid");
        }

        buffer.LoadData();
        return res;
    }

    ExecuteCommand(path: string, value: string): number {
        return SciSDKInterface.SCISDK_ExecuteCommand(path, value, this.handle);
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