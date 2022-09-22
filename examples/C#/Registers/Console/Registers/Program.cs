using CSharp_SciSDK;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Registers
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
                // write register
                res = sdk.SetParameter("board0:/MMCComponents/CREG_1.register_2", 12);
                if(res == 0)
                {
                    Console.WriteLine("Register value has been successully set");
                }
                else
                {
                    Console.WriteLine("Error while trying to write register value");
                }

                // read register
                int value = 0;
                res = sdk.GetParameter("board0:/MMCComponents/CREG_0.register_2", out value);
                if (res == 0)
                {
                    Console.WriteLine("Register value is " + value);
                }
                else
                {
                    Console.WriteLine("Error while trying to read register value");
                }

                // read values from multichannel register
                Console.WriteLine("Values from multichannel register:");
                for (int i = 0; i < 6; i++)
                {
                    value = 0;
                    res = sdk.GetParameter("board0:/MMCComponents/REGFILE_0.register_" + i, out value);
                    Console.WriteLine(value);
                }
                
                // detach device
                sdk.DetachDevice("board0");
            }
            else
            {
                // print error message
                string str_tmp;
                sdk.s_error(res, out str_tmp);
                Console.WriteLine("Connection error: " + str_tmp);
            }

            Console.WriteLine("Press enter to close console");
            Console.ReadLine();
        }
    }
}
