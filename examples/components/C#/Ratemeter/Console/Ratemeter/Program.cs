using CSharp_SciSDK;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ratemeter
{
    class Program
    {
        static void Main(string[] args)
        {
            // init library
            SciSDK sdk = new SciSDK();

            // add new device
            int res = sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0");

            if (res == 0)
            {
                // allocate buffer
                SciSDKRMRawBuffer rm_buf = new SciSDKRMRawBuffer();
                res = sdk.AllocateBuffer("board0:/MMCComponents/RateMeter_0", BufferType.BUFFER_TYPE_RAW, ref rm_buf);
                if (res == 0)
                {
                    // read data
                    res = sdk.ReadData("board0:/MMCComponents/RateMeter_0", ref rm_buf);
                    if (res == 0)
                    {
                        for (int i = 0; i < rm_buf.info.nchannels; i++)
                        {
                            Console.WriteLine("Rate meter channel " + i + ": " + rm_buf.data[i]);
                        }
                    }
                    else
                    {
                        // print error message
                        string ret = "";
                        sdk.s_error(res, out ret);
                        Console.WriteLine(ret);
                    }

                    sdk.FreeBuffer("board0:/MMCComponents/RateMeter_0", BufferType.BUFFER_TYPE_RAW, ref rm_buf);
                }
                else
                {
                    // print error message
                    string ret = "";
                    sdk.s_error(res, out ret);
                    Console.WriteLine(ret);
                }
            }
            else
            {
                // print error message
                string ret = "";
                sdk.s_error(res, out ret);
                Console.WriteLine(ret);
            }

            System.Console.WriteLine("Click enter to close console");
            System.Console.ReadLine();
        }
    }
}
