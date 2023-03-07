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
            // Initialize scisdk library
            SciSDK sdk = new SciSDK();

            // Add new device DT1260
            int res = sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0");

            // Print error message
            if (res != 0)
            {
                Console.WriteLine("Unable to connect to the device. Error " + res);
                Environment.Exit(-1);
            }

            // Set registers
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

            // Get register single value
            //int value = 0;
            //res = sdk.GetRegister("board0:/Registers/trgcnt", out value);
            //Console.WriteLine("Register count: "+ value);

            //Get register array value
            //int value = 0;
            //for(int j = 0; j<1000; j++)
            //{
            //    res = sdk.GetRegister("board0:/Registers/trgcnt", out value);
            //    Console.WriteLine("register count: "+ value);
            //}

            // Get register array value that stop with q key
            int value = 0;
            while (true)
            {
                int j;
                for (j = 0; j < 100; j++)
                {
                    res = sdk.GetRegister("board0:/Registers/trgcnt", out value);
                    Console.WriteLine("register count: " + value);
                }

                Console.Write("Press 'q' to quit or any other key to continue: ");
                string input = Console.ReadLine();
                if (input == "q")
                {
                    break;
                }
                j = 0;
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
           
            Console.WriteLine("Press enter to close console");
            Console.ReadLine();
        }
    }
}
