using CSharp_SciSDK;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace CSharp_SciSDK_Test
{
    class Program
    {
        static void Main(string[] args)
        {
            // init library
            SciSDK sdk = new SciSDK();

            // add new device
            int res = sdk.AddNewDevice("usb:28645", "dt1260", "C:\\git\\scisdk\\examples\\components\\Java\\Frame\\DT1260RegisterFile.json", "board0");
            string temp = "";
            sdk.GetAttachedDevicesList(out temp);
            Console.WriteLine(temp);
            sdk.GetLibraryVersion(out temp);
            Console.WriteLine(temp);
            //if (res == 0)
            //{
            //    res = sdk.SetParameter("board0:/MMCComponents/ImageReadout_0.thread", "false");
            //    res = sdk.SetParameter("board0:/MMCComponents/ImageReadout_0.timeout", 500);
            //    res = sdk.SetParameter("board0:/MMCComponents/ImageReadout_0.acq_mode", "non-blocking");
            //    res = sdk.SetParameter("board0:/MMCComponents/ImageReadout_0.trigger_mode", "trig");
            //    res = sdk.SetParameter("board0:/MMCComponents/ImageReadout_0.data_processing", "raw");

            //    SciSDKFrameRawBuffer buf = new SciSDKFrameRawBuffer();
            //    res = sdk.AllocateBuffer("board0:/MMCComponents/ImageReadout_0", BufferType.BUFFER_TYPE_RAW, ref buf, 100);

            //    res = sdk.ExecuteCommand("board0:/MMCComponents/ImageReadout_0.start", "");

            //    Thread.Sleep(1000);
            //    res = sdk.ReadData("board0:/MMCComponents/ImageReadout_0", ref buf);

            //    if(res == 0)
            //    {
            //        int a = 0;
            //    }
            //}



            //if (res == 0)
            //{
            //    string temp = "";
            //    sdk.GetAttachedDevicesList(out temp);
            //    Console.WriteLine(temp);
            //    //sdk.GetLibraryVersion(out temp);
            //    Console.WriteLine(temp);


            //    // allocate buffer
            //    SciSDKRMRawBuffer rm_buf = new SciSDKRMRawBuffer();
            //    res = sdk.AllocateBuffer("board0:/MMCComponents/RateMeter_0", BufferType.BUFFER_TYPE_RAW, ref rm_buf);
            //    if (res == 0)
            //    {
            //        // read data
            //        res = sdk.ReadData("board0:/MMCComponents/RateMeter_0", ref rm_buf);
            //        if (res == 0)
            //        {
            //            for (int i = 0; i < rm_buf.info.nchannels; i++)
            //            {
            //                Console.WriteLine("Rate meter channel " + i + ": " + rm_buf.data[i]);
            //            }
            //        }
            //        else
            //        {
            //            // print error message
            //            string ret = "";
            //            sdk.s_error(res, out ret);
            //            Console.WriteLine(ret);
            //        }

            //        sdk.FreeBuffer("board0:/MMCComponents/RateMeter_0", BufferType.BUFFER_TYPE_RAW, ref rm_buf);
            //    }
            //    else
            //    {
            //        // print error message
            //        string ret = "";
            //        sdk.s_error(res, out ret);
            //        Console.WriteLine(ret);
            //    }
            //}
            //else
            //{
            //    // print error message
            //    string ret = "";
            //    sdk.s_error(res, out ret);
            //    Console.WriteLine(ret);
            //}

            System.Console.WriteLine("Click enter to close console");
            System.Console.ReadLine();
            //SciSDK sdk = new SciSDK();
            //sdk.AddNewDevice("usb:13250", "dt1260", "C:\\Users\\Testing2\\Downloads\\oscilloscope.json", "board0");
            //sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self");
            //sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_level", 3000);
            //sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150);
            //sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0);
            //sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode");
            //sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking");
            //sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.timeout", 1000);

            //var buf = new SciSDKOscilloscopeDecodedBuffer();
            //int res = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", BufferType.BUFFER_TYPE_DECODED, ref buf);
            //if (res == 0)
            //{
            //    res = sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", ref buf);
            //    if (res == 0)
            //    {
            //        for (int i = 0; i < buf.digital.Length; i++)
            //        {
            //            //Console.WriteLine(buf.digital[i]);
            //        }
            //    }
            //}

            ////Console.WriteLine(sdk.DecodeData("board0:/MMCComponents/Oscilloscope_0", new SciSDKOscilloscopeDecodedBuffer(), ref buf));
            //string value = "";
            ////sdk.GetParameterDescription("board0:/MMCComponents/Oscilloscope_0.timeout", out value);
            //res = sdk.GetParameterDescription("board0:/MMCComponents/Oscilloscope_0.acq_mode", out value);
            ////int res = sdk.GetComponentList("board0", "all", out value, true);
            //Console.WriteLine(res);
            //Console.WriteLine(value);

            //sdk.DetachDevice("board0");

            //Console.WriteLine("Press enter to exit...");
            //Console.ReadLine();
        }
    }
}
