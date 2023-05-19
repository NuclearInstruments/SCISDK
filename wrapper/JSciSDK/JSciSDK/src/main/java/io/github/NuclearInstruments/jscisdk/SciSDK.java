package io.github.NuclearInstruments.jscisdk;

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
    public int SetRegister(String path, long value) throws SciSDKException {
        if (value >= 0) {
            return SciSDKLibrary.INSTANCE.SCISDK_SetRegister(path, Utils.UnsignedInt32Long2SignedInt32(value), scisdk_handle);
        } else {
            throw new SciSDKException("Register value must be equal or greater than zero");
        }
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
        String[] value_pointer = new String[1];
        int res = SciSDKLibrary.INSTANCE.SCISDK_s_error(error, value_pointer, scisdk_handle);
        value.set(value_pointer[0]);
        return res;
    }

    // method used to set parameter integer value
    public int SetParameterInteger(String path, int value) {
        return SciSDKLibrary.INSTANCE.SCISDK_SetParameterInteger(path, value, scisdk_handle);
    }

    // method used to set unsigned integer parameter value
    public int SetParameterUnsignedInteger(String path, long value) throws SciSDKException {
        if (value < 0) {
            throw new SciSDKException("Register value must be equal or greater than zero");
        }
        return SciSDKLibrary.INSTANCE.SCISDK_SetParameterUInteger(path, Utils.UnsignedInt32Long2SignedInt32(value), scisdk_handle);
    }

    // method used to set double parameter value
    public int SetParameterDouble(String path, double value) {
        return SciSDKLibrary.INSTANCE.SCISDK_SetParameterDouble(path, value, scisdk_handle);
    }

    // method used to set string parameter value
    public int SetParameterString(String path, String value) {
        return SciSDKLibrary.INSTANCE.SCISDK_SetParameterString(path, value, scisdk_handle);
    }

    // method used to get integer parameter value
    public int GetParameterInteger(String path, Ref<Integer> value) {
        IntByReference return_value = new IntByReference();
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetParameterInteger(path, return_value, scisdk_handle);
        value.set(return_value.getValue());
        return res;
    }

    // method used to get unsigned integer parameter value
    public int GetParameterUnsignedInteger(String path, Ref<Long> value) {
        IntByReference return_value = new IntByReference();
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetParameterUInteger(path, return_value, scisdk_handle);
        value.set(Utils.SignedInteger2UnsignedLong(return_value.getValue()));
        return res;
    }

    // method used to get double parameter value
    public int GetParameterDouble(String path, Ref<Double> value) {
        DoubleByReference return_value = new DoubleByReference();
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetParameterDouble(path, return_value, scisdk_handle);
        value.set(return_value.getValue());
        return res;
    }

    // method used to get string parameter value
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
        // buffer type 0=raw, 1=decoded
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
        } // OSCILLOSCOPE RAW BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeRawBuffer.class)) {
            OscilloscopeRawBuffer[] s = new OscilloscopeRawBuffer[1];
            s[0] = new OscilloscopeRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 0, s, scisdk_handle);
            buffer.set(s[0]);
            return res;
        } // OSCILLOSCOPE DUAL DECODED BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeDualDecodedBuffer.class)) {
            OscilloscopeDualDecodedBuffer[] s = new OscilloscopeDualDecodedBuffer[1];
            s[0] = new OscilloscopeDualDecodedBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 1, s, scisdk_handle);
            buffer.set(s[0]);
            return res;
        } // OSCILLOSCOPE DUAL RAW BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeDualRawBuffer.class)) {
            OscilloscopeDualRawBuffer[] s = new OscilloscopeDualRawBuffer[1];
            s[0] = new OscilloscopeDualRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 0, s, scisdk_handle);
            buffer.set(s[0]);
            return res;
        } // DIGITIZER DECODED BUFFER
        else if (buffer.get().getClass().equals(DigitizerDecodedBuffer.class)) {
            DigitizerDecodedBuffer[] s = new DigitizerDecodedBuffer[1];
            s[0] = new DigitizerDecodedBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 1, s, scisdk_handle);
            buffer.set(s[0]);
            return res;
        } // DIGITIZER RAW BUFFER
        else if (buffer.get().getClass().equals(DigitizerRawBuffer.class)) {
            DigitizerRawBuffer[] s = new DigitizerRawBuffer[1];
            s[0] = new DigitizerRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 0, s, scisdk_handle);
            buffer.set(s[0]);
            return res;
        } // LIST RAW BUFFER
        else if (buffer.get().getClass().equals(ListRawBuffer.class)) {
            ListRawBuffer[] s = new ListRawBuffer[1];
            s[0] = new ListRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 0, s, scisdk_handle);
            buffer.set(s[0]);
            return res;
        } // CUSTOM PACKET DECODED BUFFER
        else if (buffer.get().getClass().equals(CPDecodedBuffer.class)) {
            CPDecodedBuffer[] s = new CPDecodedBuffer[1];
            s[0] = new CPDecodedBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 1, s, scisdk_handle);
            buffer.set(s[0]);
            return res;
        } // CUSTOM PACKET RAW BUFFER
        else if (buffer.get().getClass().equals(CPRawBuffer.class)) {
            CPRawBuffer[] s = new CPRawBuffer[1];
            s[0] = new CPRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 0, s, scisdk_handle);
            buffer.set(s[0]);
            return res;
        } // RATEMETER RAW BUFFER
        else if (buffer.get().getClass().equals(RMRawBuffer.class)) {
            RMRawBuffer[] s = new RMRawBuffer[1];
            s[0] = new RMRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 0, s, scisdk_handle);
            buffer.set(s[0]);
            return res;
        } // FFT Decoded Buffer
        else if (buffer.get().getClass().equals(FFTDecodedBuffer.class)) {
            FFTDecodedBuffer[] s = new FFTDecodedBuffer[1];
            s[0] = new FFTDecodedBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 1, s, scisdk_handle);
            buffer.set(s[0]);
            return res;
        } // FFT Raw Buffer
        else if (buffer.get().getClass().equals(FFTRawBuffer.class)) {
            FFTRawBuffer[] s = new FFTRawBuffer[1];
            s[0] = new FFTRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 0, s, scisdk_handle);
            buffer.set(s[0]);
            return res;
        }
        else if (buffer.get().getClass().equals(FrameRawBuffer.class)) {
            FrameRawBuffer[] s = new FrameRawBuffer[1];
            s[0] = new FrameRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 0, s, scisdk_handle);
            buffer.set(s[0]);
            return res;
        }
        else if (buffer.get().getClass().equals(FrameDecodedBuffer.class)) {
            FrameDecodedBuffer[] s = new FrameDecodedBuffer[1];
            s[0] = new FrameDecodedBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBuffer(path, 1, s, scisdk_handle);
            buffer.set(s[0]);
            return res;
        }
        return -1;
    }

    // allocate buffer with specified size
    public int AllocateBuffer(String path, Ref buffer, int size) {
        // buffer type 0=raw, 1=decoded
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
        } // OSCILLOSCOPE RAW BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeRawBuffer.class)) {
            OscilloscopeRawBuffer[] s = new OscilloscopeRawBuffer[1];
            s[0] = new OscilloscopeRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBufferSize(path, 0, s, scisdk_handle, size);
            buffer.set(s[0]);
            return res;
        } // OSCILLOSCOPE DUAL DECODED BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeDualDecodedBuffer.class)) {
            OscilloscopeDualDecodedBuffer[] s = new OscilloscopeDualDecodedBuffer[1];
            s[0] = new OscilloscopeDualDecodedBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBufferSize(path, 1, s, scisdk_handle, size);
            buffer.set(s[0]);
            return res;
        } // OSCILLOSCOPE DUAL RAW BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeDualRawBuffer.class)) {
            OscilloscopeDualRawBuffer[] s = new OscilloscopeDualRawBuffer[1];
            s[0] = new OscilloscopeDualRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBufferSize(path, 0, s, scisdk_handle, size);
            buffer.set(s[0]);
            return res;
        } // DIGITIZER DECODED BUFFER
        else if (buffer.get().getClass().equals(DigitizerDecodedBuffer.class)) {
            DigitizerDecodedBuffer[] s = new DigitizerDecodedBuffer[1];
            s[0] = new DigitizerDecodedBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBufferSize(path, 1, s, scisdk_handle, size);
            buffer.set(s[0]);
            return res;
        } // DIGITIZER RAW BUFFER
        else if (buffer.get().getClass().equals(DigitizerRawBuffer.class)) {
            DigitizerRawBuffer[] s = new DigitizerRawBuffer[1];
            s[0] = new DigitizerRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBufferSize(path, 0, s, scisdk_handle, size);
            buffer.set(s[0]);
            return res;
        } // LIST RAW BUFFER
        else if (buffer.get().getClass().equals(ListRawBuffer.class)) {
            ListRawBuffer[] s = new ListRawBuffer[1];
            s[0] = new ListRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBufferSize(path, 0, s, scisdk_handle, size);
            buffer.set(s[0]);
            return res;
        } // CUSTOM PACKET DECODED BUFFER
        else if (buffer.get().getClass().equals(CPDecodedBuffer.class)) {
            CPDecodedBuffer[] s = new CPDecodedBuffer[1];
            s[0] = new CPDecodedBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBufferSize(path, 1, s, scisdk_handle, size);
            buffer.set(s[0]);
            return res;
        } // CUSTOM PACKET RAW BUFFER
        else if (buffer.get().getClass().equals(CPRawBuffer.class)) {
            CPRawBuffer[] s = new CPRawBuffer[1];
            s[0] = new CPRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBufferSize(path, 0, s, scisdk_handle, size);
            buffer.set(s[0]);
            return res;
        } // RATEMETER RAW BUFFER
        else if (buffer.get().getClass().equals(RMRawBuffer.class)) {
            RMRawBuffer[] s = new RMRawBuffer[1];
            s[0] = new RMRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBufferSize(path, 0, s, scisdk_handle, size);
            buffer.set(s[0]);
            return res;
        } // FFT Decoded Buffer
        else if (buffer.get().getClass().equals(FFTDecodedBuffer.class)) {
            FFTDecodedBuffer[] s = new FFTDecodedBuffer[1];
            s[0] = new FFTDecodedBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBufferSize(path, 1, s, scisdk_handle, size);
            buffer.set(s[0]);
            return res;
        } // FFT Raw Buffer
        else if (buffer.get().getClass().equals(FFTRawBuffer.class)) {
            FFTRawBuffer[] s = new FFTRawBuffer[1];
            s[0] = new FFTRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBufferSize(path, 0, s, scisdk_handle, size);
            buffer.set(s[0]);
            return res;
        }
        else if (buffer.get().getClass().equals(FrameRawBuffer.class)) {
            FrameRawBuffer[] s = new FrameRawBuffer[1];
            s[0] = new FrameRawBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBufferSize(path, 0, s, scisdk_handle, size);
            buffer.set(s[0]);
            return res;
        }
        else if (buffer.get().getClass().equals(FrameDecodedBuffer.class)) {
            FrameDecodedBuffer[] s = new FrameDecodedBuffer[1];
            s[0] = new FrameDecodedBuffer();
            int res = SciSDKLibrary.INSTANCE.SCISDK_AllocateBufferSize(path, 1, s, scisdk_handle, size);
            buffer.set(s[0]);
            return res;
        }
        return -1;
    }

    // read data
    public int ReadData(String path, Ref buffer) {
        Structure.ByReference buf = (Structure.ByReference) buffer.get();
        int res = SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        buffer.set(buf);
        return res;
        /*// SPECTRUM DECODED BUFFER
        if (buffer.get().getClass().equals(SpectrumDecodedBuffer.class)) {
            SpectrumDecodedBuffer buf = (SpectrumDecodedBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        } // OSCILLOSCOPE DECODED BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeDecodedBuffer.class)) {
            OscilloscopeDecodedBuffer buf = (OscilloscopeDecodedBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        }// OSCILLOSCOPE RAW BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeRawBuffer.class)) {
            OscilloscopeRawBuffer buf = (OscilloscopeRawBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        } // OSCILLOSCOPE DUAL DECODED BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeDualDecodedBuffer.class)) {
            OscilloscopeDualDecodedBuffer buf = (OscilloscopeDualDecodedBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        } // OSCILLOSCOPE DUAL RAW BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeDualRawBuffer.class)) {
            OscilloscopeDualRawBuffer buf = (OscilloscopeDualRawBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        } // DIGITIZER DECODED BUFFER
        else if (buffer.get().getClass().equals(DigitizerDecodedBuffer.class)) {
            DigitizerDecodedBuffer buf = (DigitizerDecodedBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        } // DIGITIZER RAW BUFFER
        else if (buffer.get().getClass().equals(DigitizerRawBuffer.class)) {
            DigitizerRawBuffer buf = (DigitizerRawBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        } // LIST RAW BUFFER
        else if (buffer.get().getClass().equals(ListRawBuffer.class)) {
            ListRawBuffer buf = (ListRawBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        } // CUSTOM PACKET DECODED BUFFER
        else if (buffer.get().getClass().equals(CPDecodedBuffer.class)) {
            CPDecodedBuffer buf = (CPDecodedBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        } // CUSTOM PACKET RAW BUFFER
        else if (buffer.get().getClass().equals(CPRawBuffer.class)) {
            CPRawBuffer buf = (CPRawBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        } // RATEMETER RAW BUFFER
        else if (buffer.get().getClass().equals(RMRawBuffer.class)) {
            RMRawBuffer buf = (RMRawBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        } // FFT Decoded Buffer
        else if (buffer.get().getClass().equals(FFTDecodedBuffer.class)) {
            FFTDecodedBuffer buf = (FFTDecodedBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        } // FFT Raw Buffer
        else if (buffer.get().getClass().equals(FFTRawBuffer.class)) {
            FFTRawBuffer buf = (FFTRawBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_ReadData(path, buf, scisdk_handle);
        }

        return -1;*/
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
        }// OSCILLOSCOPE RAW BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeRawBuffer.class)) {
            OscilloscopeRawBuffer[] s = new OscilloscopeRawBuffer[1];
            s[0] = (OscilloscopeRawBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_FreeBuffer(path, 0, s, scisdk_handle);
        } // OSCILLOSCOPE DUAL DECODED BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeDualDecodedBuffer.class)) {
            OscilloscopeDualDecodedBuffer[] s = new OscilloscopeDualDecodedBuffer[1];
            s[0] = (OscilloscopeDualDecodedBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_FreeBuffer(path, 1, s, scisdk_handle);
        } // OSCILLOSCOPE DUAL RAW BUFFER
        else if (buffer.get().getClass().equals(OscilloscopeDualRawBuffer.class)) {
            OscilloscopeDualRawBuffer[] s = new OscilloscopeDualRawBuffer[1];
            s[0] = (OscilloscopeDualRawBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_FreeBuffer(path, 0, s, scisdk_handle);
        } // DIGITIZER DECODED BUFFER
        else if (buffer.get().getClass().equals(DigitizerDecodedBuffer.class)) {
            DigitizerDecodedBuffer[] s = new DigitizerDecodedBuffer[1];
            s[0] = (DigitizerDecodedBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_FreeBuffer(path, 1, s, scisdk_handle);
        } // DIGITIZER RAW BUFFER
        else if (buffer.get().getClass().equals(DigitizerRawBuffer.class)) {
            DigitizerRawBuffer[] s = new DigitizerRawBuffer[1];
            s[0] = (DigitizerRawBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_FreeBuffer(path, 0, s, scisdk_handle);
        } // LIST RAW BUFFER
        else if (buffer.get().getClass().equals(ListRawBuffer.class)) {
            ListRawBuffer[] s = new ListRawBuffer[1];
            s[0] = (ListRawBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_FreeBuffer(path, 0, s, scisdk_handle);
        } // CUSTOM PACKET DECODED BUFFER
        else if (buffer.get().getClass().equals(CPDecodedBuffer.class)) {
            CPDecodedBuffer[] s = new CPDecodedBuffer[1];
            s[0] = (CPDecodedBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_FreeBuffer(path, 1, s, scisdk_handle);
        } // CUSTOM PACKET RAW BUFFER
        else if (buffer.get().getClass().equals(CPRawBuffer.class)) {
            CPRawBuffer[] s = new CPRawBuffer[1];
            s[0] = (CPRawBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_FreeBuffer(path, 0, s, scisdk_handle);
        } // RATEMETER RAW BUFFER
        else if (buffer.get().getClass().equals(RMRawBuffer.class)) {
            RMRawBuffer[] s = new RMRawBuffer[1];
            s[0] = (RMRawBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_FreeBuffer(path, 0, s, scisdk_handle);
        } // FFT Decoded Buffer
        else if (buffer.get().getClass().equals(FFTDecodedBuffer.class)) {
            FFTDecodedBuffer[] s = new FFTDecodedBuffer[1];
            s[0] = (FFTDecodedBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_FreeBuffer(path, 1, s, scisdk_handle);
        } // FFT Raw Buffer
        else if (buffer.get().getClass().equals(FFTRawBuffer.class)) {
            FFTRawBuffer[] s = new FFTRawBuffer[1];
            s[0] = (FFTRawBuffer) buffer.get();
            return SciSDKLibrary.INSTANCE.SCISDK_FreeBuffer(path, 0, s, scisdk_handle);
        }

        return -1;
    }

    // method used to decode data
    public int DecodeData(String path, Ref buffer_in, Ref buffer_out) {
        Structure.ByReference buf_in = (Structure.ByReference) buffer_in.get();
        Structure.ByReference buf_out = (Structure.ByReference) buffer_out.get();
        int res = SciSDKLibrary.INSTANCE.SCISDK_DecodeData(path, buf_in, buf_out, scisdk_handle);
        buffer_out.set(buf_out);
        return res;
    }

    // method used to read buffer status
    public int ReadStatus(String path, Ref buffer) {
        Structure.ByReference buf = (Structure.ByReference) buffer.get();
        int res = SciSDKLibrary.INSTANCE.SCISDK_ReadStatus(path, buf, scisdk_handle);
        buffer.set(buf);
        return res;
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
    
    public int GetAttachedDevicesList(Ref<String> ret){
        String ret_string[] = new String[1];
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetAttachedDevicesList(ret_string, scisdk_handle);
        ret.set(ret_string[0]);
        return res;
    }
    
    public int GetLibraryVersion(Ref<String> ret){
        String ret_string[] = new String[1];
        int res = SciSDKLibrary.INSTANCE.SCISDK_GetLibraryVersion(ret_string, scisdk_handle);
        ret.set(ret_string[0]);
        return res;
    }
}
