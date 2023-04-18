package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"magic", "data", "info"})
public class DigitizerRawBuffer extends Structure implements Structure.ByReference {

    public int magic; // uint32
    public Pointer data; // int32
    public DigitizerRawBufferInfo info;

    public DigitizerRawBuffer() {
        super();
    }

    public DigitizerRawBuffer(Pointer p) {
        super(p);
    }

    public long GetMagic() {
        return Utils.SignedInteger2UnsignedLong(magic);
    }

    public int[] GetData() {
        return Utils.Pointer2SignedIntArray(data, info.GetValidSamples() * info.GetChannels());
    }

    public DigitizerRawBufferInfo GetInfo() {
        return info;
    }

}
