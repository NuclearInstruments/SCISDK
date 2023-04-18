package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"samples", "valid_samples", "channels"})
public class DigitizerDecodedBufferInfo extends Structure {

    public int samples; // uint32
    public int valid_samples; // uint32
    public int channels; // channels
    
    public DigitizerDecodedBufferInfo(){
        super();
    }
    
    public DigitizerDecodedBufferInfo(Pointer p){
        super(p);
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
