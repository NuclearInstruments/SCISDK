using System;
using System.Collections;
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
            res = sdk.SetRegister("board0:/Registers/trgthrs", 2000);
            res = sdk.SetRegister("board0:/Registers/pol", 1);
            res = sdk.SetRegister("board0:/Registers/PulsePeriod", 6250000);

            if (res != 0)
            { 
                Console.WriteLine("Error while trying to write register value");
            }

            // Allocate buffer raw, size 1024
            SciSDKListRawBuffer lrb = new SciSDKListRawBuffer();
            res = sdk.AllocateBuffer("board0:/MMCComponents/List_0", 0, ref lrb, 1024);

            // Set parameters List
            res = sdk.SetParameter("board0:/MMCComponents/List_0.thread", "false");
            res = sdk.SetParameter("board0:/MMCComponents/List_0.timeout", 1000);
            res = sdk.SetParameter("board0:/MMCComponents/List_0.acq_mode", "blocking");

            if (res != 0)
            {
                Console.WriteLine("Error allocting list buffer");
            }

            sdk.ExecuteCommand("board0:/MMCComponents/List_0.stop", "");

            // Read data buffered
            res = sdk.ReadData("board0:/MMCComponents/List_0", ref lrb);

            // Get a single value
            //uint value = BitConverter.ToUInt32(lrb.data, 0);
            //Console.WriteLine("Data: " + value);

            //Get array value
            for (int j = 0; j < 10; j++)
            {
                res = sdk.ReadData("board0:/MMCComponents/List_0", ref lrb);
                for (int i = 0; i < lrb.info.valid_samples; i += 4)
                {
                    Console.WriteLine(BitConverter.ToUInt32(lrb.data, i));
                }
            }

            // Get array value that stop with q key
            //while (true)
            //{
            //    int j;
            //    for (j = 0; j < 5; j++)
            //    {
            //        res = sdk.ReadData("board0:/MMCComponents/List_0", ref lrb);
            //        Console.WriteLine(BitConverter.ToUInt32(lrb.data, 0));
            //    }

            //    Console.Write("Press 'q' to quit or any other key to continue: ");
            //    string input = Console.ReadLine();
            //    if (input == "q")
            //    {
            //        break;
            //    }
            //    j = 0;
            //}

            if (res != 0)
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
