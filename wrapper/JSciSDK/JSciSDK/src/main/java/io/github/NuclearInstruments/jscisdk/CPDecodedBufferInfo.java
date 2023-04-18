package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"buffer_size", "packet_size", "valid_data"})
public class CPDecodedBufferInfo extends Structure {

    public int buffer_size; // uint32
    public int packet_size; // uint32
    public int valid_data; // uint32

    public CPDecodedBufferInfo() {
        super();
    }

    public CPDecodedBufferInfo(Pointer p) {
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
