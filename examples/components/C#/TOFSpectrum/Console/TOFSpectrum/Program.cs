using CSharp_SciSDK;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TOFSpectrum
{
    class Program
    {
        static void Main(string[] args)
        {
            SciSDK sdk = new SciSDK();

            // add new device
            int res = sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0");

            if(res == 0)
            {
                // set board parameters
                sdk.SetParameter("board0:/MMCComponents/TOF_0.rebin", "0");
                sdk.SetParameter("board0:/MMCComponents/TOF_0.limitmode", "freerun");
                sdk.SetParameter("board0:/MMCComponents/TOF_0.limit", "100");

                // execute reset command
                sdk.ExecuteCommand("board0:/MMCComponents/TOF_0.reset", "");

                // execute start command
                sdk.ExecuteCommand("board0:/MMCComponents/TOF_0.start", "");

                //sdk.SetRegister("board0:/Registers/DELAY", 100);

                // wait 4 seconds
                Thread.Sleep(4000);

                // allocate buffer
                SciSDKSpectrumDecodedBuffer buffer = new SciSDKSpectrumDecodedBuffer();
                res = sdk.AllocateBuffer("board0:/MMCComponents/TOF_0", BufferType.BUFFER_TYPE_DECODED, ref buffer);

                if(res == 0)
                {
                    // read data
                    res = sdk.ReadData("board0:/MMCComponents/TOF_0", ref buffer);
                    if(res == 0)
                    {
                        string str_tmp = "";

                        for (int i = 0; i < buffer.info.total_bins; i++)
                        {
                            str_tmp += buffer.data[i].ToString() + "\n";
                        }
                        // write file
                        File.WriteAllText("output.txt", str_tmp);
                        Console.WriteLine("OUTPUT FILE: " + Directory.GetCurrentDirectory() + "\\output.txt");
                    }
                    else
                    {
                        // print error message
                        string str_tmp = "";
                        sdk.s_error(res, out str_tmp);
                        Console.WriteLine("Read data error - " + str_tmp);
                    }
                }
                else
                {
                    // print error message
                    string str_tmp = "";
                    sdk.s_error(res, out str_tmp);
                    Console.WriteLine("Allocate buffer error - " + str_tmp);
                }
            }
            else
            {
                // print error message
                Console.WriteLine("Program exit due connection error");
                string str_tmp = "";
                sdk.s_error(res, out str_tmp);
                Console.WriteLine(str_tmp);
            }
            Console.WriteLine("Press enter to close console...");
            Console.ReadLine();
        }
    }
}
