package com.nuclearinstruments.jscisdk;

import com.sun.jna.Structure;

@Structure.FieldOrder({"samples", "valid_samples", "channels"})
public class DigitizerDecodedBufferInfo extends Structure {

    public int samples;
    public int valid_samples;
    public int channels;

    public long GetSamples() {
        return samples & 0xFFFFFFFFL;
    }

    public long GetValidSamples() {
        return valid_samples & 0xFFFFFFFFL;
    }

    public long GetChannels() {
        return channels & 0xFFFFFFFFL;
    }
}
