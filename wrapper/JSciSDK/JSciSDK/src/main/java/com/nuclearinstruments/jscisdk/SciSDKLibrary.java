package com.nuclearinstruments.jscisdk;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.*;

public interface SciSDKLibrary extends Library {

    SciSDKLibrary INSTANCE = (SciSDKLibrary) Native.load((Platform.isWindows() ? "SciSDK_DLL.dll" : "libscisdk.so"), SciSDKLibrary.class);

    // method used to retrieve scisdk instance handler
    public Pointer SCISDK_InitLib();

    public int SCISDK_FreeLib(Pointer handle);

    // method used to call dll method to attach new device
    public int SCISDK_AddNewDevice(String DevicePath, String DeviceModel, String JSONFwFilePath, String Name, Pointer handle);

    // method used to call dll method to detach device
    public int SCISDK_DetachDevice(String name, Pointer handle);

    // method used to call dll method to set register value
    public int SCISDK_SetRegister(String path, int value, Pointer handle);

    // method used to call dll method to get register value
    public int SCISDK_GetRegister(String path, IntByReference value, Pointer handle);

    // method used to call dll method to get error description
    public int SCISDK_s_error(int error_no, String[] value, Pointer handle);
    
    // method used to call dll method to set string parameter value
    public int SCISDK_SetParameterString(String path, String value, Pointer handle);
    
    // method used to call dll method to set integer parameter value
    public int SCISDK_SetParameterInteger(String path, int value, Pointer handle);
    
    // method used to call dll method to set double parameter value
    public int SCISDK_SetParameterDouble(String path, double value, Pointer handle);
    
    // method used to call dll method ro retrieve string parameter value
    public int SCISDK_GetParameterString(String path, String[] value, Pointer handle);
    
    // method used to call dll method to retrieve integer parameter value
    public int SCISDK_GetParameterInteger(String path, IntByReference value, Pointer handle);
    
    // method used to call dll method to retrieve double parameter value
    public int SCISDK_GetParameterDouble(String path, DoubleByReference  value, Pointer handle);
}
