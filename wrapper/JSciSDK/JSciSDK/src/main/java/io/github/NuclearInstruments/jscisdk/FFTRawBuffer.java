package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;
import java.math.BigInteger;

@Structure.FieldOrder({"magic", "data", "timecode", "info"})
public class FFTRawBuffer extends Structure implements Structure.ByReference {

    public int magic; // uint32
    public Pointer data; // uint32[]
    public long timecode; // uint64
    public FFTRawBufferInfo info;

    public FFTRawBuffer() {
        super();
    }

    public FFTRawBuffer(Pointer p) {
        super(p);
    }

    public long GetMagic() {
        return Utils.SignedInteger2UnsignedLong(magic);
    }

    public long[] GetData() {
        return Utils.Pointer2UnsignedIntLongArray(data, info.GetBufferSize());
    }

    public BigInteger GetTimecode() {
        return Utils.SignedLong2UnsignedBigInteger(timecode);
    }

}
