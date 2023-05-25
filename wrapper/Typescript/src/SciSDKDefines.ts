import { refType, types } from 'ref-napi';
import { PtrToDoubleArray, PtrToInt32Array, PtrToInt8Array, PtrToUInt32Array, PtrToUInt8Array } from './SciSDKTypeCast';
const ref = require("ref-napi");
const Struct = require("ref-struct-di")(ref);

/**
 * !!! HOW TO ADD A NEW BUFFER TYPE !!!
 * Create here the class that must contain:
 * - static attribute called 'cpointer_class' that is a node ffi api Struct that contains the list of the native structure attributes' names and types
 * - attribute  called 'cpointer' that is the instance of the native buffer structure
 * - method called 'LoadData' used to convert the native structure into typescript/javascript's object
 * 
 * Also you need to add the option for the new buffer type inside 'AllocateBuffer', 'AllocateBufferSize', 'ReadData' and 'FreeBuffer' methods inside SciSDK class
 */

export class SciSDKOscilloscopeDecodedBuffer {
    magic: number = 0;
    analog: Array<number> = [];
    digital: Array<number> = [];
    trigger_position: number = 0;
    timecode: number = 0;
    info = {
        samples_analog: 0,
        samples_digital: 0,
        tracks_analog_per_channel: 0,
        tracks_digital_per_channel: 0,
        channels: 0,
    };
    static cpointer_class = Struct({
        magic: types.uint32,
        analog: refType('uint32 *'),
        digital: refType('uint8 *'),
        trigger_position: types.uint32,
        timecode: types.uint64,
        // info
        samples_analog: types.uint32,
        samples_digital: types.uint32,
        tracks_analog_per_channel: types.uint32,
        tracks_digital_per_channel: types.uint32,
        channels: types.uint32,
    });
    cpointer: any = null;
    // function used to load data the native structure object
    LoadData = () => {
        this.magic = this.cpointer.magic;
        this.analog = PtrToInt32Array(this.cpointer.analog, this.cpointer.samples_analog * this.cpointer.tracks_analog_per_channel * this.cpointer.channels);
        this.digital = PtrToUInt8Array(this.cpointer.digital, this.cpointer.samples_digital * this.cpointer.tracks_digital_per_channel * this.cpointer.channels);
        this.trigger_position = this.cpointer.trigger_position;
        this.timecode = this.cpointer.timecode;
        this.info.samples_analog = this.cpointer.samples_analog;
        this.info.samples_digital = this.cpointer.samples_digital;
        this.info.tracks_analog_per_channel = this.cpointer.tracks_analog_per_channel;
        this.info.tracks_digital_per_channel = this.cpointer.tracks_digital_per_channel;
        this.info.channels = this.cpointer.channels;
    }
}

export class SciSDKOscilloscopeRawBuffer {
    magic: number = 0;
    data: Array<number> = [];
    zero_position: number = 0;
    trigger_position: number = 0;
    timecode: number = 0;
    info = {
        buffer_size: 0,
        samples_analog: 0,
        samples_digital: 0,
        tracks_analog_per_channel: 0,
        tracks_digital_per_channel: 0,
        channels: 0,
    }

    static cpointer_class = Struct({
        magic: types.uint32,
        data: refType('uint32 *'),
        zero_position: types.uint32,
        trigger_position: types.uint32,
        timecode: types.uint64,
        // info
        buffer_size: types.uint32,
        samples_analog: types.uint32,
        samples_digital: types.uint32,
        tracks_analog_per_channel: types.uint32,
        tracks_digital_per_channel: types.uint32,
        channels: types.uint32,

    });
    cpointer: any = null;
    LoadData = () => {
        this.magic = this.cpointer.magic;
        this.data = PtrToUInt32Array(this.cpointer.data, this.cpointer.buffer_size);
        this.zero_position = this.cpointer.zero_position;
        this.trigger_position = this.cpointer.trigger_position;
        this.timecode = this.cpointer.timecode;
        this.info.buffer_size = this.cpointer.buffer_size;
        this.info.samples_analog = this.cpointer.samples_analog;
        this.info.samples_digital = this.cpointer.samples_digital;
        this.info.tracks_analog_per_channel = this.cpointer.tracks_analog_per_channel;
        this.info.tracks_digital_per_channel = this.cpointer.tracks_digital_per_channel;
        this.info.channels = this.cpointer.channels;
    }
}


