package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;
import java.util.*;

public class SpectrumDecodedBufferInfo extends Structure {
    public long buffer_size;
    public long total_bins;
    public long valid_bins;
    
    @Override
    protected List<String> getFieldOrder() {
        return Arrays.asList("buffer_size", "total_bins", "valid_bins");
    }
    
    public long GetBufferSize(){
        return buffer_size & 0xFFFFFFFFL;
    }
    
    
}
