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
            SciSDK_Device scidk_board = sdk.AddDevice(SciSDK_DeviceModel.SCIDK, "usb:13250", "C:\\Users\\Testing2\\Downloads\\oscilloscope.json", "board0");

            if(scidk_board != null)
            {
                SciSDK_Node oscilloscope_0 = scidk_board.GetNode("Oscilloscope_0");
                
                scidk_board.Detach();
            }
            else
            {
                Console.WriteLine("Error while trying to add new device");
            }
            
            Console.WriteLine("Press enter to exit...");
            Console.ReadLine();
        }
    }
}
