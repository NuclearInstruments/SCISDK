package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"samples_analog", "samples_digital", "tracks_analog_per_channel", "tracks_digital_per_channel", "channels"})
public class OscilloscopeDecodedBufferInfo extends Structure {

    public int samples_analog; // uint32
    public int samples_digital; // uint32
    public int tracks_analog_per_channel; // uint32
    public int tracks_digital_per_channel; // uint32
    public int channels; // uint32

    public OscilloscopeDecodedBufferInfo() {
        super();
    }

    public OscilloscopeDecodedBufferInfo(Pointer p) {
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
