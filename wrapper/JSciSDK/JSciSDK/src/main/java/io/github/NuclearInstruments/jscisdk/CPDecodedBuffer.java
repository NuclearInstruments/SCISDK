package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"magic", "data", "info"})
public class CPDecodedBuffer extends Structure implements Structure.ByReference {

    public int magic; // uint32
    public Pointer data; // CPPacket[]
    public CPDecodedBufferInfo info;

    public CPDecodedBuffer() {
        super();
    }

    public CPDecodedBuffer(Pointer p) {
        super(p);
    }

    public long GetMagic() {
        return Utils.SignedInteger2UnsignedLong(magic);
    }
    
    public CPPacket[] GetData(){
        return Utils.Pointer2CPPacketArray(data, info.GetBufferSize());
    }

    public CPDecodedBufferInfo GetInfo(){
        return info;
    }
}
