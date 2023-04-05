using System.Collections;
using System.Text;
using CSharp_SciSDK;
using System.Runtime.InteropServices;

namespace ReadSpectrum
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
                Console.WriteLine("Unable to connect to the device");
            }

            // Set registers
            res = sdk.SetRegister("board0:/Registers/pol", 1);
            res = sdk.SetRegister("board0:/Registers/trgthrs", 2000);
            res = sdk.SetRegister("board0:/Registers/Nsample", 7);
            res = sdk.SetRegister("board0:/Registers/time_hold_bs", 100);
            res = sdk.SetRegister("board0:/Registers/time_integr", 76);
            res = sdk.SetRegister("board0:/Registers/pre_integr", 5);
            res = sdk.SetRegister("board0:/Registers/gain", 30000);
            res = sdk.SetRegister("board0:/Registers/Offset_int", 0);
            
            if (res != 0)
            {
                Console.WriteLine("Error while trying to write register value");
            }

            // // SPECTRUM
            // Set parameters spectrum
            res = sdk.SetParameter("board0:/MMCComponents/Spectrum_0.rebin", "0");
            res = sdk.SetParameter("board0:/MMCComponents/Spectrum_0.limitmode", "freerun");

            if (res != 0)
            {
                Console.WriteLine("Error set spectrum parameters");
            }

            // Execute command reset
            sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "");

            // Execute command start
            sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "");

            // Waiting for data
            for(int i = 0; i < 50; i++)
            {
                Thread.Sleep(1000);
                Console.WriteLine("Waiting for data " + i.ToString() + " s");
            }

            // Allocate buffer
            SciSDKSpectrumDecodedBuffer obSpectrum = new SciSDKSpectrumDecodedBuffer();
            res = sdk.AllocateBuffer("board0:/MMCComponents/Spectrum_0", 0, ref obSpectrum);

            if (res != 0)
            {
                Console.WriteLine("Error allocating spectrum buffer");
                Environment.Exit(-1);
            }
            res = sdk.SetParameter("board0:/MMCComponents/Spectrum_0.rebin", "0");
            res = sdk.SetParameter("board0:/MMCComponents/Spectrum_0.limitmode", "freerun");
            
            res = sdk.ReadData("board0:/MMCComponents/Spectrum_0", ref obSpectrum);
            string str_to_write = "";

            if (res == 0)
            {
                for (int i = 0; i < obSpectrum.info.total_bins; i++)
                {
                    str_to_write += i.ToString() + "\t" + obSpectrum.data[i].ToString() + "\n";
                }
            }
            // Console.WriteLine(str_to_write);
            sdk.FreeBuffer("board0:/MMCComponents/Spectrum_0", 0, ref obSpectrum);

            if (res != 0)
            { 
                Console.WriteLine("Error while trying to read spectrum value");
            }

            string directoryPath = @"C:\GIT\Tutorial\Charge-Integration\software\C#\ReadSpectrum";
            string fileName = "Spectrum.txt";
            string filePath = Path.Combine(directoryPath, fileName);

            // Create a StreamWriter object to write to the file
            StreamWriter writer = new StreamWriter(filePath);

            // Write the text to the file
            writer.WriteLine(str_to_write);

            // Close the StreamWriter to release resources
            writer.Close();

            // Detach device
            sdk.DetachDevice("board0");

            Console.WriteLine("Press enter to close console");
            Console.ReadLine();

        }
    }
}

