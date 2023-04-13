package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;
import com.sun.jna.ptr.*;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.concurrent.atomic.*;


public class SciSDK {
    
    // pointer to the scisdk class object
    private Pointer scisdk_handle;
    
    public SciSDK(){
        scisdk_handle = SciSDKLibrary.INSTANCE.SCISDK_InitLib();
    }
    
    public int FreeLib(){
        return SciSDKLibrary.INSTANCE.SCISDK_FreeLib(scisdk_handle);
    }
    
    // Method used to add new device
    public int AddNewDevice(String device_path, String model, String json_file_path, String name){
        return SciSDKLibrary.INSTANCE.SCISDK_AddNewDevice(device_path, model, json_file_path, name, scisdk_handle);
    }
    
    // Method used to detach device
    public int DetachDevice(String board_name){
        return SciSDKLibrary.INSTANCE.SCISDK_DetachDevice(board_name, scisdk_handle);
    }
    
    // Method used to set register value
    public int SetRegister(String path, int value){
        return SciSDKLibrary.INSTANCE.SCISDK_SetRegister(path, value, scisdk_handle);
    }
    
    // Method used to retrieve register value
    public int GetRegister(String path, AtomicInteger value){
        IntByReference return_value = new IntByReference();
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetRegister(path, return_value, scisdk_handle);
        value.set(return_value.getValue());
        return res;
    }
    
    // Method used to get error description
    public int s_error(int error, AtomicReference<String> value){
        //Pointer value_pointer = new Memory(1);
        String[] value_pointer = new String[1];
        int res = SciSDKLibrary.INSTANCE.SCISDK_s_error(error, value_pointer, scisdk_handle);
        String error_description = value_pointer[0];
        value.set(error_description);
        return res;
    }
    
    // method used to set parameter integer value
    public int SetParameterInteger(String path, int value){
        return SciSDKLibrary.INSTANCE.SCISDK_SetParameterInteger(path, value, scisdk_handle);
    }
    
    // method used to set parameter double value
    public int SetParameterDouble(String path, double value){
        return SciSDKLibrary.INSTANCE.SCISDK_SetParameterDouble(path, value, scisdk_handle);
    }
    
    // method used to set parameter string value
    public int SetParameterString(String path, String value){      
        return SciSDKLibrary.INSTANCE.SCISDK_SetParameterString(path, value, scisdk_handle);
    }
    
    // method used to get parameter integer value
    public int GetParameterInteger(String path, AtomicInteger value){
        IntByReference return_value = new IntByReference();
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetParameterInteger(path, return_value, scisdk_handle);
        value.set(return_value.getValue());
        return res;
    }
    
    // method used to get parameter double value
    public int GetParameterDouble(String path, AtomicReference<Double> value){
        DoubleByReference return_value = new DoubleByReference();
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetParameterDouble(path, return_value, scisdk_handle);
        value.set(return_value.getValue());
        return res;
    }
    
    // method used to get parameter string value
    public int GetParameterString(String path, AtomicReference<String> value){
        String[] return_value = new String[1];
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetParameterString(path, return_value, scisdk_handle);
        value.set(return_value[0]);
        return res;
    }
    
    // method used to execute command
    public int ExecuteCommand(String path, String value){
        return SciSDKLibrary.INSTANCE.SCISDK_ExecuteCommand(path, value, scisdk_handle);
    }
    
    
    static long convertToLong(byte[] bytes)
    {
        long value = 0l;
 
        // Iterating through for loop
        for (byte b : bytes) {
            // Shifting previous value 8 bits to right and
            // add it with next value
            value = (value << 8) + (b & 255);
        }
 
        return value;
    }
    
    public int AllocateBuffer(String path, AtomicReference<SpectrumDecodedBuffer> buffer){
        SpectrumDecodedBuffer.ByReference[] s = new SpectrumDecodedBuffer.ByReference[1];
        s[0] = new SpectrumDecodedBuffer.ByReference();
        int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 1, s, scisdk_handle);
        buffer.set(s[0]);
        return res;
    }
    
    
    
}
