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
        private readonly string[] DeviceModelNames = { "dt1260", "scidk", "dt5550", "dt550w", "r5560", "r5560se", "dt5560", "v2495", "dt2495", "v2740", "dt2740" };

        public SciSDK()
        {
            scisdk_handle = SciSDK_DLLImport.SCISDK_InitLib();
        }

        public int AddDevice(SciSDK_DeviceModel model, string device_path, string json_file_path, string name)
        {
            IntPtr device_path_ptr = Marshal.StringToHGlobalAnsi(device_path);
            IntPtr model_ptr = Marshal.StringToHGlobalAnsi(DeviceModelNames[(int)model]);
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
            int res = SciSDK_DLLImport.SCISDK_GetParameterInteger(path_ptr, ret_ptr, scisdk_handle);
            double[] ret_array = new double[1];
            Marshal.Copy(ret_ptr, ret_array, 0, 1);
            value = ret_array[0];
            return res;
        }

        public int GetParameter(string path, out string value)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr ret_ptr = Marshal.StringToHGlobalAnsi("g");
            int res = SciSDK_DLLImport.SCISDK_GetParameterInteger(path_ptr, ret_ptr, scisdk_handle);
            value = Marshal.PtrToStringAnsi(ret_ptr);
            return res;
        }

        int ExecuteCommand(string path, string parameter)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr param_ptr = Marshal.StringToHGlobalAnsi(parameter);
            return SciSDK_DLLImport.SCISDK_ExecuteCommand(path_ptr, param_ptr, scisdk_handle);
        }

        public int AllocateBuffer<T>(string Path, Buffer_Type bt, ref T buffer) where T : struct
        {
            int res = 0;
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(Path);

            if (buffer.GetType() == typeof(SciSDKOscilloscopeDecodedBuffer))// oscilloscope decoded
            {
                SciSDKOscilloscopeDecodedBuffer decoded_buffer = (SciSDKOscilloscopeDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeRawBuffer))// oscilloscope raw
            {
                SciSDKOscilloscopeRawBuffer raw_buffer = (SciSDKOscilloscopeRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeDualDecodedBuffer)) // oscilloscope dual decoded
            {
                SciSDKOscilloscopeDualDecodedBuffer decoded_buffer = (SciSDKOscilloscopeDualDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeDualRawBuffer)) // oscilloscope dual raw
            {
                SciSDKOscilloscopeDualRawBuffer raw_buffer = (SciSDKOscilloscopeDualRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKDigitizerDecodedBuffer)) // digitizer decoded
            {
                SciSDKDigitizerDecodedBuffer decoded_buffer = (SciSDKDigitizerDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKDigitizerRawBuffer)) // digitizer raw
            {
                SciSDKDigitizerRawBuffer raw_buffer = (SciSDKDigitizerRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKListRawBuffer)) // list raw
            {
                SciSDKListRawBuffer raw_buffer = (SciSDKListRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKCPDecodedBuffer)) // custom packet decoded
            {
                SciSDKCPDecodedBuffer decoded_buffer = (SciSDKCPDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKCPRawBuffer)) // custom packet raw
            {
                SciSDKCPRawBuffer raw_buffer = (SciSDKCPRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKRMRawBuffer)) // rate meter raw
            {
                SciSDKRMRawBuffer raw_buffer = (SciSDKRMRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKSpectrumDecodedBuffer)) // spectrum decoded
            {
                SciSDKSpectrumDecodedBuffer decoded_buffer = (SciSDKSpectrumDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKFFTDecodedBuffer)) // fft decoded
            {
                SciSDKFFTDecodedBuffer decoded_buffer = (SciSDKFFTDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKFFTRawBuffer)) // fft raw
            {
                SciSDKFFTRawBuffer raw_buffer = (SciSDKFFTRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_AllocateBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }

            return res;
        }

        public int AllocateBuffer<T>(string Path, Buffer_Type bt, ref object buffer, int size) where T : struct
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


        public int FreeBuffer<T>(string Path, Buffer_Type bt, ref object buffer) where T : struct
        {
            int res = 0;
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(Path);

            if (buffer.GetType() == typeof(SciSDKOscilloscopeDecodedBuffer))// oscilloscope decoded
            {
                SciSDKOscilloscopeDecodedBuffer decoded_buffer = (SciSDKOscilloscopeDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeRawBuffer))// oscilloscope raw
            {
                SciSDKOscilloscopeRawBuffer raw_buffer = (SciSDKOscilloscopeRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeDualDecodedBuffer)) // oscilloscope dual decoded
            {
                SciSDKOscilloscopeDualDecodedBuffer decoded_buffer = (SciSDKOscilloscopeDualDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKOscilloscopeDualRawBuffer)) // oscilloscope dual raw
            {
                SciSDKOscilloscopeDualRawBuffer raw_buffer = (SciSDKOscilloscopeDualRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKDigitizerDecodedBuffer)) // digitizer decoded
            {
                SciSDKDigitizerDecodedBuffer decoded_buffer = (SciSDKDigitizerDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKDigitizerRawBuffer)) // digitizer raw
            {
                SciSDKDigitizerRawBuffer raw_buffer = (SciSDKDigitizerRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKListRawBuffer)) // list raw
            {
                SciSDKListRawBuffer raw_buffer = (SciSDKListRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKCPDecodedBuffer)) // custom packet decoded
            {
                SciSDKCPDecodedBuffer decoded_buffer = (SciSDKCPDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKCPRawBuffer)) // custom packet raw
            {
                SciSDKCPRawBuffer raw_buffer = (SciSDKCPRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKRMRawBuffer)) // rate meter raw
            {
                SciSDKRMRawBuffer raw_buffer = (SciSDKRMRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref raw_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKSpectrumDecodedBuffer)) // spectrum decoded
            {
                SciSDKSpectrumDecodedBuffer decoded_buffer = (SciSDKSpectrumDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKFFTDecodedBuffer)) // fft decoded
            {
                SciSDKFFTDecodedBuffer decoded_buffer = (SciSDKFFTDecodedBuffer)(object)buffer;
                decoded_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
                res = SciSDK_DLLImport.SCISDK_FreeBuffer(path_ptr, (int)bt, ref decoded_buffer.buffer_ptr, scisdk_handle);
                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKFFTRawBuffer)) // fft raw
            {
                SciSDKFFTRawBuffer raw_buffer = (SciSDKFFTRawBuffer)(object)buffer;
                raw_buffer.buffer_ptr = Marshal.StringToHGlobalAnsi("g");
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
                decoded_buffer.analog = new int[buffer_struct.info.samples_analog];
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
                raw_buffer.data = new int[raw_buffer.info.buffer_size];
                Marshal.Copy(buffer_struct.data, raw_buffer.data, 0, raw_buffer.data.Length);
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
                decoded_buffer.analog = new int[buffer_struct.info.samples_analog];
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

                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKCPDecodedBuffer)) // custom packet decoded
            {
                SciSDKCPDecodedBuffer decoded_buffer = (SciSDKCPDecodedBuffer)(object)buffer;

                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKCPRawBuffer)) // custom packet raw
            {
                SciSDKCPRawBuffer raw_buffer = (SciSDKCPRawBuffer)(object)buffer;

                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKRMRawBuffer)) // rate meter raw
            {
                SciSDKRMRawBuffer raw_buffer = (SciSDKRMRawBuffer)(object)buffer;

                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKSpectrumDecodedBuffer)) // spectrum decoded
            {
                SciSDKSpectrumDecodedBuffer decoded_buffer = (SciSDKSpectrumDecodedBuffer)(object)buffer;

                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKFFTDecodedBuffer)) // fft decoded
            {
                SciSDKFFTDecodedBuffer decoded_buffer = (SciSDKFFTDecodedBuffer)(object)buffer;

                buffer = (T)Convert.ChangeType(decoded_buffer, typeof(T));
            }
            else if (buffer.GetType() == typeof(SciSDKFFTRawBuffer)) // fft raw
            {
                SciSDKFFTRawBuffer raw_buffer = (SciSDKFFTRawBuffer)(object)buffer;

                buffer = (T)Convert.ChangeType(raw_buffer, typeof(T));
            }

            return res;
        }

        //int DecodeData(string Path, object buffer_in, object buffer_out)
        //{

        //}

        public int GetComponentList(string board_name, string type, out string ret, bool return_json)
        {
            IntPtr board_name_ptr = Marshal.StringToHGlobalAnsi(board_name);
            IntPtr node_type_ptr = Marshal.StringToHGlobalAnsi(type);
            IntPtr ret_ptr = Marshal.StringToHGlobalAnsi("g");
            int res = SciSDK_DLLImport.SCISDK_GetComponentList(board_name_ptr, node_type_ptr, ref ret_ptr, true, scisdk_handle);
            ret = Marshal.PtrToStringAnsi(ret_ptr);
            return res;
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
    }
}
