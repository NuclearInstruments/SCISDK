package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"magic", "data", "zero_position", "trigger_position", "timecode", "info"})
public class OscilloscopeDualRawBuffer extends Structure implements Structure.ByReference {

    public int magic;
    public Pointer data;
    public int zero_position;
    public int trigger_position;
    public long timecode;
    public OscilloscopeDualRawBufferInfo info;

    public OscilloscopeDualRawBuffer() {
        super();
    }

    public OscilloscopeDualRawBuffer(Pointer p) {
        super(p);
    }

    public long GetMagic() {
        return Utils.SignedInteger2UnsignedLong(magic);
    }

    public long[] GetData() {
        return Utils.Pointer2UnsignedIntLongArray(data, info.GetBufferSize());
    }

    public long GetZeroPosition() {
        return Utils.SignedInteger2UnsignedLong(zero_position);
    }

    public long GetTriggerPosition() {
        return Utils.SignedInteger2UnsignedLong(trigger_position);
    }

    public long GetTimecode() {
        return timecode;
    }

    public OscilloscopeDualRawBufferInfo GetInfo() {
        return info;
    }
}
