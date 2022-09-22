using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CSharp_SciSDK
{
    public class SciSDK
    {
        IntPtr scisdk_handle; // pointer to SDK handle object

        public SciSDK()
        {
            scisdk_handle = SciSDK_DLLImport.SCISDK_InitLib();
        }

        public int FreeLib()
        {
            return SciSDK_DLLImport.SCISDK_FreeLib(scisdk_handle);
        }

        public int AddNewDevice(string device_path, string model, string json_file_path, string name)
        {
            IntPtr device_path_ptr = Marshal.StringToHGlobalAnsi(device_path);
            IntPtr model_ptr = Marshal.StringToHGlobalAnsi(model);
            IntPtr json_file_path_ptr = Marshal.StringToHGlobalAnsi(json_file_path);
            IntPtr name_ptr = Marshal.StringToHGlobalAnsi(name);

            int res = SciSDK_DLLImport.SCISDK_AddNewDevice(device_path_ptr, model_ptr, json_file_path_ptr, name_ptr, scisdk_handle);
            return res;
        }

        public int DetachDevice(string board_name)
        {
            IntPtr board_name_ptr = Marshal.StringToHGlobalAnsi(board_name);
            return SciSDK_DLLImport.SCISDK_DetachDevice(board_name_ptr, scisdk_handle);
        }

        public int SetParameter(string path, int value)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            return SciSDK_DLLImport.SCISDK_SetParameterInteger(path_ptr, value, scisdk_handle);
        }

        public int SetParameter(string path, double value)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            return SciSDK_DLLImport.SCISDK_SetParameterDouble(path_ptr, value, scisdk_handle);
        }

        public int SetParameter(string path, string value)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr value_ptr = Marshal.StringToHGlobalAnsi(value);
            return SciSDK_DLLImport.SCISDK_SetParameterString(path_ptr, value_ptr, scisdk_handle);
        }

        public int GetParameter(string path, out int value)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr ret_ptr = Marshal.StringToHGlobalAnsi("g");
            int res = SciSDK_DLLImport.SCISDK_GetParameterInteger(path_ptr, ret_ptr, scisdk_handle);
            value = Marshal.ReadInt32(ret_ptr);
            return res;
        }

        public int GetParameter(string path, out double value)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr ret_ptr = Marshal.StringToHGlobalAnsi("g");
            int res = SciSDK_DLLImport.SCISDK_GetParameterDouble(path_ptr, ret_ptr, scisdk_handle);
            double[] ret_array = new double[1];
            Marshal.Copy(ret_ptr, ret_array, 0, 1);
            value = ret_array[0];
            return res;
        }

        public int GetParameter(string path, out string value)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr ret_ptr = Marshal.StringToHGlobalAnsi("g");
            int res = SciSDK_DLLImport.SCISDK_GetParameterString(path_ptr, ret_ptr, scisdk_handle);
            int addr_tmp = Marshal.ReadInt32(ret_ptr);
            value = Marshal.PtrToStringAnsi(new IntPtr(addr_tmp));
            return res;
        }

        public int ExecuteCommand(string path, string parameter)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr param_ptr = Marshal.StringToHGlobalAnsi(parameter);
            return SciSDK_DLLImport.SCISDK_ExecuteCommand(path_ptr, param_ptr, scisdk_handle);
        }

        public int AllocateBuffer<T>(string Path, BufferType bt, ref T buffer) where T : struct
        {
            int res = 0;
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(Path);

            if (buffer.GetType() == typeof(SciSDKOscilloscopeDecodedBuffer))// oscilloscope decoded
            {
                SciSDKOscilloscopeDecodedBuffer decoded_buffer = (SciSDKOscilloscopeDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                // read informations saved inside buffer
                OscilloscopeDecodedBufferPtr buffer_struct = (OscilloscopeDecodedBufferPtr)Marshal.PtrToStructure(decoded_buffer.buffer_ptr, typeof(OscilloscopeDecodedBufferPtr));
                decoded_buffer.magic = buffer_struct.magic;
                decoded_buffer.info = buffer_struct.info;
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeRawBuffer))// oscilloscope raw
            {
                SciSDKOscilloscopeRawBuffer raw_buffer = (SciSDKOscilloscopeRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                // read informations saved inside buffer
                OscilloscopeRawBufferPtr buffer_struct = (OscilloscopeRawBufferPtr)Marshal.PtrToStructure(raw_buffer.buffer_ptr, typeof(OscilloscopeRawBufferPtr));
                raw_buffer.magic = buffer_struct.magic;
                raw_buffer.info = buffer_struct.info;
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeDualDecodedBuffer)) // oscilloscope dual decoded
            {
                SciSDKOscilloscopeDualDecodedBuffer decoded_buffer = (SciSDKOscilloscopeDualDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                // read informations saved inside buffer
                OscilloscopeDualDecodedBufferPtr buffer_struct = (OscilloscopeDualDecodedBufferPtr)Marshal.PtrToStructure(decoded_buffer.buffer_ptr, typeof(OscilloscopeDualDecodedBufferPtr));
                decoded_buffer.magic = buffer_struct.magic;
                decoded_buffer.info = buffer_struct.info;
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeDualRawBuffer)) // oscilloscope dual raw
            {
                SciSDKOscilloscopeDualRawBuffer raw_buffer = (SciSDKOscilloscopeDualRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                // read informations saved inside buffer
                OscilloscopeDualRawBufferPtr buffer_struct = (OscilloscopeDualRawBufferPtr)Marshal.PtrToStructure(raw_buffer.buffer_ptr, typeof(OscilloscopeDualRawBufferPtr));
                raw_buffer.magic = buffer_struct.magic;
                raw_buffer.info = buffer_struct.info;
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKDigitizerDecodedBuffer)) // digitizer decoded
            {
                SciSDKDigitizerDecodedBuffer decoded_buffer = (SciSDKDigitizerDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                // read informations saved inside buffer
                DigitizerDecodedBufferPtr buffer_struct = (DigitizerDecodedBufferPtr)Marshal.PtrToStructure(decoded_buffer.buffer_ptr, typeof(DigitizerDecodedBufferPtr));
                decoded_buffer.magic = buffer_struct.magic;
                decoded_buffer.info = buffer_struct.info;
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKDigitizerRawBuffer)) // digitizer raw
            {
                SciSDKDigitizerRawBuffer raw_buffer = (SciSDKDigitizerRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                // read informations saved inside buffer
                //DigitizerRawBufferPtr buffer_struct = (DigitizerRawBufferPtr)Marshal.PtrToStructure(raw_buffer.buffer_ptr, typeof(DigitizerRawBufferPtr));
                //raw_buffer.magic = buffer_struct.magic;
                //raw_buffer.info = buffer_struct.info;
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKListRawBuffer)) // list raw
            {
                SciSDKListRawBuffer raw_buffer = (SciSDKListRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                // read informations saved inside buffer
                ListRawBufferPtr buffer_struct = (ListRawBufferPtr)Marshal.PtrToStructure(raw_buffer.buffer_ptr, typeof(ListRawBufferPtr));
                raw_buffer.magic = buffer_struct.magic;
                raw_buffer.info = buffer_struct.info;
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKCPDecodedBuffer)) // custom packet decoded
            {
                SciSDKCPDecodedBuffer decoded_buffer = (SciSDKCPDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                // read informations saved inside buffer
                CPDecodedBufferPtr buffer_struct = (CPDecodedBufferPtr)Marshal.PtrToStructure(decoded_buffer.buffer_ptr, typeof(CPDecodedBufferPtr));
                decoded_buffer.magic = buffer_struct.magic;
                decoded_buffer.info = buffer_struct.info;
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKCPRawBuffer)) // custom packet raw
            {
                SciSDKCPRawBuffer raw_buffer = (SciSDKCPRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                // read informations saved inside buffer
                CPRawBufferPtr buffer_struct = (CPRawBufferPtr)Marshal.PtrToStructure(raw_buffer.buffer_ptr, typeof(CPRawBufferPtr));
                raw_buffer.magic = buffer_struct.magic;
                raw_buffer.info = buffer_struct.info;
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKRMRawBuffer)) // rate meter raw
            {
                SciSDKRMRawBuffer raw_buffer = (SciSDKRMRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                // read informations saved inside buffer
                RMRawBufferPtr buffer_struct = (RMRawBufferPtr)Marshal.PtrToStructure(raw_buffer.buffer_ptr, typeof(RMRawBufferPtr));
                raw_buffer.magic = buffer_struct.magic;
                raw_buffer.info = buffer_struct.info;
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKSpectrumDecodedBuffer)) // spectrum decoded
            {
                SciSDKSpectrumDecodedBuffer decoded_buffer = (SciSDKSpectrumDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                // read informations saved inside buffer
                SpectrumDecodedBufferPtr buffer_struct = (SpectrumDecodedBufferPtr)Marshal.PtrToStructure(decoded_buffer.buffer_ptr, typeof(SpectrumDecodedBufferPtr));
                decoded_buffer.magic = buffer_struct.magic;
                decoded_buffer.info = buffer_struct.info;
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKFFTDecodedBuffer)) // fft decoded
            {
                SciSDKFFTDecodedBuffer decoded_buffer = (SciSDKFFTDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                // read informations saved inside buffer
                SciSDKFFTDecodedBuffer buffer_struct = (SciSDKFFTDecodedBuffer)Marshal.PtrToStructure(decoded_buffer.buffer_ptr, typeof(SciSDKFFTDecodedBuffer));
                decoded_buffer.magic = buffer_struct.magic;
                decoded_buffer.info = buffer_struct.info;
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKFFTRawBuffer)) // fft raw
            {
                SciSDKFFTRawBuffer raw_buffer = (SciSDKFFTRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                // read informations saved inside buffer
                FFTRawBufferPtr buffer_struct = (FFTRawBufferPtr)Marshal.PtrToStructure(raw_buffer.buffer_ptr, typeof(FFTRawBufferPtr));
                raw_buffer.magic = buffer_struct.magic;
                raw_buffer.info = buffer_struct.info;
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }

            return res;
        }

        public int AllocateBuffer<T>(string Path, BufferType bt, ref T buffer, int size) where T : struct
        {
            int res = 0;
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(Path);

            if (buffer.GetType() == typeof(SciSDKOscilloscopeDecodedBuffer))// oscilloscope decoded
            {
                SciSDKOscilloscopeDecodedBuffer decoded_buffer = (SciSDKOscilloscopeDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBufferSize(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle, size);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeRawBuffer))// oscilloscope raw
            {
                SciSDKOscilloscopeRawBuffer raw_buffer = (SciSDKOscilloscopeRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBufferSize(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle, size);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeDualDecodedBuffer)) // oscilloscope dual decoded
            {
                SciSDKOscilloscopeDualDecodedBuffer decoded_buffer = (SciSDKOscilloscopeDualDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBufferSize(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle, size);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeDualRawBuffer)) // oscilloscope dual raw
            {
                SciSDKOscilloscopeDualRawBuffer raw_buffer = (SciSDKOscilloscopeDualRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBufferSize(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle, size);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKDigitizerDecodedBuffer)) // digitizer decoded
            {
                SciSDKDigitizerDecodedBuffer decoded_buffer = (SciSDKDigitizerDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBufferSize(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle, size);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKDigitizerRawBuffer)) // digitizer raw
            {
                SciSDKDigitizerRawBuffer raw_buffer = (SciSDKDigitizerRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBufferSize(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle, size);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKListRawBuffer)) // list raw
            {
                SciSDKListRawBuffer raw_buffer = (SciSDKListRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBufferSize(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle, size);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKCPDecodedBuffer)) // custom packet decoded
            {
                SciSDKCPDecodedBuffer decoded_buffer = (SciSDKCPDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBufferSize(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle, size);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKCPRawBuffer)) // custom packet raw
            {
                SciSDKCPRawBuffer raw_buffer = (SciSDKCPRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBufferSize(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle, size);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKRMRawBuffer)) // rate meter raw
            {
                SciSDKRMRawBuffer raw_buffer = (SciSDKRMRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBufferSize(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle, size);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKSpectrumDecodedBuffer)) // spectrum decoded
            {
                SciSDKSpectrumDecodedBuffer decoded_buffer = (SciSDKSpectrumDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBufferSize(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle, size);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKFFTDecodedBuffer)) // fft decoded
            {
                SciSDKFFTDecodedBuffer decoded_buffer = (SciSDKFFTDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBufferSize(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle, size);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKFFTRawBuffer)) // fft raw
            {
                SciSDKFFTRawBuffer raw_buffer = (SciSDKFFTRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBufferSize(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle, size);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }

            return res;
        }

        public int FreeBuffer<T>(string Path, BufferType bt, ref T buffer) where T : struct
        {
            int res = 0;
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(Path);

            if (buffer.GetType() == typeof(SciSDKOscilloscopeDecodedBuffer))// oscilloscope decoded
            {
                SciSDKOscilloscopeDecodedBuffer decoded_buffer = (SciSDKOscilloscopeDecodedBuffer)(object)buffer;
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeRawBuffer))// oscilloscope raw
            {
                SciSDKOscilloscopeRawBuffer raw_buffer = (SciSDKOscilloscopeRawBuffer)(object)buffer;
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeDualDecodedBuffer)) // oscilloscope dual decoded
            {
                SciSDKOscilloscopeDualDecodedBuffer decoded_buffer = (SciSDKOscilloscopeDualDecodedBuffer)(object)buffer;
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeDualRawBuffer)) // oscilloscope dual raw
            {
                SciSDKOscilloscopeDualRawBuffer raw_buffer = (SciSDKOscilloscopeDualRawBuffer)(object)buffer;
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKDigitizerDecodedBuffer)) // digitizer decoded
            {
                SciSDKDigitizerDecodedBuffer decoded_buffer = (SciSDKDigitizerDecodedBuffer)(object)buffer;
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKDigitizerRawBuffer)) // digitizer raw
            {
                SciSDKDigitizerRawBuffer raw_buffer = (SciSDKDigitizerRawBuffer)(object)buffer;
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKListRawBuffer)) // list raw
            {
                SciSDKListRawBuffer raw_buffer = (SciSDKListRawBuffer)(object)buffer;
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKCPDecodedBuffer)) // custom packet decoded
            {
                SciSDKCPDecodedBuffer decoded_buffer = (SciSDKCPDecodedBuffer)(object)buffer;
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKCPRawBuffer)) // custom packet raw
            {
                SciSDKCPRawBuffer raw_buffer = (SciSDKCPRawBuffer)(object)buffer;
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKRMRawBuffer)) // rate meter raw
            {
                SciSDKRMRawBuffer raw_buffer = (SciSDKRMRawBuffer)(object)buffer;
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKSpectrumDecodedBuffer)) // spectrum decoded
            {
                SciSDKSpectrumDecodedBuffer decoded_buffer = (SciSDKSpectrumDecodedBuffer)(object)buffer;
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKFFTDecodedBuffer)) // fft decoded
            {
                SciSDKFFTDecodedBuffer decoded_buffer = (SciSDKFFTDecodedBuffer)(object)buffer;
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKFFTRawBuffer)) // fft raw
            {
                SciSDKFFTRawBuffer raw_buffer = (SciSDKFFTRawBuffer)(object)buffer;
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }

            return res;
        }

        public int ReadData<T>(string Path, ref T buffer) where T : struct
        {
            int res = 0;
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(Path);
            if (buffer.GetType() == typeof(SciSDKOscilloscopeDecodedBuffer))// oscilloscope decoded
            {
                SciSDKOscilloscopeDecodedBuffer decoded_buffer = (SciSDKOscilloscopeDecodedBuffer)(object)buffer;
                SciSDK_DLLImport.SCISDK_ReadData(path_ptr, decoded_buffer.buffer_ptr, scisdk_handle);
                OscilloscopeDecodedBufferPtr buffer_struct = (OscilloscopeDecodedBufferPtr)Marshal.PtrToStructure(decoded_buffer.buffer_ptr, typeof(OscilloscopeDecodedBufferPtr));
                // copy informations from buffer with IntPtr to buffer without IntPtr
                decoded_buffer.analog = new int[buffer_struct.info.samples_analog * buffer_struct.info.channels];
                Marshal.Copy(buffer_struct.analog, decoded_buffer.analog, 0, decoded_buffer.analog.Length);
                byte[] buf_tmp = new byte[buffer_struct.info.samples_digital * buffer_struct.info.tracks_digital_per_channel];
                Marshal.Copy(buffer_struct.digital, buf_tmp, 0, buf_tmp.Length);
                decoded_buffer.digital = new int[buf_tmp.Length];
                for (int i = 0; i < buf_tmp.Length; i++)
                {
                    decoded_buffer.digital[i] = buf_tmp[i];
                }
                decoded_buffer.info = buffer_struct.info;
                decoded_buffer.magic = buffer_struct.magic;
                decoded_buffer.timecode = buffer_struct.timecode;
                decoded_buffer.trigger_position = buffer_struct.trigger_position;
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeRawBuffer)) // oscilloscope raw
            {
                SciSDKOscilloscopeRawBuffer raw_buffer = (SciSDKOscilloscopeRawBuffer)(object)buffer;
                SciSDK_DLLImport.SCISDK_ReadData(path_ptr, raw_buffer.buffer_ptr, scisdk_handle);
                OscilloscopeRawBufferPtr buffer_struct = (OscilloscopeRawBufferPtr)Marshal.PtrToStructure(raw_buffer.buffer_ptr, typeof(OscilloscopeRawBufferPtr));
                // copy informations from buffer with IntPtr to buffer without IntPtr
                raw_buffer.info = buffer_struct.info;
                raw_buffer.data = new UInt32[raw_buffer.info.buffer_size];
                MarshalUnmananagedArray2Struct(buffer_struct.data, raw_buffer.data.Length, out raw_buffer.data);
                raw_buffer.magic = buffer_struct.magic;
                raw_buffer.timecode = buffer_struct.timecode;
                raw_buffer.trigger_position = buffer_struct.trigger_position;
                raw_buffer.zero_position = buffer_struct.zero_position;
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeDualDecodedBuffer)) // oscilloscope dual decoded
            {
                SciSDKOscilloscopeDualDecodedBuffer decoded_buffer = (SciSDKOscilloscopeDualDecodedBuffer)(object)buffer;
                SciSDK_DLLImport.SCISDK_ReadData(path_ptr, decoded_buffer.buffer_ptr, scisdk_handle);
                OscilloscopeDualDecodedBufferPtr buffer_struct = (OscilloscopeDualDecodedBufferPtr)Marshal.PtrToStructure(decoded_buffer.buffer_ptr, typeof(OscilloscopeDualDecodedBufferPtr));
                // copy informations from buffer with IntPtr to buffer without IntPtr
                decoded_buffer.analog = new int[buffer_struct.info.samples_analog * buffer_struct.info.channels];
                Marshal.Copy(buffer_struct.analog, decoded_buffer.analog, 0, decoded_buffer.analog.Length);
                byte[] buf_tmp = new byte[buffer_struct.info.samples_digital * buffer_struct.info.tracks_digital_per_channel];
                Marshal.Copy(buffer_struct.digital, buf_tmp, 0, buf_tmp.Length);
                decoded_buffer.digital = new int[buf_tmp.Length];
                for (int i = 0; i < buf_tmp.Length; i++)
                {
                    decoded_buffer.digital[i] = buf_tmp[i];
                }
                decoded_buffer.info = buffer_struct.info;
                decoded_buffer.magic = buffer_struct.magic;
                decoded_buffer.timecode = buffer_struct.timecode;
                decoded_buffer.trigger_position = buffer_struct.trigger_position;
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeDualRawBuffer)) // oscilloscope dual raw
            {
                SciSDKOscilloscopeDualRawBuffer raw_buffer = (SciSDKOscilloscopeDualRawBuffer)(object)buffer;
                SciSDK_DLLImport.SCISDK_ReadData(path_ptr, raw_buffer.buffer_ptr, scisdk_handle);
                OscilloscopeDualRawBufferPtr buffer_struct = (OscilloscopeDualRawBufferPtr)Marshal.PtrToStructure(raw_buffer.buffer_ptr, typeof(OscilloscopeDualRawBufferPtr));
                // copy informations from buffer with IntPtr to buffer without IntPtr
                raw_buffer.info = buffer_struct.info;
                raw_buffer.data = new int[raw_buffer.info.buffer_size];
                Marshal.Copy(buffer_struct.data, raw_buffer.data, 0, raw_buffer.data.Length);
                raw_buffer.magic = buffer_struct.magic;
                raw_buffer.timecode = buffer_struct.timecode;
                raw_buffer.trigger_position = buffer_struct.trigger_position;
                raw_buffer.zero_position = buffer_struct.zero_position;
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKDigitizerDecodedBuffer)) // digitizer decoded
            {
                SciSDKDigitizerDecodedBuffer decoded_buffer = (SciSDKDigitizerDecodedBuffer)(object)buffer;
                SciSDK_DLLImport.SCISDK_ReadData(path_ptr, decoded_buffer.buffer_ptr, scisdk_handle);
                DigitizerDecodedBufferPtr buffer_struct = (DigitizerDecodedBufferPtr)Marshal.PtrToStructure(decoded_buffer.buffer_ptr, typeof(DigitizerDecodedBufferPtr));
                // copy informations from buffer with IntPtr to buffer without IntPtr
                decoded_buffer.info = buffer_struct.info;
                decoded_buffer.magic = buffer_struct.magic;
                decoded_buffer.hits = buffer_struct.hits;
                decoded_buffer.timecode = buffer_struct.timecode;
                decoded_buffer.counter = buffer_struct.counter;
                decoded_buffer.user = buffer_struct.user;

                decoded_buffer.analog = new int[buffer_struct.info.samples * buffer_struct.info.channels];
                Marshal.Copy(buffer_struct.analog, decoded_buffer.analog, 0, decoded_buffer.analog.Length);

                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKDigitizerRawBuffer)) // digitizer raw
            {
                SciSDKDigitizerRawBuffer raw_buffer = (SciSDKDigitizerRawBuffer)(object)buffer;

                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKListRawBuffer)) // list raw
            {
                SciSDKListRawBuffer raw_buffer = (SciSDKListRawBuffer)(object)buffer;
                SciSDK_DLLImport.SCISDK_ReadData(path_ptr, raw_buffer.buffer_ptr, scisdk_handle);
                ListRawBufferPtr buffer_struct = (ListRawBufferPtr)Marshal.PtrToStructure(raw_buffer.buffer_ptr, typeof(ListRawBufferPtr));

                raw_buffer.info = buffer_struct.info;
                raw_buffer.magic = buffer_struct.magic;
                raw_buffer.data = new char[raw_buffer.info.buffer_size];
                Marshal.Copy(buffer_struct.data, raw_buffer.data, 0, raw_buffer.data.Length);

                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKCPDecodedBuffer)) // custom packet decoded
            {
                SciSDKCPDecodedBuffer decoded_buffer = (SciSDKCPDecodedBuffer)(object)buffer;
                SciSDK_DLLImport.SCISDK_ReadData(path_ptr, decoded_buffer.buffer_ptr, scisdk_handle);
                CPDecodedBufferPtr buffer_struct = (CPDecodedBufferPtr)Marshal.PtrToStructure(decoded_buffer.buffer_ptr, typeof(CPDecodedBufferPtr));
                // copy informations from buffer with IntPtr to buffer without IntPtr
                decoded_buffer.info = buffer_struct.info;
                decoded_buffer.magic = buffer_struct.magic;
                SciSDKCPPacket[] packet_array = new SciSDKCPPacket[decoded_buffer.info.buffer_size];
                MarshalUnmananagedArray2Struct(buffer_struct.data, (int)decoded_buffer.info.buffer_size, out packet_array);

                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKCPRawBuffer)) // custom packet raw
            {
                SciSDKCPRawBuffer raw_buffer = (SciSDKCPRawBuffer)(object)buffer;
                SciSDK_DLLImport.SCISDK_ReadData(path_ptr, raw_buffer.buffer_ptr, scisdk_handle);
                CPRawBufferPtr buffer_struct = (CPRawBufferPtr)Marshal.PtrToStructure(raw_buffer.buffer_ptr, typeof(CPRawBufferPtr));
                // copy informations from buffer with IntPtr to buffer without IntPtr
                raw_buffer.info = buffer_struct.info;
                raw_buffer.magic = buffer_struct.magic;
                raw_buffer.data = new UInt32[raw_buffer.info.buffer_size];
                MarshalUnmananagedArray2Struct(buffer_struct.data, raw_buffer.data.Length, out raw_buffer.data);

                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKRMRawBuffer)) // rate meter raw
            {
                SciSDKRMRawBuffer raw_buffer = (SciSDKRMRawBuffer)(object)buffer;
                SciSDK_DLLImport.SCISDK_ReadData(path_ptr, raw_buffer.buffer_ptr, scisdk_handle);
                RMRawBufferPtr buffer_struct = (RMRawBufferPtr)Marshal.PtrToStructure(raw_buffer.buffer_ptr, typeof(RMRawBufferPtr));
                // copy informations from buffer with IntPtr to buffer without IntPtr
                raw_buffer.info = buffer_struct.info;
                raw_buffer.magic = buffer_struct.magic;
                raw_buffer.data = new double[raw_buffer.info.buffer_size];
                Marshal.Copy(buffer_struct.data, raw_buffer.data, 0, raw_buffer.data.Length);

                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKSpectrumDecodedBuffer)) // spectrum decoded
            {
                SciSDKSpectrumDecodedBuffer decoded_buffer = (SciSDKSpectrumDecodedBuffer)(object)buffer;
                SciSDK_DLLImport.SCISDK_ReadData(path_ptr, decoded_buffer.buffer_ptr, scisdk_handle);
                SpectrumDecodedBufferPtr buffer_struct = (SpectrumDecodedBufferPtr)Marshal.PtrToStructure(decoded_buffer.buffer_ptr, typeof(SpectrumDecodedBufferPtr));
                // copy informations from buffer with IntPtr to buffer without IntPtr
                decoded_buffer.info = buffer_struct.info;
                decoded_buffer.magic = buffer_struct.magic;
                decoded_buffer.timecode = buffer_struct.timecode;
                decoded_buffer.inttime = buffer_struct.inttime;

                decoded_buffer.data = new UInt32[decoded_buffer.info.buffer_size];
                MarshalUnmananagedArray2Struct(buffer_struct.data, decoded_buffer.data.Length, out decoded_buffer.data);

                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKFFTDecodedBuffer)) // fft decoded
            {
                SciSDKFFTDecodedBuffer decoded_buffer = (SciSDKFFTDecodedBuffer)(object)buffer;
                SciSDK_DLLImport.SCISDK_ReadData(path_ptr, decoded_buffer.buffer_ptr, scisdk_handle);
                FFTDecodedBufferPtr buffer_struct = (FFTDecodedBufferPtr)Marshal.PtrToStructure(decoded_buffer.buffer_ptr, typeof(FFTDecodedBufferPtr));
                // copy informations from buffer with IntPtr to buffer without IntPtr
                decoded_buffer.info = buffer_struct.info;
                decoded_buffer.magic = buffer_struct.magic;
                decoded_buffer.timecode = buffer_struct.timecode;
                decoded_buffer.mag = new double[decoded_buffer.info.channels * decoded_buffer.info.samples];
                Marshal.Copy(buffer_struct.mag, decoded_buffer.mag, 0, decoded_buffer.mag.Length);
                decoded_buffer.ph = new double[decoded_buffer.info.channels * decoded_buffer.info.samples];
                Marshal.Copy(buffer_struct.ph, decoded_buffer.ph, 0, decoded_buffer.ph.Length);

                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKFFTRawBuffer)) // fft raw
            {
                SciSDKFFTRawBuffer raw_buffer = (SciSDKFFTRawBuffer)(object)buffer;
                SciSDK_DLLImport.SCISDK_ReadData(path_ptr, raw_buffer.buffer_ptr, scisdk_handle);
                FFTRawBufferPtr buffer_struct = (FFTRawBufferPtr)Marshal.PtrToStructure(raw_buffer.buffer_ptr, typeof(FFTRawBufferPtr));
                // copy informations from buffer with IntPtr to buffer without IntPtr
                raw_buffer.info = buffer_struct.info;
                raw_buffer.magic = buffer_struct.magic;
                raw_buffer.timecode = buffer_struct.timecode;
                raw_buffer.data = new UInt32[buffer_struct.info.buffer_size];
                MarshalUnmananagedArray2Struct(raw_buffer.buffer_ptr, raw_buffer.data.Length, out raw_buffer.data);

                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }

            return res;
        }

        public int SetRegister(string path, int value)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            return SciSDK_DLLImport.SCISDK_SetRegister(path_ptr, value, scisdk_handle);
        }

        public int GetRegister(string path, out int value)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr value_ptr = Marshal.StringToHGlobalAnsi("g");
            int res = SciSDK_DLLImport.SCISDK_GetRegister(path_ptr, value_ptr, scisdk_handle);
            value = Marshal.ReadInt32(value_ptr);
            return res;
        }

        public int DecodeData<T>(string path, object buffer_in, ref T buffer_out) where T : struct
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr buf_in_ptr = Marshal.AllocHGlobal(Marshal.SizeOf(buffer_in));
            IntPtr buf_out_ptr = Marshal.AllocHGlobal(Marshal.SizeOf(buffer_out));
            Marshal.StructureToPtr(buffer_in, buf_in_ptr, false);
            int res = SciSDK_DLLImport.SCISDK_DecodeData(path_ptr, buf_in_ptr, buf_out_ptr, scisdk_handle);

            if (buffer_out.GetType() == typeof(OscilloscopeDecodedBufferPtr))
            {
                buffer_out = (T)Convert.ChangeType((OscilloscopeDecodedBufferPtr)Marshal.PtrToStructure(buf_out_ptr, typeof(OscilloscopeDecodedBufferPtr)), typeof(T));
            }
            else if (buffer_out.GetType() == typeof(SciSDKOscilloscopeRawBuffer))// oscilloscope raw
            {
                buffer_out = (T)Convert.ChangeType((SciSDKOscilloscopeRawBuffer)Marshal.PtrToStructure(buf_out_ptr, typeof(SciSDKOscilloscopeRawBuffer)), typeof(T));
            }
            else if (buffer_out.GetType() == typeof(SciSDKOscilloscopeDualDecodedBuffer)) // oscilloscope dual decoded
            {
                buffer_out = (T)Convert.ChangeType((SciSDKOscilloscopeDualDecodedBuffer)Marshal.PtrToStructure(buf_out_ptr, typeof(SciSDKOscilloscopeDualDecodedBuffer)), typeof(T));
            }
            else if (buffer_out.GetType() == typeof(SciSDKOscilloscopeDualRawBuffer)) // oscilloscope dual raw
            {
                buffer_out = (T)Convert.ChangeType((SciSDKOscilloscopeDualRawBuffer)Marshal.PtrToStructure(buf_out_ptr, typeof(SciSDKOscilloscopeDualRawBuffer)), typeof(T));
            }
            else if (buffer_out.GetType() == typeof(SciSDKDigitizerDecodedBuffer)) // digitizer decoded
            {
                buffer_out = (T)Convert.ChangeType((SciSDKDigitizerDecodedBuffer)Marshal.PtrToStructure(buf_out_ptr, typeof(SciSDKDigitizerDecodedBuffer)), typeof(T));
            }
            else if (buffer_out.GetType() == typeof(SciSDKDigitizerRawBuffer)) // digitizer raw
            {
                buffer_out = (T)Convert.ChangeType((SciSDKDigitizerRawBuffer)Marshal.PtrToStructure(buf_out_ptr, typeof(SciSDKDigitizerRawBuffer)), typeof(T));
            }
            else if (buffer_out.GetType() == typeof(SciSDKListRawBuffer)) // list raw
            {
                buffer_out = (T)Convert.ChangeType((SciSDKListRawBuffer)Marshal.PtrToStructure(buf_out_ptr, typeof(SciSDKListRawBuffer)), typeof(T));
            }
            else if (buffer_out.GetType() == typeof(SciSDKCPDecodedBuffer)) // custom packet decoded
            {
                buffer_out = (T)Convert.ChangeType((SciSDKCPDecodedBuffer)Marshal.PtrToStructure(buf_out_ptr, typeof(SciSDKCPDecodedBuffer)), typeof(T));
            }
            else if (buffer_out.GetType() == typeof(SciSDKCPRawBuffer)) // custom packet raw
            {
                buffer_out = (T)Convert.ChangeType((SciSDKCPRawBuffer)Marshal.PtrToStructure(buf_out_ptr, typeof(SciSDKCPRawBuffer)), typeof(T));
            }
            else if (buffer_out.GetType() == typeof(SciSDKRMRawBuffer)) // rate meter raw
            {
                buffer_out = (T)Convert.ChangeType((SciSDKRMRawBuffer)Marshal.PtrToStructure(buf_out_ptr, typeof(SciSDKRMRawBuffer)), typeof(T));
            }
            else if (buffer_out.GetType() == typeof(SciSDKSpectrumDecodedBuffer)) // spectrum decoded
            {
                buffer_out = (T)Convert.ChangeType((SciSDKSpectrumDecodedBuffer)Marshal.PtrToStructure(buf_out_ptr, typeof(SciSDKSpectrumDecodedBuffer)), typeof(T));
            }
            else if (buffer_out.GetType() == typeof(SciSDKFFTDecodedBuffer)) // fft decoded
            {
                buffer_out = (T)Convert.ChangeType((SciSDKFFTDecodedBuffer)Marshal.PtrToStructure(buf_out_ptr, typeof(SciSDKFFTDecodedBuffer)), typeof(T));
            }
            else if (buffer_out.GetType() == typeof(SciSDKFFTRawBuffer)) // fft raw
            {
                buffer_out = (T)Convert.ChangeType((SciSDKFFTRawBuffer)Marshal.PtrToStructure(buf_out_ptr, typeof(SciSDKFFTRawBuffer)), typeof(T));
            }

            Marshal.FreeHGlobal(buf_in_ptr);
            Marshal.FreeHGlobal(buf_out_ptr);
            return res;
        }

        public int s_error(int error, out string value)
        {
            IntPtr value_ptr = Marshal.StringToHGlobalAnsi("g");
            int res = SciSDK_DLLImport.SCISDK_s_error(error, value_ptr, scisdk_handle);
            int addr_tmp = Marshal.ReadInt32(value_ptr);
            value = Marshal.PtrToStringAnsi(new IntPtr(addr_tmp));
            return res;
        }

        public int GetAllParameters(string path, out string ret)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr ret_ptr = Marshal.StringToHGlobalAnsi("g");
            int res = SciSDK_DLLImport.SCISDK_GetAllParameters(path_ptr, ret_ptr, scisdk_handle);
            int addr_tmp = Marshal.ReadInt32(ret_ptr);
            ret = Marshal.PtrToStringAnsi(new IntPtr(addr_tmp));
            return res;
        }

        public int GetParameterDescription(string path, out string ret)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr ret_ptr = Marshal.StringToHGlobalAnsi("g");
            int res = SciSDK_DLLImport.SCISDK_GetParameterDescription(path_ptr, ret_ptr, scisdk_handle);
            int addr_tmp = Marshal.ReadInt32(ret_ptr);
            ret = Marshal.PtrToStringAnsi(new IntPtr(addr_tmp));
            return res;
        }

        public int GetComponentList(string board_name, string type, out string ret, bool return_json)
        {
            IntPtr board_name_ptr = Marshal.StringToHGlobalAnsi(board_name);
            IntPtr node_type_ptr = Marshal.StringToHGlobalAnsi(type);
            IntPtr ret_ptr = Marshal.StringToHGlobalAnsi("g");
            int res = SciSDK_DLLImport.SCISDK_GetComponentList(board_name_ptr, node_type_ptr, ref ret_ptr, true, scisdk_handle);
            int addr_tmp = Marshal.ReadInt32(ret_ptr);
            ret = Marshal.PtrToStringAnsi(new IntPtr(addr_tmp));
            return res;
        }

        public int GetParameterMaximumValue(string path, out double ret)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr ret_ptr = Marshal.StringToHGlobalAnsi("g");
            int res = SciSDK_DLLImport.SCISDK_GetParameterMaximumValue(path_ptr, ret_ptr, scisdk_handle);
            double[] ret_array = new double[1];
            Marshal.Copy(ret_ptr, ret_array, 0, 1);
            ret = ret_array[0];
            return res;
        }

        public int GetParameterMinimumValue(string path, out double ret)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr ret_ptr = Marshal.StringToHGlobalAnsi("g");
            int res = SciSDK_DLLImport.SCISDK_GetParameterMinimumValue(path_ptr, ret_ptr, scisdk_handle);
            double[] ret_array = new double[1];
            Marshal.Copy(ret_ptr, ret_array, 0, 1);
            ret = ret_array[0];
            return res;
        }

        public int GetParametersProperties(string path, out string ret)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr ret_ptr = Marshal.StringToHGlobalAnsi("g");
            int res = SciSDK_DLLImport.SCISDK_GetParametersProperties(path_ptr, ret_ptr, scisdk_handle);
            int addr_tmp = Marshal.ReadInt32(ret_ptr);
            ret = Marshal.PtrToStringAnsi(new IntPtr(addr_tmp));
            return res;
        }

        private static void MarshalUnmananagedArray2Struct<T>(IntPtr unmanagedArray, int length, out T[] mangagedArray)
        {
            var size = Marshal.SizeOf(typeof(T));
            mangagedArray = new T[length];
            for (int i = 0; i < length; i++)
            {
                IntPtr ins = new IntPtr(unmanagedArray.ToInt64() + i * size);
                mangagedArray[i] = Marshal.PtrToStructure<T>(ins);
            }
        }


        public IntPtr GetHandle()
        {
            return scisdk_handle;
        }

        // define buffer used for read data
        // oscilloscope buffer ptr struct
        [Serializable]
        [StructLayout(LayoutKind.Sequential)]
        private struct OscilloscopeDecodedBufferPtr
        {
            public UInt32 magic;
            public IntPtr analog;
            public IntPtr digital;
            public UInt32 trigger_position;
            public UInt64 timecode;
            public SciSDKOscilloscopeDecodedBufferInfo info;
        }

        // oscilloscope buffer ptr struct
        [Serializable]
        [StructLayout(LayoutKind.Sequential)]
        private struct OscilloscopeRawBufferPtr
        {
            public UInt32 magic;
            public IntPtr data;
            public UInt32 zero_position;
            public UInt32 trigger_position;
            public UInt64 timecode;
            public SciSDKOscilloscopeRawBufferInfo info;
        }

        // oscilloscope dual decoded buffer
        [Serializable]
        [StructLayout(LayoutKind.Sequential)]
        private struct OscilloscopeDualDecodedBufferPtr
        {
            public UInt32 magic;
            public IntPtr analog;
            public IntPtr digital;
            public UInt32 trigger_position;
            public UInt64 timecode;
            public SciSDKOscilloscopeDualDecodedBufferInfo info;
        }

        // oscilloscope dual raw buffer
        [Serializable]
        [StructLayout(LayoutKind.Sequential)]
        private struct OscilloscopeDualRawBufferPtr
        {
            public UInt32 magic;
            public IntPtr data;
            public UInt32 zero_position;
            public UInt32 trigger_position;
            public UInt64 timecode;
            public SciSDKOscilloscopeDualRawBufferInfo info;
        }

        // digitizer decoded buffer
        private struct DigitizerDecodedBufferPtr
        {
            public UInt32 magic;
            public IntPtr analog;
            public UInt64 hits;
            public UInt64 timecode;
            public UInt32 counter;
            public UInt32 user;
            public SciSDKDigitizerDecodedBufferInfo info;
        }

        // list raw buffer
        public struct ListRawBufferPtr
        {
            public UInt32 magic;
            public IntPtr data;
            public SciSDKListRawBufferInfo info;
        }

        // custom packet decoded
        public struct CPDecodedBufferPtr
        {
            public UInt32 magic;
            public IntPtr data;
            public SciSDKCPDecodedBufferInfo info;
        }

        // custom packet raw
        public struct CPRawBufferPtr
        {
            public UInt32 magic;
            public IntPtr data;
            public SciSDKCPRawBufferInfo info;
        }

        // ratemeter raw
        public struct RMRawBufferPtr
        {
            public UInt32 magic;
            public IntPtr data;
            public SciSDKRMRawBufferInfo info;
        }

        // spectrum decoded buffer
        public struct SpectrumDecodedBufferPtr
        {
            public UInt32 magic;
            public IntPtr data;
            public UInt64 timecode;
            public UInt32 inttime;
            public SciSDKSpectrumDecodedBufferInfo info;
        }

        // fft decoded buffer
        public struct FFTDecodedBufferPtr
        {
            public UInt32 magic;
            public IntPtr mag;
            public IntPtr ph;
            public UInt64 timecode;
            public SciSDKFFTDecodedBufferInfo info;
        }

        // fft raw buffer
        public struct FFTRawBufferPtr
        {
            public UInt32 magic;
            public IntPtr data;
            public UInt64 timecode;
            public SciSDKFFTRawBufferInfo info;
        }
    }
}
