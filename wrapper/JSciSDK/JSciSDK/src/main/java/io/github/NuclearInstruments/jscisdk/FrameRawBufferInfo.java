package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"buffer_size", "packet_size", "valid_data"})
public class FrameRawBufferInfo extends Structure {

    public int buffer_size;
    public int packet_size;
    public int valid_data;

    public FrameRawBufferInfo() {
        super();
    }

    public FrameRawBufferInfo(Pointer p) {
        super(p);
    }

    public long GetBufferSize() {
        return Utils.SignedInteger2UnsignedLong(buffer_size);
    }

    public long GetPacketSize() {
        return Utils.SignedInteger2UnsignedLong(packet_size);
    }

    public long GetValidData() {
        return Utils.SignedInteger2UnsignedLong(valid_data);
    }
}
