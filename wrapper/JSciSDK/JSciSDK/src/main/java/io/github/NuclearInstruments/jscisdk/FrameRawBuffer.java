package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"magic", "data", "info"})
public class FrameRawBuffer extends Structure implements Structure.ByReference {

    public int magic;
    public Pointer data;
    public FrameRawBufferInfo info;

    public FrameRawBuffer() {
        super();
    }

    public FrameRawBuffer(Pointer p) {
        super(p);
    }

    public long GetMagic() {
        return Utils.SignedInteger2UnsignedLong(magic);
    }

    public long[] GetData() {
        return Utils.Pointer2UnsignedIntLongArray(data, info.GetValidData());
    }

    public FrameRawBufferInfo GetInfo() {
        return info;
    }
}
