using CSharp_SciSDK;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Console
{
    class Program
    {
        static void Main(string[] args)
        {
            // init library
            SciSDK sdk = new SciSDK();

            // add new device
            sdk.AddNewDevice("usb:13250", "dt1260", "oscilloscope.json", "board0");

            // set parameters
            string oscilloscope_base_path = "board0:/MMCComponents/Oscilloscope_0";
            sdk.SetParameter(oscilloscope_base_path + ".decimator", 0);
            sdk.SetParameter(oscilloscope_base_path + ".trigger_mode", "self");
            sdk.SetParameter(oscilloscope_base_path + ".trigger_level", 3000);
            sdk.SetParameter(oscilloscope_base_path + ".pretrigger", 150);
            sdk.SetParameter(oscilloscope_base_path + ".trigger_channel", 0);
            sdk.SetParameter(oscilloscope_base_path + ".data_processing", "decode");
            sdk.SetParameter(oscilloscope_base_path + ".acq_mode", "blocking");
            sdk.SetParameter(oscilloscope_base_path + ".timeout", 1000);

            // allocate buffer
            SciSDKOscilloscopeDecodedBuffer odb = new SciSDKOscilloscopeDecodedBuffer();
            if (sdk.AllocateBuffer(oscilloscope_base_path, BufferType.BUFFER_TYPE_DECODED, ref odb) == 0)
            {
                sdk.ExecuteCommand(oscilloscope_base_path + ".reset_read_valid_flag", "");

                // read data from board
                if (sdk.ReadData(oscilloscope_base_path, ref odb) == 0)
                {
                 
                    string str_tmp = "";
                    // save analog values inside a file
                    foreach (int value in odb.analog)
                    {
                        str_tmp += value.ToString() + "\n";
                    }
                    File.WriteAllText("analog.txt", str_tmp);
                    System.Console.WriteLine("Analog value saved inside " + Directory.GetCurrentDirectory() + "\\analog.txt");
                    
                    str_tmp = "";
                    int n_track = 1;// number of the track
                    // read data from digital trace 0 and save them in a file
                    for (int i = (int)odb.info.samples_digital * (n_track - 1); i < odb.info.samples_digital * n_track; i++)
                    {
                        str_tmp += odb.digital[i].ToString() + " \n";
                    }
                    File.WriteAllText("digital0.txt", str_tmp);

                    n_track++;
                    str_tmp = "";
                    // read data from digital trace 1 and save them in a file
                    for (int i = (int)odb.info.samples_digital * (n_track - 1); i < odb.info.samples_digital * n_track; i++)
                    {
                        str_tmp += odb.digital[i].ToString() + " \n";
                    }
                    File.WriteAllText("digital1.txt", str_tmp);

                    n_track++;
                    str_tmp = "";
                    // read data from digital trace 2 and save them in a file
                    for (int i = (int)odb.info.samples_digital * (n_track - 1); i < odb.info.samples_digital * n_track; i++)
                    {
                        str_tmp += odb.digital[i].ToString() + " \n";
                    }
                    File.WriteAllText("digital2.txt", str_tmp);

                    n_track++;
                    str_tmp = "";
                    // read data from digital trace 3 and save them in a file
                    for (int i = (int)odb.info.samples_digital * (n_track - 1); i < odb.info.samples_digital * n_track; i++)
                    {
                        str_tmp += odb.digital[i].ToString() + " \n";
                    }
                    File.WriteAllText("digital3.txt", str_tmp);
                }
                else
                {
                    System.Console.WriteLine("Error while trying to read data");
                }
            }
            else
            {
                System.Console.WriteLine("Error while trying allocate buffer");
            }
            System.Console.WriteLine("Click enter to close console");
            System.Console.ReadLine();
        }
    }
}
