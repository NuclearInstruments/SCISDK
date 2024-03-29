package io.github.NuclearInstruments.jscisdk;

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
     * Function used to read a bytes array from memory
     */
    public static byte[] Pointer2BytesArray(Pointer ptr, long size) {
        Long array_size = Long.valueOf(size);
        return ptr.getByteArray(0, array_size.intValue());
    }

    /**
     * Function used to read a double array from memory
     */
    public static double[] Pointer2DoubleArray(Pointer ptr, long size) {
        Long array_size = Long.valueOf(size);
        return ptr.getDoubleArray(0, array_size.intValue());
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
     * Function used to convert bytes array into integer number
     */
    public static int Bytes2Int(byte[] bytes) {
        int value = 0;
        for (byte b : bytes) {
            value = (value << 8) + (b & 0xFF);
        }
        return value;
    }

    /**
     * Function used to convert 32 bit unsigned int stored inside 64 bit long
     * type variable into a signed 32 bit integer variable
     */
    public static int UnsignedInt32Long2SignedInt32(long value) {
        return Bytes2Int(LongToBytes(value));
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
    public static CPPacket[] Pointer2CPPacketArray(long ptr, long size) {
        int array_size = Long.valueOf(size).intValue();
        int structure_size = new CPPacket().size();
        CPPacket[] packets = new CPPacket[array_size];
        for (int i = 0; i < packets.length; i++) {
            Pointer temp_ptr = new Pointer(ptr + structure_size * i);
            packets[i] = new CPPacket(temp_ptr);
        }
        return packets;
    }

    /**
     * Function used to read from memory an array of frame packet
     */
    public static FramePacket[] Pointer2FramePacketArray(long ptr, long size) {
        int array_size = Long.valueOf(size).intValue();
        int structure_size = new FramePacket().size();
        FramePacket[] packets = new FramePacket[array_size];

        for (int i = 0; i < packets.length; i++) {
            Pointer temp_ptr = new Pointer(ptr + structure_size * i);
            packets[i] = new FramePacket(temp_ptr);
        }

        return packets;
    }
}
