package io.github.NuclearInstruments.jscisdk;

import com.sun.jna.*;
import java.math.BigInteger;

@Structure.FieldOrder({"timestamp", "trigger_count", "event_count", "hits"})
public class FramePacketInfo extends Structure {

    public long timestamp;
    public long trigger_count;
    public long event_count;
    public long hits;

    public FramePacketInfo() {
        super();
    }

    public FramePacketInfo(Pointer p) {
        super(p);
    }

    public BigInteger GetTimestamp() {
        return Utils.SignedLong2UnsignedBigInteger(timestamp);
    }

    public BigInteger GetTriggerCount() {
        return Utils.SignedLong2UnsignedBigInteger(trigger_count);
    }

    public BigInteger GetEventCount() {
        return Utils.SignedLong2UnsignedBigInteger(event_count);
    }

    public BigInteger GetHits() {
        return Utils.SignedLong2UnsignedBigInteger(hits);
    }
}
