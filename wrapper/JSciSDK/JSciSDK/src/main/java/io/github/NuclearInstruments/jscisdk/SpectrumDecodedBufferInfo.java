package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;

@Structure.FieldOrder({"buffer_size", "total_bins", "valid_bins"})
public class SpectrumDecodedBufferInfo extends Structure {

    public int buffer_size; // uint32
    public int total_bins; // uint32
    public int valid_bins; // uint32

    public SpectrumDecodedBufferInfo() {
    }

    public SpectrumDecodedBufferInfo(Pointer p) {
        super(p);
    }

    public long GetBufferSize() {
        return Utils.SignedInteger2UnsignedLong(buffer_size);
    }

    public long GetTotalBins() {
        return Utils.SignedInteger2UnsignedLong(total_bins);
    }

    public long GetValidBins() {
        return Utils.SignedInteger2UnsignedLong(valid_bins);
    }

}
