package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"magic", "data", "info"})
public class ListRawBuffer extends Structure implements Structure.ByReference {

    public int magic; // uint32
    public Pointer data; // bytes array
    public ListRawBufferInfo info;

    public ListRawBuffer() {
        super();
    }

    public ListRawBuffer(Pointer p) {
        super(p);
    }

    public long GetMagic() {
        return Utils.SignedInteger2UnsignedLong(magic);
    }

    public byte[] GetData() {
        return Utils.Pointer2BytesArray(data, info.GetBufferSize());
    }

    public ListRawBufferInfo GetInfo() {
        return info;
    }
}
