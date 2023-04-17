package com.nuclearinstruments.jscisdk;

import com.sun.jna.Pointer;
import java.math.BigInteger;

public class Utils {

    /**
     * Function used to convert read an unsigned int array from memory
     */
    public static long[] Pointer2UnsignedIntLongArray(Pointer ptr, long size) {
        Long array_size = Long.valueOf(size);
        int[] data_array_tmp = ptr.getIntArray(0, array_size.intValue());
        long[] data_array = new long[array_size.intValue()];
        for (int i = 0; i < data_array.length; i++) {
            data_array[i] = SignedInteger2UnsignedLong(data_array_tmp[i]);
        }
        return data_array;
    }

    /**
     * Function used to read a signed int array from memory
     */
    public static int[] Pointer2SignedIntArray(Pointer ptr, long size) {
        Long array_size = Long.valueOf(size);
        int[] data_array_tmp = ptr.getIntArray(0, array_size.intValue());
        int[] data_array = new int[array_size.intValue()];
        for (int i = 0; i < data_array.length; i++) {
            data_array[i] = data_array_tmp[i];
        }
        return data_array;
    }

    /**
     * Function used to read a bytes arrat from memory
     */
    public static byte[] Pointer2BytesArray(Pointer ptr, long size) {
        Long array_size = Long.valueOf(size);
        return ptr.getByteArray(0, array_size.intValue());
    }

    /**
     * Function used to convert a long variable into bytes array
     */
    public static byte[] LongToBytes(long l) {
        byte[] result = new byte[8];
        for (int i = 7; i >= 0; i--) {
            result[i] = (byte) (l & 0xFF);
            l >>= 8;
        }
        return result;
    }

    /**
     * Function used to convert 64 bit unsigned long value stored inside a 64
     * bit signed variable into a 64 bit unsigned number stored inside a
     * BigInteger object
     */
    public static BigInteger SignedLong2UnsignedBigInteger(long value) {
        return new BigInteger(1, LongToBytes(value));
    }

    /**
     * Function used to convert 32 bit unsigned integer stored inside a 32 bit
     * signed integer variable into a 64 bit long variable
     */
    public static long SignedInteger2UnsignedLong(int value) {
        return value & 0xFFFFFFFFL;
    }

    /**
     * Function used to read from memory an array of cppackets
     */
    public static CPPacket[] Pointer2CPPacketArray(Pointer ptr, long size) {
        int array_size = Long.valueOf(size).intValue();
        int structure_size = new CPPacket().size();
        CPPacket[] packets = new CPPacket[array_size];

        for (int i = 0; i < packets.length; i++) {
            Pointer obj_ptr = ptr.getPointer(i * structure_size);
            packets[i] = new CPPacket(obj_ptr);
        }

        return packets;
    }
}
