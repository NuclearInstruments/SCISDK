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
    }
}
