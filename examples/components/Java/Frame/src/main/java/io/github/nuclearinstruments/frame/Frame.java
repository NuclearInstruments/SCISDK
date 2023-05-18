package io.github.nuclearinstruments.frame;

import io.github.NuclearInstruments.jscisdk.*;

public class Frame {

    public static void main(String[] args) {
        SciSDK sdk = new SciSDK();

        // DT1260
        int res = sdk.AddNewDevice("usb:28645", "dt1260", "./DT1260RegisterFile.json", "board0");
        if (res != 0) {
            System.out.println("Program exit due to connection error");

            // print error description
            Ref<String> error_description = new Ref<>("");
            sdk.s_error(res, error_description);
            System.out.println("ERROR DESCRIPTION: " + error_description.get());
            return;
        }

        res = sdk.SetParameterString("board0:/MMCComponents/ImageReadout_0.thread", "false");
        res = sdk.SetParameterInteger("board0:/MMCComponents/ImageReadout_0.timeout", 500);
        res = sdk.SetParameterString("board0:/MMCComponents/ImageReadout_0.acq_mode", "non-blocking");
        res = sdk.SetParameterString("board0:/MMCComponents/ImageReadout_0.trigger_mode", "trig");
        res = sdk.SetParameterString("board0:/MMCComponents/ImageReadout_0.data_processing", "decode");
        
        // allocate buffer
        Ref<FrameDecodedBuffer> fdb = new Ref(new FrameDecodedBuffer());
        res = sdk.AllocateBuffer("board0:/MMCComponents/ImageReadout_0", fdb, 100);

        res = sdk.ExecuteCommand("board0:/MMCComponents/ImageReadout_0.start", "");

        // read data
        while (true) {
            res = sdk.ReadData("board0:/MMCComponents/ImageReadout_0", fdb);
            if (res == 0) {
                FramePacket[] fp = fdb.get().GetData();
                for (int i = 0; i < fp.length; i++) {
                    System.out.println(fp[i].GetPixel()[0] + ", " + fp[i].GetPixel()[1] + ", " + fp[i].GetPixel()[2]);
                    System.out.println(fp[i].GetPixel()[3] + ", " + fp[i].GetPixel()[4] + ", " + fp[i].GetPixel()[5]);
                    System.out.println(fp[i].GetPixel()[6] + ", " + fp[i].GetPixel()[7] + ", " + fp[i].GetPixel()[8]);
                    System.out.println("");
                }
            }
        }
    }
}
