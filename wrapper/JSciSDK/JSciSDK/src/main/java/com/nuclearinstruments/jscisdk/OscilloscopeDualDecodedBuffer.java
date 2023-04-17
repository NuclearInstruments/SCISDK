package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;
import java.math.BigInteger;

@Structure.FieldOrder({"magic", "analog", "digital", "timecode", "info"})
public class OscilloscopeDualDecodedBuffer extends Structure implements Structure.ByReference {

    public int magic; // uint32
    public Pointer analog; //int32[]
    public Pointer digital; // int[]
    public long timecode; // uint64
    public OscilloscopeDecodedBufferInfo info;

    public OscilloscopeDualDecodedBuffer() {
        super();
    }

    public OscilloscopeDualDecodedBuffer(Pointer p) {
        super(p);
    }

    public long GetMagic() {
        return Utils.SignedInteger2UnsignedLong(magic);
    }

    public int[] GetAnalog() {
        return Utils.Pointer2SignedIntArray(analog, info.GetSamplesAnalog() * info.GetChannels());
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

    public BigInteger GetTimecode() {
        return Utils.SignedLong2UnsignedBigInteger(timecode);
    }

    public OscilloscopeDecodedBufferInfo GetInfo() {
        return info;
    }

}
