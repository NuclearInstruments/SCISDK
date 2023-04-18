package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"magic", "mag", "ph", "timecode", "info"})
public class FFTDecodedBuffer extends Structure implements Structure.ByReference {

    public int magic; // uint32
    public Pointer mag; // double[]
    public Pointer ph; // double[]
    public long timecode; // uint64
    public FFTDecodedBufferInfo info;

    public FFTDecodedBuffer() {
        super();
    }

    public FFTDecodedBuffer(Pointer p) {
        super(p);
    }
    
    public long GetMagic(){
        return Utils.SignedInteger2UnsignedLong(magic);
    }
    
    public double[] GetMag(){
        return Utils.Pointer2DoubleArray(mag, info.GetChannels() * info.GetSamples());
    }
    
    public double[] GetPh(){
        return Utils.Pointer2DoubleArray(ph, info.GetChannels() * info.GetSamples());
    }

}
