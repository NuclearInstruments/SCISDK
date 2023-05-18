package io.github.nuclearinstruments.custompacket;

import io.github.NuclearInstruments.jscisdk.*;

public class CustomPacket {

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

        res = sdk.SetParameterString("board0:/MMCComponents/CP_0.thread", "false");
        res = sdk.SetParameterInteger("board0:/MMCComponents/CP_0.timeout", 500);
        res = sdk.SetParameterString("board0:/MMCComponents/CP_0.acq_mode", "non-blocking");
        res = sdk.SetParameterString("board0:/MMCComponents/CP_0.check_align_word", "check_align_word");
        res = sdk.SetParameterString("board0:/MMCComponents/CP_0.data_processing", "decode");

        // Allocate buffer decoded, size 100
        Ref<CPDecodedBuffer> buf = new Ref<>(new CPDecodedBuffer());
        res = sdk.AllocateBuffer("board0:/MMCComponents/CP_0", buf, 100);

        res = sdk.ExecuteCommand("board0:/MMCComponents/CP_0.start", "");

        // read data
        while (true) {
            res = sdk.ReadData("board0:/MMCComponents/CP_0", buf);
            if (res == 0) {
                CPDecodedBuffer buf_obj = buf.get();
                for(int i=0;i<buf_obj.GetInfo().GetValidData();i++){
                    System.out.println("header   : " + Long.toHexString(buf_obj.GetData()[i].GetRow()[0]));
                    System.out.println("timestamp: " + (long)(buf_obj.GetData()[i].GetRow()[1] + (buf_obj.GetData()[i].GetRow()[2] << 32)));
                    System.out.println("counter 0: " + buf_obj.GetData()[i].GetRow()[3]);
                    System.out.println("counter 1: " + buf_obj.GetData()[i].GetRow()[4]);
                    System.out.println("counter 2: " + buf_obj.GetData()[i].GetRow()[5]);
                    System.out.println("counter 3: " + buf_obj.GetData()[i].GetRow()[6]);
                    System.out.println("-----------------");
                }
            }
        }
    }
}
