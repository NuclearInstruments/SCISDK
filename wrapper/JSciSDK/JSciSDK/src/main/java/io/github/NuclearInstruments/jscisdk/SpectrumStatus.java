package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"running", "completed", "progress", "peak_max", "total_counter", "integration_time"})
public class SpectrumStatus extends Structure implements Structure.ByReference{
    
    public boolean running;
    public boolean completed;
    public int progress;
    public int peak_max;
    public int total_counter;
    public double integration_time;
    
    public SpectrumStatus(){
        super();
    }
    
    public SpectrumStatus(Pointer p){
        super(p);
    }
    
    public boolean GetRunning(){
        return running;
    }
    
    public boolean GetCompleted(){
        return completed;
    }
    
    public long GetProgress(){
        return Utils.SignedInteger2UnsignedLong(progress);
    }
    
    public long GetPeakMax(){
        return Utils.SignedInteger2UnsignedLong(peak_max);
    }
    
    public long GetTotalCounter(){
        return Utils.SignedInteger2UnsignedLong(total_counter);
    }
    
    public double GetIntegrationTime(){
        return integration_time;
    }
}
