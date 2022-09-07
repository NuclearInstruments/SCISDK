using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Console
{
    class SciSDK
    {
        // import all methods from SCISDK dll file

        [DllImport("SciSDK_DLL.dll")]
        public static extern IntPtr SCISDK_InitLib();

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_FreeLib(IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_AddNewDevice(IntPtr DevicePath, IntPtr DeviceModel, IntPtr JSONFwFilePath, IntPtr Name, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_DetachDevice(IntPtr name, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_SetParameterString(IntPtr Path, IntPtr value, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_SetParameterInteger(IntPtr Path, int value, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_SetParameterDouble(IntPtr Path, double value, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_GetParameterString(IntPtr Path, IntPtr value, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_GetParameterInteger(IntPtr Path, IntPtr value, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_GetParameterDouble(IntPtr Path, IntPtr value, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_DecodeData(IntPtr Path, IntPtr buffer_in, IntPtr buffer_out, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_AllocateBuffer(IntPtr Path, int buffer_type, ref IntPtr buffer, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_SetRegister(IntPtr Path, int value, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_GetRegister(IntPtr Path, IntPtr value, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_FreeBuffer(IntPtr Path, int buffer_type, ref IntPtr buffer, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_ReadData(IntPtr Path, IntPtr buffer, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_s_error(int err_no, IntPtr value, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_ExecuteCommand(IntPtr Path, IntPtr value, IntPtr handle);
    }


    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
    struct Oscilloscope_decoded_buffer_struct
    {
        public UInt32 magic;
        public IntPtr analog;
        public IntPtr digital;
        public UInt32 trigger_position;
        public UInt64 timecode;
        public InfoStruct info;
    }

    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
    struct InfoStruct
    {
        public UInt32 samples_analog;
        public UInt32 samples_digital;
        public UInt32 tracks_analog_per_channel;
        public UInt32 tracks_digital_per_channel;
        public UInt32 channels;
    }
}
