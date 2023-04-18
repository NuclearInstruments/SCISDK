package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"buffer_size", "valid_data"})
public class CPRawBufferInfo extends Structure {

    public int buffer_size; // uint32
    public int valid_data; // uint32

    public CPRawBufferInfo() {
        super();
    }

    public CPRawBufferInfo(Pointer p) {
        super(p);
    }

    public long GetBufferSize() {
        return Utils.SignedInteger2UnsignedLong(buffer_size);
    }

    public long GetValidData() {
        return Utils.SignedInteger2UnsignedLong(valid_data);
    }

}
