package io.github.nuclearinstruments.ratemeter;

import io.github.NuclearInstruments.jscisdk.*;

public class Ratemeter {

    public static void main(String[] args) {
        // TODO: CHANGE JSCISDK local project dependency with maven central dependency 
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

        // allocate buffer
        Ref<RMRawBuffer> buf = new Ref<>(new RMRawBuffer());
        res = sdk.AllocateBuffer("board0:/MMCComponents/RateMeter_0", buf);

        if (res == 0) {
            // read data
            res = sdk.ReadData("board0:/MMCComponents/RateMeter_0", buf);
            if (res == 0) {
                System.out.println("-------------------");
                for (int i = 0; i < buf.get().GetData().length; i++) {
                    System.out.println(buf.get().GetData()[i]);
                }
            } else {
                Ref<String> error_description = new Ref<>("");
                sdk.s_error(res, error_description);
                System.out.println("ERROR DESCRIPTION: " + error_description.get());
            }
            res = sdk.FreeBuffer("board0:/MMCComponents/RateMeter_0", buf);
        } else {
            Ref<String> error_description = new Ref<>("");
            sdk.s_error(res, error_description);
            System.out.println("ERROR DESCRIPTION: " + error_description.get());
        }

    }
}
