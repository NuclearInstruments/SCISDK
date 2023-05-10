package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"magic", "data", "info"})
public class FrameDecodedBuffer extends Structure implements Structure.ByReference {

    public int magic;
    public long data;
    public FrameDecodedBufferInfo info;

    public FrameDecodedBuffer() {
        super();
    }

    public FrameDecodedBuffer(Pointer p) {
        super(p);
    }

    public long GetMagic() {
        return Utils.SignedInteger2UnsignedLong(magic);
    }

    public FramePacket[] GetData() {
        return Utils.Pointer2FramePacketArray(data, info.GetValidData());
    }

    public FrameDecodedBufferInfo GetInfo() {
        return info;
    }
}
