package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"armed", "ready", "running"})
public class OscilloscopeStatus extends Structure implements Structure.ByReference {

    public boolean armed;
    public boolean ready;
    public boolean running;

    public OscilloscopeStatus() {
        super();
    }

    public OscilloscopeStatus(Pointer p) {
        super(p);
    }

    public boolean IsArmed() {
        return armed;
    }

    public boolean IsReady() {
        return ready;
    }

    public boolean IsRunning() {
        return running;
    }
}
