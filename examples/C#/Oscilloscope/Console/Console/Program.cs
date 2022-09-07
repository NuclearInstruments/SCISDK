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
            IntPtr scisdk_handle = SciSDK.SCISDK_InitLib();

            // add new device
            SciSdk_Wrapper.AddNewDevice("usb:13250", "dt1260", "oscilloscope.json", "board0", scisdk_handle);

            // set parameters
            string oscilloscope_base_path = "board0:/MMCComponents/Oscilloscope_0";
            SciSdk_Wrapper.SetParamInt(oscilloscope_base_path + ".decimator", 0, scisdk_handle);
            SciSdk_Wrapper.SetParamString(oscilloscope_base_path + ".trigger_mode", "self", scisdk_handle);
            SciSdk_Wrapper.SetParamInt(oscilloscope_base_path + ".trigger_level", 3000, scisdk_handle);
            SciSdk_Wrapper.SetParamInt(oscilloscope_base_path + ".pretrigger", 150, scisdk_handle);
            SciSdk_Wrapper.SetParamInt(oscilloscope_base_path + ".trigger_channel", 0, scisdk_handle);
            SciSdk_Wrapper.SetParamString(oscilloscope_base_path + ".data_processing", "decode", scisdk_handle);
            SciSdk_Wrapper.SetParamString(oscilloscope_base_path + ".acq_mode", "blocking", scisdk_handle);
            SciSdk_Wrapper.SetParamInt(oscilloscope_base_path + ".timeout", 1000, scisdk_handle);

            // create oscilloscope buffer
            IntPtr buffer_ptr = Marshal.StringToHGlobalAnsi("t");


            // allocate buffer
            if (SciSdk_Wrapper.AllocateBuffer(oscilloscope_base_path, SciSdk_Wrapper.T_BUFFER_DECODED, ref buffer_ptr, scisdk_handle))
            {
                SciSdk_Wrapper.ExecuteCommand(oscilloscope_base_path + ".reset_read_valid_flag", "", scisdk_handle);

                // read data from board
                if (SciSdk_Wrapper.ReadData(oscilloscope_base_path, ref buffer_ptr, scisdk_handle))
                {
                    // convert buffer pointer to buffer struct
                    Oscilloscope_decoded_buffer_struct buffer_struct = new Oscilloscope_decoded_buffer_struct();
                    buffer_struct = (Oscilloscope_decoded_buffer_struct)Marshal.PtrToStructure(buffer_ptr, typeof(Oscilloscope_decoded_buffer_struct));

                    // read memory and store analog values inside an integer type array
                    int[] analog_values = new int[buffer_struct.info.samples_analog];
                    Marshal.Copy(buffer_struct.analog, analog_values, 0, analog_values.Length);

                    string str_tmp = "";
                    // save analog values inside a file
                    foreach (int value in analog_values)
                    {
                        str_tmp += value.ToString() + "\n";
                    }
                    File.WriteAllText("analog.txt", str_tmp);
                    System.Console.WriteLine("Analog value saved inside " + Directory.GetCurrentDirectory() + "\\analog.txt");

                    // read memory and store digital values inside an inter type array
                    byte[] digital_values = new byte[buffer_struct.info.samples_digital * buffer_struct.info.tracks_digital_per_channel];
                    Marshal.Copy(buffer_struct.digital, digital_values, 0, digital_values.Length);

                    str_tmp = "";
                    int n_track = 1;// number of the track
                    // read data from digital trace 0 and save them in a file
                    for (int i = (int)buffer_struct.info.samples_digital * (n_track - 1); i < buffer_struct.info.samples_digital * n_track; i++)
                    {
                        str_tmp += digital_values[i].ToString() + " \n";
                    }
                    File.WriteAllText("digital0.txt", str_tmp);

                    n_track++;
                    str_tmp = "";
                    // read data from digital trace 1 and save them in a file
                    for (int i = (int)buffer_struct.info.samples_digital * (n_track - 1); i < buffer_struct.info.samples_digital * n_track; i++)
                    {
                        str_tmp += digital_values[i].ToString() + " \n";
                    }
                    File.WriteAllText("digital1.txt", str_tmp);

                    n_track++;
                    str_tmp = "";
                    // read data from digital trace 2 and save them in a file
                    for (int i = (int)buffer_struct.info.samples_digital * (n_track - 1); i < buffer_struct.info.samples_digital * n_track; i++)
                    {
                        str_tmp += digital_values[i].ToString() + " \n";
                    }
                    File.WriteAllText("digital2.txt", str_tmp);

                    n_track++;
                    str_tmp = "";
                    // read data from digital trace 3 and save them in a file
                    for (int i = (int)buffer_struct.info.samples_digital * (n_track - 1); i < buffer_struct.info.samples_digital * n_track; i++)
                    {
                        str_tmp += digital_values[i].ToString() + " \n";
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
