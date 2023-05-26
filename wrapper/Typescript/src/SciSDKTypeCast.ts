import { types } from 'ref-napi';
const ref = require("ref-napi");

/* Method for number conversion */
export function PtrToInt32Array(ptr: any, size: number): Array<number> {
    let raw_data_arr = ptr.reinterpret(size * types.int32.size);
    let decoded_data_arr: Array<number> = [];
    for (let i = 0; i < size; i++) {
        decoded_data_arr.push(ref.get(raw_data_arr, i * types.int32.size, types.int32));
    }
    return decoded_data_arr;
}

export function PtrToUInt32Array(ptr: any, size: number): Array<number> {
    let raw_data_arr = ptr.reinterpret(size * types.uint32.size);
    let decoded_data_arr: Array<number> = [];
    for (let i = 0; i < size; i++) {
        decoded_data_arr.push(ref.get(raw_data_arr, i * types.uint32.size, types.uint32));
    }
    return decoded_data_arr;
}

export function PtrToInt8Array(ptr: any, size: number): Array<number> {
    let raw_data_arr = ptr.reinterpret(size * types.int8.size);
    let decoded_data_arr: Array<number> = [];
    for (let i = 0; i < size; i++) {
        decoded_data_arr.push(ref.get(raw_data_arr, i * types.int8.size, types.int8));
    }
    return decoded_data_arr;
}

export function PtrToUInt8Array(ptr: any, size: number): Array<number> {
    let raw_data_arr = ptr.reinterpret(size * types.uint8.size);
    let decoded_data_arr: Array<number> = [];
    for (let i = 0; i < size; i++) {
        decoded_data_arr.push(ref.get(raw_data_arr, i * types.uint8.size, types.uint8));
    }
    return decoded_data_arr;
}

export function PtrToDoubleArray(ptr: any, size: number): Array<number> {
    let result = ptr.reinterpret(size * types.double.size);
    let data: Array<number> = [];
    for (let i = 0; i < size; i++) {
        data.push(ref.get(result, i * types.double.size, types.double));
    }
    return data;
}