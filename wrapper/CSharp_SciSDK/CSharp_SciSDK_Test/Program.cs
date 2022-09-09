using CSharp_SciSDK;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharp_SciSDK_Test
{
    class Program
    {
        static void Main(string[] args)
        {
            SciSDK sdk = new SciSDK();
            sdk.AddNewDevice("usb:13250", "dt1260", "C:\\Users\\Testing2\\Downloads\\oscilloscope.json", "board0");
            //sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self");
            sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_level", 3000);
            //sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150);
            //sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0);
            //sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode");
            sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking");
            //sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.timeout", 1000);

            var buf = new SciSDKOscilloscopeDecodedBuffer();
            //int res = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", Buffer_Type.BUFFER_TYPE_DECODED, ref buf);
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

            //Console.WriteLine(sdk.DecodeData("board0:/MMCComponents/Oscilloscope_0", new SciSDKOscilloscopeDecodedBuffer(), ref buf));
            string value = "";
            //sdk.GetParameterDescription("board0:/MMCComponents/Oscilloscope_0.timeout", out value);
            int res = sdk.GetParameterDescription("board0:/MMCComponents/Oscilloscope_0.acq_mode", out value);
            //int res = sdk.GetComponentList("board0", "all", out value, true);
            Console.WriteLine(res);
            Console.WriteLine(value);

            sdk.DetachDevice("board0");

            Console.WriteLine("Press enter to exit...");
            Console.ReadLine();
        }
    }
}