export class SciSDKOscilloscopeDualDecodedBuffer {
    magic: number = 0;
    analog: Array<number> = [];
    digital: Array<number> = [];
    trigger_position: number = 0;
    timecode: number = 0;
    info = {
        samples_analog: 0,
        samples_digital: 0,
        tracks_analog_per_channel: 0,
        tracks_digital_per_channel: 0,
        channels: 0,
    };
    static cpointer_class = Struct({
        magic: types.uint32,
        analog: refType('uint32 *'),
        digital: refType('uint8 *'),
        trigger_position: types.uint32,
        timecode: types.uint64,
        // info
        samples_analog: types.uint32,
        samples_digital: types.uint32,
        tracks_analog_per_channel: types.uint32,
        tracks_digital_per_channel: types.uint32,
        channels: types.uint32,
    });
    cpointer: any = null;
    // function used to load data the native structure object
    LoadData = () => {
        this.magic = this.cpointer.magic;
        this.analog = PtrToInt32Array(this.cpointer.analog, this.cpointer.samples_analog * this.cpointer.tracks_analog_per_channel * this.cpointer.channels);
        this.digital = PtrToUInt8Array(this.cpointer.digital, this.cpointer.samples_digital * this.cpointer.tracks_digital_per_channel * this.cpointer.channels);
        this.trigger_position = this.cpointer.trigger_position;
        this.timecode = this.cpointer.timecode;
        this.info.samples_analog = this.cpointer.samples_analog;
        this.info.samples_digital = this.cpointer.samples_digital;
        this.info.tracks_analog_per_channel = this.cpointer.tracks_analog_per_channel;
        this.info.tracks_digital_per_channel = this.cpointer.tracks_digital_per_channel;
        this.info.channels = this.cpointer.channels;
    }
}

export class SciSDKOscilloscopeDualRawBuffer {
    magic: number = 0;
    data: Array<number> = [];
    zero_position: number = 0;
    trigger_position: number = 0;
    timecode: number = 0;
    info = {
        buffer_size: 0,
        samples_analog: 0,
        samples_digital: 0,
        tracks_analog_per_channel: 0,
        tracks_digital_per_channel: 0,
        channels: 0,
    }

    static cpointer_class = Struct({
        magic: types.uint32,
        data: refType('uint32 *'),
        zero_position: types.uint32,
        trigger_position: types.uint32,
        timecode: types.uint64,
        // info
        buffer_size: types.uint32,
        samples_analog: types.uint32,
        samples_digital: types.uint32,
        tracks_analog_per_channel: types.uint32,
        tracks_digital_per_channel: types.uint32,
        channels: types.uint32,

    });
    cpointer: any = null;
    LoadData = () => {
        this.magic = this.cpointer.magic;
        this.data = PtrToUInt32Array(this.cpointer.data, this.cpointer.buffer_size);
        this.zero_position = this.cpointer.zero_position;
        this.trigger_position = this.cpointer.trigger_position;
        this.timecode = this.cpointer.timecode;
        this.info.buffer_size = this.cpointer.buffer_size;
        this.info.samples_analog = this.cpointer.samples_analog;
        this.info.samples_digital = this.cpointer.samples_digital;
        this.info.tracks_analog_per_channel = this.cpointer.tracks_analog_per_channel;
        this.info.tracks_digital_per_channel = this.cpointer.tracks_digital_per_channel;
        this.info.channels = this.cpointer.channels;
    }
}

export class SciSDKDigitizerDecodedBuffer {

    magic: number = 0;
    analog: Array<number> = [];
    hits: number = 0;
    timecode: number = 0;
    counter: number = 0;
    user: number = 0;
    info = {
        samples: 0,
        valid_samples: 0,
        channels: 0,
        enabled_channels: 0,
    }

    static cpointer_class = Struct({
        magic: types.uint32,
        analog: refType('int32 *'),
        hits: types.uint64,
        timecode: types.uint64,
        counter: types.uint32,
        user: types.uint32,
        // info
        samples: types.uint32,
        valid_samples: types.uint32,
        channels: types.uint32,
        enabled_channels: types.uint32,
    });
    cpointer: any = null;
    LoadData = () => {
        this.magic = this.cpointer.magic;
        this.analog = PtrToInt32Array(this.cpointer.analog, this.cpointer.channels * this.cpointer.samples);
        this.hits = this.cpointer.hits;
        this.timecode = this.cpointer.timecode;
        this.counter = this.cpointer.counter;
        this.user = this.cpointer.user;
        this.info.samples = this.cpointer.samples;
        this.info.valid_samples = this.cpointer.valid_samples;
        this.info.channels = this.cpointer.channels;
        this.info.enabled_channels = this.cpointer.enabled_channels;
    }
}

