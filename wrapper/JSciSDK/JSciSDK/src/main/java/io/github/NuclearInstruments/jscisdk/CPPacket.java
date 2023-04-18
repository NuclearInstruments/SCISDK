package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"row", "n"})
public class CPPacket extends Structure {

    public Pointer row; // uint32[]
    public int n; // uint32
    
    public CPPacket(){
        super();
    }
    
    public CPPacket(Pointer p){
        super(p);
        read();
    }
    
    public long GetN(){
        return Utils.SignedInteger2UnsignedLong(n);
    }
    
    public long[] GetRow(){
        return Utils.Pointer2UnsignedIntLongArray(row, GetN());
    }

}
