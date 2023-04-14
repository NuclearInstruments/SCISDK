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

    public long[] GetDigital() {
        long digital_count = GetInfo().GetChannels() * GetInfo().GetSamplesDigital() * GetInfo().GetTracksDigitalPerChannel();
        return Utils.Pointer2LongArray(digital, digital_count);
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