export class SciSDKDigitizerRawBuffer {

    magic: number = 0;
    data: Array<number> = [];
    info = {
        buffer_size: 0,
        samples: 0,
        valid_samples: 0,
        channels: 0
    };

    static cpointer_class = Struct({
        magic: types.uint32,
        data: refType('int32 *'),
        // info
        buffer_size: types.uint32,
        samples: types.uint32,
        valid_samples: types.uint32,
        channels: types.uint32,
    });
    cpointer: any = null;
    LoadData = () => {
        this.magic = this.cpointer.magic;
        this.data = PtrToInt32Array(this.cpointer.data, this.cpointer.buffer_size);
        this.info.buffer_size = this.cpointer.buffer_size;
        this.info.samples = this.cpointer.samples;
        this.info.valid_samples = this.cpointer.valid_samples;
        this.info.channels = this.cpointer.channels;
    }
}

export class SciSDKListRawBuffer {

    magic: number = 0;
    data: Array<number> = [];
    info = {
        buffer_size: 0,
        samples: 0,
        valid_samples: 0,
        channels: 0,
    }

    static cpointer_class = Struct({
        magic: types.uint32,
        data: refType('char *'),
        // info
        buffer_size: types.uint32,
        samples: types.uint32,
        valid_samples: types.uint32,
        channels: types.uint32,
    });
    cpointer: any = null;
    LoadData = () => {
        this.magic = this.cpointer.magic;
        this.data = PtrToInt8Array(this.cpointer.data, this.cpointer.buffer_size);
        this.info.buffer_size = this.cpointer.buffer_size;
        this.info.samples = this.cpointer.samples;
        this.info.valid_samples = this.cpointer.valid_samples;
        this.info.channels = this.cpointer.channels;
    }
}

export class SciSDKCustomPacket {
    row: Array<number> = [];
    n: number = 0;

    static cpointer_class = Struct({
        row: refType("uint32 *"),
        n: types.uint32
    });
    static LoadFromCPointer(cpointer: any): SciSDKCustomPacket {
        let packet = new SciSDKCustomPacket();
        packet.row = PtrToUInt32Array(cpointer.row, cpointer.n);
        packet.n = cpointer.n;
        return packet;
    }
}

export class SciSDKCustomPacketDecodedBuffer {
    magic: number = 0;
    data: Array<SciSDKCustomPacket> = [];
    info = {
        buffer_size: 0,
        packet_size: 0,
        valid_data: 0
    }

    static cpointer_class = Struct({
        magic: types.uint32,
        data: refType(SciSDKCustomPacket.cpointer_class), // TODO: decode to scisdk cp packet
        buffer_size: types.uint32,
        packet_size: types.uint32,
        valid_data: types.uint32,
    });
    cpointer: any = null;
    LoadData = () => {
        this.magic = this.cpointer.magic;
        let result = this.cpointer.data.reinterpret(this.cpointer.buffer_size * SciSDKCustomPacket.cpointer_class.size);
        for (let i = 0; i < this.cpointer.buffer_size; i++) {
            this.data[i] = SciSDKCustomPacket.LoadFromCPointer(ref.get(result, i * SciSDKCustomPacket.cpointer_class.size, SciSDKCustomPacket.cpointer_class));
        }
        this.info.buffer_size = this.cpointer.buffer_size;
        this.info.packet_size = this.cpointer.packet_size;
        this.info.valid_data = this.cpointer.valid_data;
    }
}

export class SciSDKCustomPacketRawBuffer {

    magic: number = 0;
    data: Array<number> = [];
    info = {
        buffer_size: 0,
        valid_data: 0
    }

    static cpointer_class = Struct({
        magic: types.uint32,
        data: refType("uint32 *"),
        // info
        buffer_size: types.uint32,
        valid_data: types.uint32,
    });
    cpointer: any = null;
    LoadData = () => {
        this.magic = this.cpointer.magic;
        this.data = PtrToUInt32Array(this.cpointer.data, this.cpointer.buffer_size);
        this.info.buffer_size = this.cpointer.buffer_size;
        this.info.valid_data = this.cpointer.valid_data;
    }
}

