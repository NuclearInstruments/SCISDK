import { refType, types } from 'ref-napi';
import { StructType } from 'ref-struct-di';

const ref = require("ref-napi");
const Struct = require("ref-struct-di")(ref);
const ArrayType = require("ref-array-di")(ref);

var int = ref.types.int;
export const IntArray = ArrayType(int);

export const OscilloscopeDecodedBuffer = Struct({
    magic: types.uint32,
    analog: IntArray,
    digital: refType('int8*'),
    trigger_position: types.uint32,
    timecode: types.uint64,
    samples_analog: types.uint32,
    samples_digital: types.uint32,
    tracks_analog_per_channel: types.uint32,
    tracks_digital_per_channel: types.uint32,
    channels: types.uint32,
});