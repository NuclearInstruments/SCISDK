package com.nuclearinstruments.jscisdk;

import com.sun.jna.*;
import com.sun.jna.ptr.*;

public class SciSDK {

    // pointer to the scisdk class object
    private Pointer scisdk_handle;

    public SciSDK() {
        scisdk_handle = SciSDKLibrary.INSTANCE.SCISDK_InitLib();
    }

    public int FreeLib() {
        return SciSDKLibrary.INSTANCE.SCISDK_FreeLib(scisdk_handle);
    }

    // Method used to add new device
    public int AddNewDevice(String device_path, String model, String json_file_path, String name) {
        return SciSDKLibrary.INSTANCE.SCISDK_AddNewDevice(device_path, model, json_file_path, name, scisdk_handle);
    }

    // Method used to detach device
    public int DetachDevice(String board_name) {
        return SciSDKLibrary.INSTANCE.SCISDK_DetachDevice(board_name, scisdk_handle);
    }

    // Method used to set register value
    public int SetRegister(String path, int value) {
        return SciSDKLibrary.INSTANCE.SCISDK_SetRegister(path, value, scisdk_handle);
    }

    // Method used to retrieve register value
    public int GetRegister(String path, Ref<Integer> value) {
        IntByReference return_value = new IntByReference();
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetRegister(path, return_value, scisdk_handle);
        value.set(return_value.getValue());
        return res;
    }

    // Method used to get error description
    public int s_error(int error, Ref<String> value) {
        //Pointer value_pointer = new Memory(1);
        String[] value_pointer = new String[1];
        int res = SciSDKLibrary.INSTANCE.SCISDK_s_error(error, value_pointer, scisdk_handle);
        String error_description = value_pointer[0];
        value.set(error_description);
        return res;
    }

    // method used to set parameter integer value
    public int SetParameterInteger(String path, int value) {
        return SciSDKLibrary.INSTANCE.SCISDK_SetParameterInteger(path, value, scisdk_handle);
    }

    // method used to set parameter double value
    public int SetParameterDouble(String path, double value) {
        return SciSDKLibrary.INSTANCE.SCISDK_SetParameterDouble(path, value, scisdk_handle);
    }

    // method used to set parameter string value
    public int SetParameterString(String path, String value) {
        return SciSDKLibrary.INSTANCE.SCISDK_SetParameterString(path, value, scisdk_handle);
    }

    // method used to get parameter integer value
    public int GetParameterInteger(String path, Ref<Integer> value) {
        IntByReference return_value = new IntByReference();
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetParameterInteger(path, return_value, scisdk_handle);
        value.set(return_value.getValue());
        return res;
    }

    // method used to get parameter double value
    public int GetParameterDouble(String path, Ref<Double> value) {
        DoubleByReference return_value = new DoubleByReference();
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetParameterDouble(path, return_value, scisdk_handle);
        value.set(return_value.getValue());
        return res;
    }

    // method used to get parameter string value
    public int GetParameterString(String path, Ref<String> value) {
        String[] return_value = new String[1];
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetParameterString(path, return_value, scisdk_handle);
        value.set(return_value[0]);
        return res;
    }

    // method used to execute command
    public int ExecuteCommand(String path, String value) {
        return SciSDKLibrary.INSTANCE.SCISDK_ExecuteCommand(path, value, scisdk_handle);
    }

