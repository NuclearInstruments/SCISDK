package io.github.nuclearinstruments.list;

import io.github.NuclearInstruments.jscisdk.*;
import java.util.Arrays;

public class List {

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

        // set parameters
        res = sdk.SetParameterString("board0:/MMCComponents/List_0.thread", "false");
        res = sdk.SetParameterInteger("board0:/MMCComponents/List_0.timeout", 500);
        res = sdk.SetParameterString("board0:/MMCComponents/List_0.acq_mode", "blocking");

        // allocate buffer raw, size 1024
        Ref<ListRawBuffer> buf = new Ref<>(new ListRawBuffer());
        res = sdk.AllocateBuffer("board0:/MMCComponents/List_0", buf, 1024);

        res = sdk.ExecuteCommand("board0:/MMCComponents/List_0.stop", "");
        res = sdk.ExecuteCommand("board0:/MMCComponents/List_0.start", "");

        while (true) {
            res = sdk.ReadData("board0:/MMCComponents/List_0", buf);

            if (res == 0) {
                ListRawBuffer buf_obj = buf.get();
                for (int i = 0; i < buf_obj.GetInfo().GetValidSamples() / 8; i++) {
                    byte[] chunk = Arrays.copyOfRange(buf_obj.GetData(), i * 8, (i + 1) * 8);
                    int ch1 = chunk[0] + (chunk[1] << 8);
                    int ch0 = chunk[2] + (chunk[3] << 8);
                    int counter = chunk[4] + (chunk[5] << 8) + (chunk[6] << 16) + (chunk[7] << 24);
                    System.out.println(ch1 + ", " + ch0 + ", " + counter);
                }
            }
        }

    }
    
}
