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

        public SciSDK_Device AddDevice(SciSDK_DeviceModel model, string device_path, string json_file_path, string name)
        {
            SciSDK_Device device = null;
            string[] path_vect = device_path.Split(':');
            if (path_vect.Length == 2)
            {
                try
                {
                    if (path_vect[0] == "usb")// if device is connected via USB
                    {
                        int serial_no = Int32.Parse(path_vect[1]);
                        device = new SciSDK_Device(this, name, model, serial_no, json_file_path);
                    }
                    else // if device is connected via LAN
                    {
                        IPAddress device_ip = IPAddress.Parse(path_vect[0]);
                        int port = Int32.Parse(path_vect[1]);
                        device = new SciSDK_Device(this, name, model, device_ip, port, json_file_path);
                    }
                }
                catch (Exception e)
                {
                    return null;
                }

                // check if json file path is valid
                if (!File.Exists(json_file_path))
                {
                    return null;
                }

                // add new device with sciSDK api
                IntPtr device_path_ptr = Marshal.StringToHGlobalAnsi(device_path);
                IntPtr model_ptr = Marshal.StringToHGlobalAnsi(SciSDK_Device.DeviceModelNames[(int)model]);
                IntPtr json_file_path_ptr = Marshal.StringToHGlobalAnsi(json_file_path);
                IntPtr name_ptr = Marshal.StringToHGlobalAnsi(name);

                int res = SciSDK_DLLImport.SCISDK_AddNewDevice(device_path_ptr, model_ptr, json_file_path_ptr, name_ptr, scisdk_handle);
                if (res == 0)
                {
                    return device;
                }
            }
            return null;
        }

        public IntPtr GetHandle()
        {
            return scisdk_handle;
        }
    }
}
