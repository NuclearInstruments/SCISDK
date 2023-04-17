package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;
import java.math.BigInteger;

@Structure.FieldOrder({"magic", "analog", "hits", "timecode", "counter", "user", "info"})
public class DigitizerDecodedBuffer extends Structure implements Structure.ByReference {

    public int magic; // uint32
    public Pointer analog; // int32[]
    public long hits; // uint64
    public long timecode; // uint64
    public int counter; // uint32
    public int user; // uint32
    public DigitizerDecodedBufferInfo info;
    
    public DigitizerDecodedBuffer(){
        super();
    }
    
    public DigitizerDecodedBuffer(Pointer p){
        super(p);
    }

    public long GetMagic() {
        return Utils.SignedInteger2UnsignedLong(magic);
    }

    public int[] GetAnalog() {
        return Utils.Pointer2SignedIntArray(analog, info.GetValidSamples() * info.GetChannels());
    }

    public BigInteger GetHits() {
        return Utils.SignedLong2UnsignedBigInteger(hits);
    }

    public BigInteger GetTimecode() {
        return Utils.SignedLong2UnsignedBigInteger(timecode);
    }

    public long GetCounter() {
        return Utils.SignedInteger2UnsignedLong(counter);
    }

    public long GetUser() {
        return Utils.SignedInteger2UnsignedLong(user);
    }

    public DigitizerDecodedBufferInfo GetInfo() {
        return info;
    }

}
