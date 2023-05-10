package io.github.nuclearinstruments.digitizer;

import io.github.NuclearInstruments.jscisdk.*;

public class Digitizer {

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

        int enabled_channels = 1;
        res = sdk.SetParameterString("board0:/MMCComponents/Digitizer_0.data_processing", "decode");
        res = sdk.SetParameterInteger("board0:/MMCComponents/Digitizer_0.enabledch", enabled_channels);
        res = sdk.SetParameterInteger("board0:/MMCComponents/Digitizer_0.acq_len", 8000);
        res = sdk.SetParameterString("board0:/MMCComponents/Digitizer_0.acq_mode", "blocking");
        res = sdk.SetParameterInteger("board0:/MMCComponents/Digitizer_0.timeout", 2000);
        res = sdk.ExecuteCommand("board0:/MMCComponents/Digitizer_0.start", "");

        // allocate buffer
        Ref<DigitizerDecodedBuffer> buf = new Ref<>(new DigitizerDecodedBuffer());
        res = sdk.AllocateBuffer("board0:/MMCComponents/Digitizer_0", buf);

        // read data
        res = sdk.ReadData("board0:/MMCComponents/Digitizer_0", buf);
        if (res == 0) {
            DigitizerDecodedBuffer buf_obj = buf.get();
            // print data
            for (int i = 0; i < buf_obj.GetInfo().GetValidSamples(); i++) {
                System.out.println(buf_obj.GetAnalog()[i]);
            }
        } else {
            System.out.println("Error trying to read digitizer data");
            Ref<String> error_description = new Ref<>("");
            sdk.s_error(res, error_description);
            System.out.println("ERROR DESCRIPTION: " + error_description.get());
        }
        
        res = sdk.FreeBuffer("board0:/MMCComponents/Digitizer_0", buf);
        res = sdk.FreeLib();
    }
}
