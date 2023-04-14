package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"magic", "analog", "digital", "trigger_position", "timecode", "info"})
public class OscilloscopeDecodedBuffer extends Structure implements Structure.ByReference {
    
    public int magic;
    public Pointer analog;
    public Pointer digital;
    public int trigger_position;
    public long timecode;
    public OscilloscopeDecodedBufferInfo info;

    public long GetMagic() {
        return magic & 0xFFFFFFFFL;
    }

    public long[] GetAnalog() {
        long analog_count = GetInfo().GetSamplesAnalog() * GetInfo().GetChannels();
        return Utils.Pointer2LongArray(analog, analog_count);
    }

    public int[] GetDigital() {
        Long digital_count = GetInfo().GetChannels() * GetInfo().GetSamplesDigital() * GetInfo().GetTracksDigitalPerChannel();
        byte[] digital_b_array = digital.getByteArray(0, digital_count.intValue());
        int[] res_array = new int[digital_b_array.length];
        for(int i=0;i<digital_b_array.length;i++){
            res_array[i] =digital_b_array[i] == 1 ? 1 : 0;
        }
        return res_array;
    }

    public long GetTriggerPosition() {
        return trigger_position & 0xFFFFFFFFL;
    }
    
    public long GetTimeCode(){
        return timecode;
    }
    
    public OscilloscopeDecodedBufferInfo GetInfo(){
        return info;
    }
}
