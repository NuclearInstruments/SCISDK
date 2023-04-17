package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"magic", "data", "info"})
public class RMRawBuffer extends Structure implements Structure.ByReference {

    public int magic; // uint32
    public Pointer data; // double[]
    public RMRawBufferInfo info;

    public RMRawBuffer() {
        super();
    }

    public RMRawBuffer(Pointer p) {
        super(p);
    }

    public long GetMagic() {
        return Utils.SignedInteger2UnsignedLong(magic);
    }

    public double[] GetData() {
        return Utils.Pointer2DoubleArray(data, info.GetBufferSize());
    }

    public RMRawBufferInfo GetInfo() {
        return info;
    }
}
