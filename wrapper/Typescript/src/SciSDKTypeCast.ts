import { types } from 'ref-napi';
const ref = require("ref-napi");

/* Method for number conversion */
export function PtrToInt32Array(ptr: any, size: number): Array<number> {
    let raw_data_arr = ptr.reinterpret(size * 4);
    let decoded_data_arr: Array<number> = [];
    for (let i = 0; i < raw_data_arr.length; i += 4) {
        decoded_data_arr.push((raw_data_arr[i + 3] << 24) +
            (raw_data_arr[i + 2] << 16) +
            (raw_data_arr[i + 1] << 8) +
            (raw_data_arr[i]));
    }
    return decoded_data_arr;
}

export function PtrToUInt32Array(ptr: any, size: number): Array<number> {
    let raw_data_arr = ptr.reinterpret(size * 4);
    let decoded_data_arr: Array<number> = [];
    for (let i = 0; i < raw_data_arr.length; i += 4) {
        decoded_data_arr.push(((raw_data_arr[i + 3] << 24) +
            (raw_data_arr[i + 2] << 16) +
            (raw_data_arr[i + 1] << 8) +
            (raw_data_arr[i])) >>> 0);
    }
    return decoded_data_arr;
}

export function PtrToInt8Array(ptr: any, size: number): Array<number> {
    let raw_data_arr = ptr.reinterpret(size);
    let decoded_data_arr: Array<number> = [];
    for (let i = 0; i < raw_data_arr.length; i += 2) {
        decoded_data_arr.push(raw_data_arr[i]);
    }
    return decoded_data_arr;
}

export function PtrToUInt8Array(ptr: any, size: number): Array<number> {
    let raw_data_arr = ptr.reinterpret(size);
    let decoded_data_arr: Array<number> = [];
    for (let i = 0; i < raw_data_arr.length; i += 2) {
        decoded_data_arr.push(raw_data_arr[i] >>> 0);
    }
    return decoded_data_arr;
}

export function PtrToDoubleArray(ptr: any, size: number): Array<number> {
    let result = ptr.reinterpret(size * types.double.size);
    let data:Array<number> = [];
    for (let i = 0; i < size; i++) {
        data.push(ref.get(result, i * types.double.size, types.double));
    }
    return data;
}