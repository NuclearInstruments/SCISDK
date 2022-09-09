using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CSharp_SciSDK
{
    class SciSDK_DLLImport
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
        public static extern int SCISDK_AllocateBufferSize(IntPtr Path, int buffer_type, ref IntPtr buffer, IntPtr handle, int size);

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

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_GetComponentList(IntPtr Name, IntPtr type, ref IntPtr Ret, bool return_json, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_GetAllParameters(IntPtr Path, IntPtr Ret, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_GetParameterDescription(IntPtr Path, IntPtr Ret, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_GetParameterListOfValues(IntPtr Path, IntPtr Ret, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_GetParameterMinimumValue(IntPtr Path, IntPtr Ret, IntPtr handle);

        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_GetParameterMaximumValue(IntPtr Path, IntPtr Ret, IntPtr handle);
         
        [DllImport("SciSDK_DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SCISDK_GetParametersProperties(IntPtr Path, IntPtr Ret, IntPtr handle);
    }
}
