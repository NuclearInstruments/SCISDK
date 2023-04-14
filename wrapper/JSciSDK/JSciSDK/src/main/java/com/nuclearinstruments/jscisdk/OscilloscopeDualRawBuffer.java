package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"magic", "data", "zero_position", "trigger_position", "timecode", "info"})
public class OscilloscopeDualRawBuffer extends Structure implements Structure.ByReference {

    public int magic;
    public Pointer data;
    public int zero_position;
    public int trigger_position;
    public long timecode;
    public OscilloscopeDualRawBufferInfo info;

    public long GetMagic() {
        return magic & 0xFFFFFFFFL;
    }

    public long[] GetData() {
        return Utils.Pointer2LongArray(data, info.GetBufferSize());
    }

    public long GetZeroPosition() {
        return zero_position & 0xFFFFFFFFL;
    }
    
    public long GetTriggerPosition(){
        return trigger_position & 0xFFFFFFFFL;
    }
    
    public long GetTimecode(){
        return timecode;
    }
    
    public OscilloscopeDualRawBufferInfo GetInfo(){
        return info;
    }
}
