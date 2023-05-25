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