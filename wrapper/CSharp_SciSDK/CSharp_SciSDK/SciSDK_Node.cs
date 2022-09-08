using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CSharp_SciSDK
{
    public enum SciSDK_NodeType : uint
    {
        CustomPacket = 0,
        Digitizer = 1,
        FFT = 2,
        List = 3,
        Oscilloscope = 4,
        OscilloscopeDual = 5,
        RateMeter = 6,
        Register = 7,
        RegisterFile = 8,
        Spectrum = 9,
    }

    public class SciSDK_Node
    {
        private string name;
        private SciSDK_NodeType node_type;
        private string path;
        private SciSDK_Device device;

        public SciSDK_Node(SciSDK_Device device,string path, string name, SciSDK_NodeType node_type)
        {
            this.device = device;
            this.path = path;
            this.name = name;
            this.node_type = node_type;
        }

        // set params methods
        public virtual int SetParam(string param, int value)
        {
            return 0;
        }
        public virtual int SetParam(string param, uint value)
        {
            return 0;
        }
        public virtual int SetParam(string param, double value)
        {
            return 0;
        }
        public virtual int SetParam(string param, string value)
        {
            return 0;
        }

        // get params methods
        public virtual int GetParam(string param, out int value)
        {
            value = 0;
            return 0;
        }
        public virtual int GetParam(string param, out uint value)
        {
            value = 0;
            return 0;
        }
        public virtual int GetParam(string param, out double value)
        {
            value = 0;
            return 0;
        }
        public virtual int GetParam(string param, out string value)
        {
            value = "";
            return 0;
        }

        // method that return the description of a parameter
        public int GetParameterDescription(string param, out string value)
        {
            IntPtr path_ptr = Marshal.StringToHGlobalAnsi(path + "." + param);
            IntPtr value_ptr = Marshal.StringToHGlobalAnsi("g");
            int res = SciSDK_DLLImport.SCISDK_GetParameterDescription(path_ptr, value_ptr, device.GetSCISDK_Handle());
            value = Marshal.PtrToStringAnsi(value_ptr);
            return res;
        }
        //NI_RESULT GetParameterListOfValues(string path, string* ret);
        //NI_RESULT GetParameterMinimumValue(string path, double* ret);
        //NI_RESULT GetParameterMaximumValue(string path, double* ret);
        //NI_RESULT GetParametersProperties(string path, string* ret);
    }

    
}
