package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;
import java.util.*;

@Structure.FieldOrder({"buffer_size", "total_bins", "valid_bins"})
public class SpectrumDecodedBufferInfo extends Structure {
    public int buffer_size;
    public int total_bins;
    public int valid_bins;
    
    public long GetBufferSize(){
        return buffer_size & 0xFFFFFFFFL;
    }
    
    public long GetTotalBins(){
        return total_bins & 0xFFFFFFFFL;
    }
    
    public long GetValidBins(){
        return valid_bins & 0xFFFFFFFFL;
    }
    
    
}