    // allocate buffer
    public int AllocateBuffer(String path, Ref buffer) {
        // SPECTRUM DECODED BUFFER
        if (buffer.get().getClass().equals(SpectrumDecodedBuffer.class)) {
            SpectrumDecodedBuffer[] s = new SpectrumDecodedBuffer[1];
            s[0] = new SpectrumDecodedBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 1, s, scisdk_handle);
            buffer.set(s[0]);
            return res;
        } // OSCILLOSCOPE DECODED BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeDecodedBuffer.class)) {
            OscilloscopeDecodedBuffer[] s = new OscilloscopeDecodedBuffer[1];
            s[0] = new OscilloscopeDecodedBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 1, s, scisdk_handle);
            buffer.set(s[0]);
            return res;
        }
        return -1;
    }

    // allocate buffer with specified size
    public int AllocateBuffer(String path, Ref buffer, int size) {
        // SPECTRUM DECODED BUFFER
        if (buffer.get().getClass().equals(SpectrumDecodedBuffer.class)) {
            SpectrumDecodedBuffer[] s = new SpectrumDecodedBuffer[1];
            s[0] = new SpectrumDecodedBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBufferSize(path, 1, s, scisdk_handle, size);
            buffer.set(s[0]);
            return res;
        } // OSCILLOSCOPE DECODED BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeDecodedBuffer.class)) {
            OscilloscopeDecodedBuffer[] s = new OscilloscopeDecodedBuffer[1];
            s[0] = new OscilloscopeDecodedBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBufferSize(path, 1, s, scisdk_handle, size);
            buffer.set(s[0]);
            return res;
        }
        return -1;
    }

    // read data
    public int ReadData(String path, Ref buffer) {
        // SPECTRUM DECODED BUFFER
        if (buffer.get().getClass().equals(SpectrumDecodedBuffer.class)) {
            SpectrumDecodedBuffer buf = (SpectrumDecodedBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        } // OSCILLOSCOPE DECODED BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeDecodedBuffer.class)) {
            OscilloscopeDecodedBuffer buf = (OscilloscopeDecodedBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        }
        return -1;
    }

    // make free buffer's memory
    public int FreeBuffer(String path, Ref buffer) {
        // SPECTRUM DECODED BUFFER
        if (buffer.get().getClass().equals(SpectrumDecodedBuffer.class)) {
            SpectrumDecodedBuffer[] s = new SpectrumDecodedBuffer[1];
            s[0] = (SpectrumDecodedBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_FreeBuffer(path, 1, s, scisdk_handle);
        } // OSCILLOSCOPE DECODED BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeDecodedBuffer.class)) {
            OscilloscopeDecodedBuffer[] s = new OscilloscopeDecodedBuffer[1];
            s[0] = (OscilloscopeDecodedBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_FreeBuffer(path, 1, s, scisdk_handle);
        }

        return -1;
    }

    // get component list
    public int GetComponentList(String name, String type, Ref<String> ret, boolean return_json) {
        String ret_string[] = new String[1];
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetComponentList(name, type, ret_string, return_json, scisdk_handle);
        ret.set(ret_string[0]);
        return res;
    }

    // get all parameters
    public int GetAllParameters(String path, Ref<String> ret) {
        String ret_string[] = new String[1];
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetAllParameters(path, ret_string, scisdk_handle);
        ret.set(ret_string[0]);
        return res;
    }

    // get parameter description
    public int GetParameterDescription(String path, Ref<String> ret) {
        String ret_string[] = new String[1];
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetParameterDescription(path, ret_string, scisdk_handle);
        ret.set(ret_string[0]);
        return res;
    }

    // get parameter list of values
    public int GetParameterListOfValues(String path, Ref<String> ret) {
        String ret_string[] = new String[1];
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetParameterListOfValues(path, ret_string, scisdk_handle);
        ret.set(ret_string[0]);
        return res;
    }

    // get parameter minimum value
    public int GetParameterMinimumValue(String path, Ref<Double> ret) {
        double[] ret_value = new double[1];
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetParameterMinimumValue(path, ret_value, scisdk_handle);
        ret.set(ret_value[0]);
        return res;
    }

    // get parameter maximum value
    public int GetParameterMaximumValue(String path, Ref<Double> ret) {
        double[] ret_value = new double[1];
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetParameterMaximumValue(path, ret_value, scisdk_handle);
        System.out.println(ret_value[0]);
        ret.set(ret_value[0]);
        return res;
    }

    // get parameter properties
    public int GetParameterProperties(String path, Ref<String> ret) {
        String ret_string[] = new String[1];
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetParametersProperties(path, ret_string, scisdk_handle);
        ret.set(ret_string[0]);
        return res;
    }

    public int FreeString(String _str) {
        return SciSDKLibrary.INSTANCE.SCISDK_free_string(_str);
    }

}
