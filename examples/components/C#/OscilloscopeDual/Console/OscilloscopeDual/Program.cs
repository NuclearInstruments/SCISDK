using CSharp_SciSDK;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OscilloscopeDual
{
    class Program
    {
        static void Main(string[] args)
        {
            SciSDK sdk = new SciSDK();

            // add new device
            int res = sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0");

            if (res == 0)
            {
                // set board parameters
                sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self");
                sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_level", 3000);
                sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0);
                sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150);
                sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.decimator", 0);
                sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode");
                // sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "raw");
                sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking");
                sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.timeout", 1000);

                // allocate buffer
                SciSDKOscilloscopeDualDecodedBuffer buf = new SciSDKOscilloscopeDualDecodedBuffer();
                res = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", BufferType.BUFFER_TYPE_DECODED, ref buf);

                if (res == 0)
                {
                    sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", "");

                    // read data
                    res = sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", ref buf);
                    if (res == 0)
                    {
                        // read channels and store data in one file for each channel
                        for (int i = 0; i < buf.info.channels; i++)
                        {
                            string str_tmp = "";
                            for (int j = 0; j < buf.info.samples_analog; j++)
                            {
                                str_tmp += buf.analog[j + i * buf.info.samples_analog] + "\n";
                            }
                            File.WriteAllText("analog" + i + ".txt", str_tmp);
                        }
                    }
                    else
                    {
                        // print error message
                        string ret_str = "";
                        sdk.s_error(res, out ret_str);
                        Console.WriteLine("Read data error" + ret_str);
                    }
                }
                else
                {
                    // print error message
                    string ret_str = "";
                    sdk.s_error(res, out ret_str);
                    Console.WriteLine("Allocate buffer error " + ret_str);
                }
                // detach device
                sdk.DetachDevice("board0");
            }
            else
            {
                // print error message
                string ret_str = "";
                sdk.s_error(res, out ret_str);
                Console.WriteLine("Connection error " + ret_str);
            }

            System.Console.WriteLine("Click enter to close console");
            System.Console.ReadLine();
        }
    }
}
