package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;
import com.sun.jna.Structure.FieldOrder;

@FieldOrder({"magic", "data", "timecode", "inttime", "info"})
public class SpectrumDecodedBuffer extends Structure implements Structure.ByReference {

    public int magic;
    public Pointer data;
    public long timecode;
    public int inttime;
    public SpectrumDecodedBufferInfo info;

    public SpectrumDecodedBuffer() {
    }

    public SpectrumDecodedBuffer(Pointer p) {
        super(p);
    }

    public long GetMagic() {
        return Utils.SignedInteger2UnsignedLong(magic);
    }

    public long[] GetData() {
        return Utils.Pointer2UnsignedIntLongArray(data, GetInfo().GetValidBins());
    }

    public long GetIntTime() {
        return Utils.SignedInteger2UnsignedLong(inttime);
    }

    public SpectrumDecodedBufferInfo GetInfo() {
        return info;
    }

}
