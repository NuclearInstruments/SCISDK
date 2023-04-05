using CSharp_SciSDK;
using System;
using System.Reflection;
using System.Runtime.InteropServices;


namespace Read_Oscilloscope
{
    class Program
    {
        static void Main(string[] args)
        {
            // init library
            SciSDK sdk = new SciSDK();

            int polarity = 1;
            int trigger_threshold = 2000;
            int Nsample = 7;
            int time_hold_bs = 100;
            int time_integr = 76;
            int pre_integr = 5;
            int gain = 30000;
            int Offset_int = 0;

            // add new device
            int res = sdk.AddNewDevice("usb:13250", "dt1260", "RegisterFile.json", "board0");
            if (res != 0)
            {
                Console.WriteLine("Unable to connect to the device");
            }

            // Set registers
            res = sdk.SetRegister("board0:/Registers/pol", polarity);
            res = sdk.SetRegister("board0:/Registers/trgthrs", trigger_threshold);
            res = sdk.SetRegister("board0:/Registers/Nsample", Nsample);
            res = sdk.SetRegister("board0:/Registers/time_hold_bs", time_hold_bs);
            res = sdk.SetRegister("board0:/Registers/time_integr", time_integr);
            res = sdk.SetRegister("board0:/Registers/pre_integr", pre_integr);
            res = sdk.SetRegister("board0:/Registers/gain", gain);
            res = sdk.SetRegister("board0:/Registers/Offset_int", Offset_int);

            if (res == 0)
            {
                Console.WriteLine("Register value has been successully set");
            }
            else
            {
                Console.WriteLine("Error while trying to write register value");
            }

            // // OSCILLOSCOPE
            // Set parameters oscilloscope
            string oscilloscope_base_path = "board0:/MMCComponents/Oscilloscope_0";
            sdk.SetParameter(oscilloscope_base_path + ".decimator", 1);
            sdk.SetParameter(oscilloscope_base_path + ".trigger_mode", "ext");
            sdk.SetParameter(oscilloscope_base_path + ".pretrigger", 150);
            sdk.SetParameter(oscilloscope_base_path + ".data_processing", "decode");
            sdk.SetParameter(oscilloscope_base_path + ".acq_mode", "blocking");
            sdk.SetParameter(oscilloscope_base_path + ".timeout", 1000);

            string directoryPath = @"C:\GIT\Tutorial\Charge-Integration\software\C#\Read_Oscilloscope\";

            // allocate buffer
            SciSDKOscilloscopeDecodedBuffer odb = new SciSDKOscilloscopeDecodedBuffer();
            res = sdk.AllocateBuffer(oscilloscope_base_path, BufferType.BUFFER_TYPE_DECODED, ref odb);

            if (res != 0)
            {
                Console.WriteLine("Error allocating spectrum buffer");
                Environment.Exit(-1);
            }

            sdk.ExecuteCommand(oscilloscope_base_path + ".reset_read_valid_flag", "");

            // read data from board
            res = sdk.ReadData(oscilloscope_base_path, ref odb);
            int offs_dig = (int)odb.info.samples_digital * (int)odb.info.tracks_digital_per_channel;
            int offs_an = (int)odb.info.samples_analog;


            if (res == 0)
            {
                string str_tmp = "Analog Channels" + "\n";
                str_tmp += "0    " + "\t" + "1    " + "\t" + "2    " + "\t" + "3    " + "\n";
                for (int index = 0; index < odb.info.samples_analog; index++)
                {
                    str_tmp += odb.analog[index].ToString() + "\t" + 
                        odb.analog[index + offs_an * 1].ToString() + "\t" + 
                        odb.analog[index + offs_an * 2].ToString() + "\t" + 
                        odb.analog[index + offs_an * 3].ToString() + "\n";
                }
                File.WriteAllText(directoryPath + "analog.txt", str_tmp);
                //System.Console.WriteLine("Analog value saved inside " + directoryPath + "analog.txt");


                str_tmp = "";
                str_tmp += "Digital 0" + "\n" + "analog channels" + "\n";
                str_tmp += "0    " + "\t" + "1    " + "\t" + "2    " + "\t" + "3    " + "\n";
                for (int i = 0; i < odb.info.samples_digital; i++)
                {
                    str_tmp +=  (odb.digital[i] ? 1 : 0).ToString() + "\t" + 
                        (odb.digital[i + offs_dig * 1] ? 1 : 0).ToString() + "\t" + 
                        (odb.digital[i + offs_dig * 2] ? 1 : 0).ToString() + "\t" +
                        (odb.digital[i + offs_dig * 3] ? 1 : 0).ToString() + "\n";
                }
                File.WriteAllText(directoryPath + "digital0.txt", str_tmp);

                str_tmp = "";
                str_tmp += "Digital 1" + "\n" + "analog channels" + "\n";
                str_tmp += "0    " + "\t" + "1    " + "\t" + "2    " + "\t" + "3    " + "\n";
                for (int i = 0; i < odb.info.samples_digital; i++)
                {
                    str_tmp += (odb.digital[i+ offs_an] ? 1 : 0).ToString() + "\t" + 
                        (odb.digital[i + offs_dig * 1 + offs_an] ? 1 : 0).ToString() + "\t" + 
                        (odb.digital[i + offs_dig * 2 + offs_an] ? 1 : 0).ToString() + "\t" +
                        (odb.digital[i + offs_dig * 3 + offs_an] ? 1 : 0).ToString() + "\n";
                }
                File.WriteAllText(directoryPath + "digital1.txt", str_tmp);

                str_tmp = "";
                str_tmp += "Digital 2" + "\n" + "analog channels" + "\n";
                str_tmp += "0    " + "\t" + "1    " + "\t" + "2    " + "\t" + "3    " + "\n";
                for (int i = 0; i < odb.info.samples_digital; i++)
                {
                    str_tmp += (odb.digital[i + offs_an * 2] ? 1 : 0).ToString() + "\t" + 
                        (odb.digital[i + offs_dig * 1 + offs_an * 2] ? 1 : 0).ToString() + "\t" + 
                        (odb.digital[i + offs_dig * 2 + offs_an * 2] ? 1 : 0).ToString() + "\t" +
                        (odb.digital[i + offs_dig * 3 + offs_an * 2] ? 1 : 0).ToString() + "\n";
                }
                File.WriteAllText(directoryPath + "digital2.txt", str_tmp);

                str_tmp = "";
                str_tmp += "Digital 3" + "\n" + "analog channels" + "\n";
                str_tmp += "0    " + "\t" + "1    " + "\t" + "2    " + "\t" + "3    " + "\n";
                for (int i = 0; i < odb.info.samples_digital; i++)
                {
                    str_tmp += (odb.digital[i + offs_an * 3] ? 1 : 0).ToString() + "\t" + 
                        (odb.digital[i + offs_dig * 1 + offs_an * 3] ? 1 : 0).ToString() + "\t" + 
                        (odb.digital[i + offs_dig * 2 + offs_an * 3] ? 1 : 0).ToString() + "\t" +
                        (odb.digital[i + offs_dig * 3 + offs_an * 3] ? 1 : 0).ToString() + "\n";
                }
                File.WriteAllText(directoryPath + "digital3.txt", str_tmp);

                
            }
            else
            {
                System.Console.WriteLine("Error while trying to read data");
            }
            
            System.Console.WriteLine("Click enter to close console");
            System.Console.ReadLine();
        }
    }
}
