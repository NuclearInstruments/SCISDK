using CSharp_SciSDK;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Digitizer
{
    class Program
    {
        static void Main(string[] args)
        {
            // init lib
            SciSDK sdk = new SciSDK();

            // add device
            int res = sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0");

            if (res == 0)
            {
                // allocate buffer
                SciSDKDigitizerDecodedBuffer buffer = new SciSDKDigitizerDecodedBuffer();
                res = sdk.AllocateBuffer("board0:/MMCComponents/Digitizer_0", BufferType.BUFFER_TYPE_DECODED, ref buffer);

                if(res == 0)
                {
                    // set board params
                    sdk.SetParameter("board0:/MMCComponents/Digitizer_0.acq_len", 1000);
                    sdk.SetParameter("board0:/MMCComponents/Digitizer_0.acq_mode", "blocking");
                    sdk.SetParameter("board0:/MMCComponents/Digitizer_0.timeout", 10000);
                    sdk.SetParameter("board0:/MMCComponents/Digitizer_0.enabledch", 2);
                    sdk.SetParameter("board0:/MMCComponents/Digitizer_0.data_processing", "decode");

                    // start reading data
                    sdk.ExecuteCommand("board0:/MMCComponents/Digitizer_0.start", "");

                    // read data
                    res = sdk.ReadData("board0:/MMCComponents/Digitizer_0", ref buffer);

                    if(res == 0)
                    {
                        for(int j = 0; j < buffer.info.channels; j++)
                        {
                            // store data into a file
                            string str_tmp = "";
                            for (int i = 0; i < buffer.info.valid_samples; i++)
                            {
                                str_tmp += buffer.analog[i + (j * buffer.info.samples)].ToString() + "\n";
                            }
                            File.WriteAllText("channel"+j.ToString()+".txt", str_tmp);
                            Console.WriteLine("Output file saved at: " + Directory.GetCurrentDirectory() + "\\channel"+j.ToString()+".txt");
                        }
                    }
                    else
                    {
                        //print error message
                        string str_tmp = "";
                        sdk.s_error(res, out str_tmp);
                        Console.WriteLine("Read data error: " + res.ToString() + " - " + str_tmp);
                    }
                }
                else
                {
                    //print error message
                    string str_tmp = "";
                    sdk.s_error(res, out str_tmp);
                    Console.WriteLine("Allocate buffer error: " + res.ToString() + " - " + str_tmp);
                }
            }
            else
            {
                //print error message
                string str_tmp = "";
                sdk.s_error(res, out str_tmp);
                Console.WriteLine("Connection error: " + res.ToString() + " - " + str_tmp);
            }

            Console.WriteLine("Press enter to close console");
            Console.ReadLine();
        }
    }
}
