using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Oscilloscope
{
    public class SciSdk_Wrapper
    {
        public const int T_BUFFER_RAW = 0;
        public const int T_BUFFER_DECODED = 1;

        // method used to set a string parameter with scisdk
        public static bool SetParamString(string path, string value, IntPtr scisdk_handle)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr value_ptr = Marshal.StringToHGlobalAnsi(value);//initialize pointer
            if (SciSDK.SCISDK_SetParameterString(path_ptr, value_ptr, scisdk_handle) == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        // method used to set an int parameter with scisdk
        public static bool SetParamInt(string path, int value, IntPtr scisdk_handle)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            if (SciSDK.SCISDK_SetParameterInteger(path_ptr, value, scisdk_handle) == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        // method used to get an int type param from scisdk
        public static bool GetParamInt(string path, out int value, IntPtr scisdk_handle)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr ret_value_ptr = Marshal.StringToHGlobalAnsi("t");//initialize pointer
            if (SciSDK.SCISDK_GetParameterInteger(path_ptr, ret_value_ptr, scisdk_handle) == 0)
            {
                value = Marshal.ReadInt32(ret_value_ptr);// read int value stored at position specified by pointer
                return true;
            }
            else
            {
                value = 0;
                return false;
            }
        }

        // method used to allocate buffer
        public static bool AllocateBuffer(string path, int buffer_type, ref IntPtr buffer, IntPtr scisdk_handle)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            if (SciSDK.SCISDK_AllocateBuffer(path_ptr, buffer_type, ref buffer, scisdk_handle) == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        // method used to clear buffer
        public static bool FreeBuffer(string path, int buffer_type, ref IntPtr buffer, IntPtr scisdk_handle)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            if (SciSDK.SCISDK_FreeBuffer(path_ptr, buffer_type, ref buffer, scisdk_handle) == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        // method used to execute command
        public static bool ExecuteCommand(string path, string param, IntPtr scisdk_handle)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            IntPtr param_ptr = Marshal.StringToHGlobalAnsi(param);
            if (SciSDK.SCISDK_ExecuteCommand(path_ptr, param_ptr, scisdk_handle) == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        // method used to read data from board
        public static bool ReadData(string path, ref IntPtr buffer, IntPtr scisdk_handle)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path);
            if (SciSDK.SCISDK_ReadData(path_ptr, buffer, scisdk_handle) == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

    }
}
