package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;
import java.math.BigInteger;

@Structure.FieldOrder({"magic", "analog", "digital", "trigger_position", "timecode", "info"})
public class OscilloscopeDecodedBuffer extends Structure implements Structure.ByReference {

    public int magic; //  uint32
    public Pointer analog; // int[]
    public Pointer digital; // bool[]
    public int trigger_position; // uint32
    public long timecode; //uint64
    public OscilloscopeDecodedBufferInfo info;
    
    public OscilloscopeDecodedBuffer(){
        super();
    }
    
    public OscilloscopeDecodedBuffer(Pointer p){
        super(p);
    }

    public long GetMagic() {
        return Utils.SignedInteger2UnsignedLong(magic);
    }

    public int[] GetAnalog() {
        long analog_count = GetInfo().GetSamplesAnalog() * GetInfo().GetChannels();
        return Utils.Pointer2SignedIntArray(analog, analog_count);
    }

    public int[] GetDigital() {
        Long digital_count = GetInfo().GetChannels() * GetInfo().GetSamplesDigital() * GetInfo().GetTracksDigitalPerChannel();
        byte[] digital_b_array = digital.getByteArray(0, digital_count.intValue());
        int[] res_array = new int[digital_b_array.length];
        for (int i = 0; i < digital_b_array.length; i++) {
            res_array[i] = digital_b_array[i] == 1 ? 1 : 0;
        }
        return res_array;
    }

    public long GetTriggerPosition() {
        return Utils.SignedInteger2UnsignedLong(trigger_position);
    }

    public BigInteger GetTimeCode() {
        return Utils.SignedLong2UnsignedBigInteger(timecode);
    }

    public OscilloscopeDecodedBufferInfo GetInfo() {
        return info;
    }
}
