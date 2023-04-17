package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"buffer_size", "samples_analog", "samples_digital", "tracks_analog_per_channel", "tracks_digital_per_channel", "channels"})
public class OscilloscopeDualRawBufferInfo extends Structure {

    public int buffer_size;
    public int samples_analog;
    public int samples_digital;
    public int tracks_analog_per_channel;
    public int tracks_digital_per_channel;
    public int channels;

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