export class SciSDKRateMeterRawBuffer {

    magic: number = 0;
    data: Array<number> = [];
    info = {
        buffer_size: 0,
        valid_data: 0,
        nchannels: 0
    }

    static cpointer_class = Struct({
        magic: types.uint32,
        data: refType("double *"),
        // info
        buffer_size: types.uint32,
        valid_data: types.uint32,
        nchannels: types.uint32,
    });
    cpointer: any = null;
    LoadData = () => {
        this.magic = this.cpointer.magic;
        this.data = PtrToDoubleArray(this.cpointer.data, this.cpointer.buffer_size);
        this.info.buffer_size = this.cpointer.buffer_size;
        this.info.valid_data = this.cpointer.valid_data;
        this.info.nchannels = this.cpointer.nchannels;
    }
}

export class SciSDKSpectrumDecodedBuffer {

    magic: number = 0;
    data: Array<number> = [];
    timecode: number = 0;
    inttime: number = 0;
    info = {
        buffer_size: 0,
        total_bins: 0,
        valid_bins: 0,
    }

    static cpointer_class = Struct({
        magic: types.uint32,
        data: refType("uint32 *"),
        timecode: types.uint64,
        inttime: types.uint32,
        // info
        buffer_size: types.uint32,
        total_bins: types.uint32,
        valid_bins: types.uint32,
    });
    cpointer: any = null;
    LoadData = () => {
        this.magic = this.cpointer.magic;
        this.data = PtrToUInt32Array(this.cpointer.data, this.cpointer.buffer_size);
        this.timecode = this.cpointer.timecode;
        this.inttime = this.cpointer.inttime;
        this.info.buffer_size = this.cpointer.buffer_size;
        this.info.total_bins = this.cpointer.total_bins;
        this.info.valid_bins = this.cpointer.valid_bins;
    }
}

export class SciSDKFFTDecodedBuffer {

    magic: number = 0;
    mag: Array<number> = [];
    ph: Array<number> = [];
    timecode: number = 0;
    info = {
        samples: 0,
        channels: 0
    }

    static cpointer_class = Struct({
        magic: types.uint32,
        mag: refType('double *'),
        ph: refType('double *'),
        timecode: types.uint64,
        // info
        samples: types.uint32,
        channels: types.uint32,
    });
    cpointer: any = null;
    LoadData = () => {
        this.magic = this.cpointer.magic;
        this.mag = PtrToDoubleArray(this.cpointer.mag, this.cpointer.samples * this.cpointer.channels);
        this.ph = PtrToDoubleArray(this.cpointer.mag, this.cpointer.samples * this.cpointer.channels);
        this.timecode = this.cpointer.timecode;
        this.info.channels = this.cpointer.channels;
        this.info.samples = this.cpointer.samples;
    }
}

export class SciSDKFFTRawBuffer {
    magic: number = 0;
    data: Array<number> = [];
    timecode: number = 0;
    info = {
        buffer_size: 0,
        samples: 0,
        channels: 0,
    }

    static cpointer_class = Struct({
        magic: types.uint32,
        data: refType('uint32 *'),
        timecode: types.uint64,
        // info
        buffer_size: types.uint32,
        samples: types.uint32,
        channels: types.uint32,
    });
    cpointer: any = null;
    LoadData = () => {
        this.magic = this.cpointer.magic;
        this.data = PtrToUInt32Array(this.cpointer.data, this.cpointer.buffer_size);
        this.timecode = this.cpointer.timecode;
        this.info.buffer_size = this.cpointer.buffer_size;
        this.info.samples = this.cpointer.samples;
        this.info.channels = this.cpointer.channels;
    }
}

export class SciSDKFrameDecodedBuffer {
    static cpointer_class = Struct({
        magic: types.uint32,
        data: refType('void *'),// TODO: cast to SCISDK_FRAME_PACKET
        // info
        buffer_size: types.uint32,
        valid_data: types.uint32,
    });
    cpointer: any = null;
    LoadData = () => {

    }
}

export class SciSDKFrameRawBuffer {
    static cpointer_class = Struct({
        magic: types.uint32,
        data: refType('uint32 *'),
        // info
        buffer_size: types.uint32,
        packet_size: types.uint32,
        valid_data: types.uint32,
    });
    cpointer: any = null;
    LoadData = () => {

    }
}