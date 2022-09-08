using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CSharp_SciSDK
{
    public enum ConnectionType : uint
    {
        USB = 0,
        LAN = 1
    }

    public enum SciSDK_DeviceModel : uint
    {
        DT1260 = 0,
        SCIDK = 1,
        DT5550 = 2,
        DT5550W = 3,
        R5560 = 4,
        R5560SE = 5,
        DT5560 = 6,
        V2495 = 7,
        DT2495 = 8,
        V2740 = 9,
        DT2740 = 10
    }

    public class SciSDK_Device
    {
        public static readonly string[] DeviceModelNames = { "dt1260", "scidk", "dt5550", "dt550w", "r5560", "r5560se", "dt5560", "v2495", "dt2495", "v2740", "dt2740" };
        private SciSDK scisdk_handle;
        private string name;
        private SciSDK_DeviceModel model;
        private int serial_number;
        private IPAddress device_ip;
        private int device_port;
        private string json_file_path;
        private ConnectionType connection_type;

        public SciSDK_Device(SciSDK scisdk_handle, string name, SciSDK_DeviceModel model, int serial_number, string json_file_path)
        {
            this.scisdk_handle = scisdk_handle;
            this.name = name;
            this.model = model;
            this.serial_number = serial_number;
            this.json_file_path = json_file_path;
            connection_type = ConnectionType.USB;
        }

        public SciSDK_Device(SciSDK scisdk_handle, string name, SciSDK_DeviceModel model, IPAddress device_ip, int device_port, string json_file_path)
        {
            this.scisdk_handle = scisdk_handle;
            this.name = name;
            this.model = model;
            this.device_ip = device_ip;
            this.device_port = device_port;
            this.json_file_path = json_file_path;
            connection_type = ConnectionType.LAN;
        }

        public int Detach()
        {
            IntPtr name_ptr = Marshal.StringToHGlobalAnsi(name);
            return SciSDK_DLLImport.SCISDK_DetachDevice(name_ptr, scisdk_handle.GetHandle());
        }

        public string GetName()
        {
            return name;
        }

        public IntPtr GetSCISDK_Handle()
        {
            return scisdk_handle.GetHandle();
        }

        // method that returns a node, if exists, that have the name passed as param else it returns null value
        public SciSDK_Node GetNode(string node_name)
        {
            try
            {
                // control if a node with this name exists
                IntPtr board_name_ptr = Marshal.StringToHGlobalAnsi(name);
                IntPtr node_type_ptr = Marshal.StringToHGlobalAnsi("ALL");
                IntPtr ret_ptr = Marshal.StringToHGlobalAnsi("g");
                IntPtr scisdk_handle_ptr = scisdk_handle.GetHandle();
                int res = SciSDK_DLLImport.SCISDK_GetComponentList(board_name_ptr, node_type_ptr, ref ret_ptr, true, scisdk_handle_ptr);
                if (res == 0)
                {
                    string json_string = Marshal.PtrToStringAnsi(ret_ptr);
                    JArray nodes_array = JArray.Parse(json_string);
                    for (int i = 0; i < nodes_array.Count; i++)
                    {
                        JObject current_obj = (JObject)nodes_array[i];
                        if ((string)current_obj["name"] == node_name)
                        {
                            SciSDK_NodeType type = new SciSDK_NodeType();
                            switch ((string)current_obj["type"])
                            {
                                case "Oscilloscope":
                                    type = SciSDK_NodeType.Oscilloscope;
                                    break;
                            }
                            return new SciSDK_Node(this, (string)current_obj["path"], name, type);
                        }
                    }
                }
                return null;
            }
            catch (Exception e)
            {
                return null;
            }
        }


    }
}
