package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"magic", "data", "info"})
public class CPDecodedBuffer extends Structure implements Structure.ByReference {

    public int magic; // uint32
    public long data; // CPPacket[]
    public CPDecodedBufferInfo info;

    public CPDecodedBuffer() {
        super();
    }

    public CPDecodedBuffer(Pointer p) {
        super(p);
        readField("magic");
        readField("data");
        readField("info");
    }

    public long GetMagic() {
        return Utils.SignedInteger2UnsignedLong(magic);
    }
    
    public CPPacket[] GetData(){
        return Utils.Pointer2CPPacketArray(data, info.GetValidData());
    }

    public CPDecodedBufferInfo GetInfo(){
        return info;
    }
}
