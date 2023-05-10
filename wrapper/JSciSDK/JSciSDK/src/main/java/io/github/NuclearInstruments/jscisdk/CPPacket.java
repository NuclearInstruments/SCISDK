package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;
import com.sun.jna.ptr.IntByReference;

@Structure.FieldOrder({"row", "n"})
public class CPPacket extends Structure {

    public Pointer row;// uint32[]
    public int n; // uint32

    public CPPacket() {
        super();
    }

    public CPPacket(Pointer p) {
        super(p);
        read();
//        readField("n");
//        System.out.println(n);
//        //row = new long[n];
//        readField("row");
    }

    public long GetN() {
        return Utils.SignedInteger2UnsignedLong(n);
    }

    public int[] GetRow() {
        return row.getIntArray(0, 9);
    }

}
