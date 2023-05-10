package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"buffer_size", "valid_data"})
public class FrameDecodedBufferInfo extends Structure {

    public int buffer_size;
    public int valid_data;

    public FrameDecodedBufferInfo() {
        super();
    }

    public FrameDecodedBufferInfo(Pointer p) {
        super(p);
    }

    public long GetBufferSize() {
        return Utils.SignedInteger2UnsignedLong(buffer_size);
    }

    public long GetValidData() {
        return Utils.SignedInteger2UnsignedLong(valid_data);
    }
}
