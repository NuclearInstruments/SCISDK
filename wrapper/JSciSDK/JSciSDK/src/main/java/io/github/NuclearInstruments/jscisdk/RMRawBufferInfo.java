package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"buffer_size", "valid_data", "nchannels"})
public class RMRawBufferInfo extends Structure {

    public int buffer_size; // uint32
    public int valid_data; // uint32
    public int nchannels; // uint32

    public RMRawBufferInfo() {
        super();
    }

    public RMRawBufferInfo(Pointer p) {
        super(p);
    }
    
    public long GetBufferSize(){
        return Utils.SignedInteger2UnsignedLong(buffer_size);
    }
    
    public long GetValidData(){
        return Utils.SignedInteger2UnsignedLong(valid_data);
    }
    
    public long GetNChannels(){
        return Utils.SignedInteger2UnsignedLong(nchannels);
    }

}
