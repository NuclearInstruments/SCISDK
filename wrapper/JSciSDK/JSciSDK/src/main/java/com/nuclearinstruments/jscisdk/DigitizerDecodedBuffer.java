package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;
import java.math.BigInteger;

@Structure.FieldOrder({"magic", "analog", "hits", "timecode", "counter", "user", "info"})
public class DigitizerDecodedBuffer extends Structure implements Structure.ByReference {

    public int magic;
    public Pointer analog;
    public long hits;
    public long timecode;
    public int counter;
    public int user;
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
