package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"samples", "channels"})
public class FFTDecodedBufferInfo extends Structure {

    public int samples; // uint32
    public int channels; // uint32

    public FFTDecodedBufferInfo() {
        super();
    }

    public FFTDecodedBufferInfo(Pointer p) {
        super(p);
    }

    public long GetSamples() {
        return Utils.SignedInteger2UnsignedLong(samples);
    }

    public long GetChannels() {
        return Utils.SignedInteger2UnsignedLong(channels);
    }

}
