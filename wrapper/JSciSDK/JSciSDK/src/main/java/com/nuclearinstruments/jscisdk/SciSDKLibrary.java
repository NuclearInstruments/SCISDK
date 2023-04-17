package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;
import com.sun.jna.ptr.*;

public interface SciSDKLibrary extends Library {

    SciSDKLibrary INSTANCE = (SciSDKLibrary) Native.load((Platform.isWindows() ? "SciSDK_DLL.dll" : "libscisdk.so"), SciSDKLibrary.class);

    // method used to retrieve scisdk instance handler
    public Pointer SCISDK_InitLib();

    // method used to call dll method to destroy scisdk object instance
    public int SCISDK_FreeLib(Pointer handle);

    // method used to call dll method to attach new device
    public int SCISDK_AddNewDevice(String DevicePath, String DeviceModel, String JSONFwFilePath, String Name, Pointer handle);

    // method used to call dll method to detach device
    public int SCISDK_DetachDevice(String name, Pointer handle);

    // method used to call dll method to set string parameter value
    public int SCISDK_SetParameterString(String path, String value, Pointer handle);

    // method used to call dll method to set integer parameter value
    public int SCISDK_SetParameterInteger(String path, int value, Pointer handle);
    
    // method used to call dll method to set unsigned integer parameter value
    public int SCISDK_SetParameterUInteger(String path, int value, Pointer handle);

    // method used to call dll method to set double parameter value
    public int SCISDK_SetParameterDouble(String path, double value, Pointer handle);

    // method used to call dll method ro retrieve string parameter value
    public int SCISDK_GetParameterString(String path, String[] value, Pointer handle);

    // method used to call dll method to retrieve integer parameter value
    public int SCISDK_GetParameterInteger(String path, IntByReference value, Pointer handle);
    
    // method used to call dll method to retrieve unsigned integer parameter value
    public int SCISDK_GetParameterUInteger(String path, IntByReference value, Pointer handle);

    // method used to call dll method to retrieve double parameter value
    public int SCISDK_GetParameterDouble(String path, DoubleByReference value, Pointer handle);

    // method used to call dll method to decode data
    public int SCISDK_DecodeData(String path, Structure.ByReference buffer_in, Structure.ByReference buffer_out, Pointer handle);

    // method used to call dll method to allocate buffer
    public int SCISDK_AllocateBuffer(String path, int buffer_type, Structure.ByReference[] buffer, Pointer handle);

    // method used to call dll method to allocate buffer with size parameter
    public int SCISDK_AllocateBufferSize(String path, int buffer_type, Structure.ByReference[] buffer, Pointer handle, int size);

    // method used to call dll method to set register value
    public int SCISDK_SetRegister(String path, int value, Pointer handle);

    // method used to call dll method to get register value
    public int SCISDK_GetRegister(String path, IntByReference value, Pointer handle);

    // method used to call dll method make free buffer's memory
    public int SCISDK_FreeBuffer(String path, int buffer_type, Structure.ByReference[] buffer, Pointer handle);

    //method used to call dll method to read data
    public int SCISDK_ReadData(String path, Structure.ByReference buffer, Pointer handle);

    //method used to call dll method to read status
    public int SCISDK_ReadStatus(String path, Structure.ByReference buffer, Pointer handle);

    // method used to call dll method to get error description
    public int SCISDK_s_error(int error_no, String[] value, Pointer handle);

    // method used to call dll method to execute command
    public int SCISDK_ExecuteCommand(String path, String value, Pointer handle);

    // method used to call dll method to get component list
    public int SCISDK_GetComponentList(String name, String type, String[] ret, boolean return_json, Pointer handle);

    // method used to call dll method to get all parameters
    public int SCISDK_GetAllParameters(String path, String[] ret, Pointer handle);

    // method used to call dll method to get parameter description
    public int SCISDK_GetParameterDescription(String path, String[] ret, Pointer handle);

    // method used to call dll method to get parameter list of values
    public int SCISDK_GetParameterListOfValues(String path, String[] ret, Pointer handle);

    // method used to call dll method to get parameter minimum value
    public int SCISDK_GetParameterMinimumValue(String path, double[] re, Pointer handle);

    // method used to call dll method to get parameter maximum value
    public int SCISDK_GetParameterMaximumValue(String path, double[] ret, Pointer handle);

    // method used to call dll method to get parameter properties
    public int SCISDK_GetParametersProperties(String path, String[] ret, Pointer handle);

    // method used to call dll method to make free a scisdk allocated string
    public int SCISDK_free_string(String _str);
}
