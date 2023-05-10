package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"pixel", "n", "info"})
public class FramePacket extends Structure implements Structure.ByReference {

    public Pointer pixel;
    public int n;
    public FramePacketInfo info;

    public FramePacket() {
        super();
    }

    public FramePacket(Pointer p) {
        super(p);
        read();
    }

    public int[] GetPixel() {
        return pixel.getIntArray(0, n);
    }

    public long GetN() {
        return Utils.SignedInteger2UnsignedLong(n);
    }

    public FramePacketInfo GetInfo() {
        return info;
    }
}
