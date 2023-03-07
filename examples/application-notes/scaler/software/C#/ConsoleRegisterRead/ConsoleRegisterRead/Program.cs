using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CSharp_SciSDK;


namespace Registers
{
    class Program
    {
        static void Main(string[] args)
        {
            SciSDK sdk = new SciSDK();

            // Add new device
            int res = sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0");

            // Print error message
            if (res != 0)
            {
                Console.WriteLine("Unable to connect to the device. Error " + res);
                Environment.Exit(-1);
            }

            if (res == 0)
            {
                // Write register
                res = sdk.SetRegister("board0:/Registers/trgthrs", 2500);
                res = sdk.SetRegister("board0:/Registers/pol", 1);

                if (res == 0)
                {
                    Console.WriteLine("Register value has been successully set");
                }
                else
                {
                    Console.WriteLine("Error while trying to write register value");
                }

                // Read register
                int value = 0;
                for(int j = 0; j<100; j++)
                {
                    res = sdk.GetRegister("board0:/Registers/trgcnt", out value);
                    Console.WriteLine(value);
                }
                

                if (res == 0)
                {
                    Console.WriteLine("Register value is read");
                }
                else
                {
                    Console.WriteLine("Error while trying to read register value");
                }

                // Detach device
                sdk.DetachDevice("board0");
            }
            else
            {
                // Print error message
                string str_tmp;
                sdk.s_error(res, out str_tmp);
                Console.WriteLine("Connection error: " + str_tmp);
            }

            Console.WriteLine("Press enter to close console");
            Console.ReadLine();
        }
    }
}
