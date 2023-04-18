package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"buffer_size", "samples", "valid_samples", "channels"})
public class ListRawBufferInfo extends Structure {

    public int buffer_size; // uint32
    public int samples; // uint32
    public int valid_samples; // uint32
    public int channels; // uint32

    public ListRawBufferInfo() {
        super();
    }

    public ListRawBufferInfo(Pointer p) {
        super(p);
    }

    public long GetBufferSize() {
        return Utils.SignedInteger2UnsignedLong(buffer_size);
    }

    public long GetSamples() {
        return Utils.SignedInteger2UnsignedLong(samples);
    }

    public long GetValidSamples() {
        return Utils.SignedInteger2UnsignedLong(valid_samples);
    }

    public long GetChannels() {
        return Utils.SignedInteger2UnsignedLong(channels);
    }

}
