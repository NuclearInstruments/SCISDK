package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"magic", "analog", "digital", "timecode", "info"})
public class OscilloscopeDualDecodedBuffer extends Structure implements Structure.ByReference {

    public int magic;
    public Pointer analog;
    public Pointer digital;
    public long timecode;
    public OscilloscopeDecodedBufferInfo info;

    public long GetMagic() {
        return magic & 0xFFFFFFFFL;
    }

    public long[] GetAnalog() {
        return Utils.Pointer2LongArray(analog, info.GetSamplesAnalog() * info.GetChannels());
    }

    public int[] GetDigital() {
        Long digital_count = info.GetChannels() * info.GetSamplesDigital() * info.GetTracksDigitalPerChannel();
        byte[] digital_b_array = digital.getByteArray(0, digital_count.intValue());
        int[] res_array = new int[digital_b_array.length];
        for (int i = 0; i < digital_b_array.length; i++) {
            res_array[i] = digital_b_array[i] == 1 ? 1 : 0;
        }
        return res_array;
    }

    public long GetTimecode() {
        return timecode;
    }

    public OscilloscopeDecodedBufferInfo GetInfo() {
        return info;
    }

}
