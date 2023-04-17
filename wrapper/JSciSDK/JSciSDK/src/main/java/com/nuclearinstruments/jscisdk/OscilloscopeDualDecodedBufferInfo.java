package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"samples_analog", "samples_digital", "tracks_analog_per_channel", "tracks_digital_per_channel", "channels"})
public class OscilloscopeDualDecodedBufferInfo extends Structure {

    public int samples_analog;
    public int samples_digital;
    public int tracks_analog_per_channel;
    public int tracks_digital_per_channel;
    public int channels;
    
    public OscilloscopeDualDecodedBufferInfo(){
        super();
    }
    
    public OscilloscopeDualDecodedBufferInfo(Pointer p){
        super(p);
    }

    public long GetSamplesAnalog() {
        return Utils.SignedInteger2UnsignedLong(samples_analog);
    }

    public long GetSamplesDigital() {
        return Utils.SignedInteger2UnsignedLong(samples_digital);
    }

    public long GetTracksAnalogPerChannel() {
        return Utils.SignedInteger2UnsignedLong(tracks_analog_per_channel);
    }

    public long GetTracksDigitalPerChannel() {
        return Utils.SignedInteger2UnsignedLong(tracks_digital_per_channel);
    }

    public long GetChannels() {
        return Utils.SignedInteger2UnsignedLong(channels);
    }

}
