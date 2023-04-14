package com.nuclearinstruments.jscisdk;

import com.sun.jna.Pointer;


public class Utils {
    
    public static long[] Pointer2LongArray(Pointer ptr, long size){
        Long array_size = Long.valueOf(size);
        int[] data_array_tmp = ptr.getIntArray(0,  array_size.intValue());
        long[] data_array = new long[array_size.intValue()];
        for(int i=0;i<data_array.length;i++){
            data_array[i] = data_array_tmp[i] & 0xFFFFFFFFL;
        }
        return data_array;
    }
    
}
