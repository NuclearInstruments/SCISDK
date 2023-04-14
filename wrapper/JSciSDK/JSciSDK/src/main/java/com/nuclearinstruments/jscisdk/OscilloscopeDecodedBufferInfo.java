package com.nuclearinstruments.jscisdk;

import com.sun.jna.Structure;

@Structure.FieldOrder({"samples_analog", "samples_digital", "tracks_analog_per_channel", "tracks_digital_per_channel", "channels"})
    public class OscilloscopeDecodedBufferInfo extends Structure{
    
        public int samples_analog;
        public int samples_digital;
        public int tracks_analog_per_channel;
        public int tracks_digital_per_channel;
        public int channels;
        
        
        public long GetSamplesAnalog(){
            return samples_analog & 0xFFFFFFFFL;
        }
        
        public long GetSamplesDigital(){
            return samples_digital & 0xFFFFFFFFL;
        }
        
        public long GetTracksAnalogPerChannel(){
            return tracks_analog_per_channel & 0xFFFFFFFFL;
        }
        
        public long GetTracksDigitalPerChannel(){
            return tracks_digital_per_channel & 0xFFFFFFFFL;
        }
        
        public long GetChannels(){
            return channels & 0xFFFFFFFFL;
        }
    }
