package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"buffer_size", "samples_analog", "samples_digital", "tracks_analog_per_channel", "tracks_digital_per_channel", "channels"})
public class OscilloscopeDualRawBufferInfo extends Structure {

    public int buffer_size; // uint32
    public int samples_analog; // uint32
    public int samples_digital; // uint32
    public int tracks_analog_per_channel; // uint32
    public int tracks_digital_per_channel; // uint32
    public int channels; // uint32

    public OscilloscopeDualRawBufferInfo() {
    }

    public OscilloscopeDualRawBufferInfo(Pointer p) {
        super(p);
    }

    public long GetBufferSize() {
        return Utils.SignedInteger2UnsignedLong(buffer_size);
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
