package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"magic", "data", "info"})
public class CPRawBuffer extends Structure implements Structure.ByReference {

    public int magic; // uint32
    public Pointer data; // uint32[]
    public CPRawBufferInfo info;

    public CPRawBuffer() {
        super();
    }

    public CPRawBuffer(Pointer p) {
        super(p);
    }

    public long GetMagic() {
        return Utils.SignedInteger2UnsignedLong(magic);
    }

    public long[] GetData() {
        return Utils.Pointer2UnsignedIntLongArray(data, info.GetBufferSize());
    }

    public CPRawBufferInfo GetInfo() {
        return info;
    }

}
