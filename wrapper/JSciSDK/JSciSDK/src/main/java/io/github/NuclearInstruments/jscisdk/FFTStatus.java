package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"armed", "ready", "running"})
public class FFTStatus extends Structure implements Structure.ByReference{
    
    public boolean armed;
    public boolean ready;
    public boolean running;
    
    public FFTStatus(){
        super();
    }
    
    public FFTStatus(Pointer p){
        super(p);
    }
    
    public boolean GetArmed(){
        return armed;
    }
    
    public boolean GetReady(){
        return ready;
    }
    
    public boolean GetRunning(){
        return running;
    }    
}
