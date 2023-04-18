package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;
import java.math.BigInteger;

@Structure.FieldOrder({"magic", "data", "zero_position", "trigger_position", "timecode", "info"})
public class OscilloscopeDualRawBuffer extends Structure implements Structure.ByReference {

    public int magic; // uint32
    public Pointer data; // int32
    public int zero_position; // uint32
    public int trigger_position; // uint32
    public long timecode; // uint64
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

    public BigInteger GetTimecode() {
        return Utils.SignedLong2UnsignedBigInteger(timecode);
    }

    public OscilloscopeDualRawBufferInfo GetInfo() {
        return info;
    }
}
