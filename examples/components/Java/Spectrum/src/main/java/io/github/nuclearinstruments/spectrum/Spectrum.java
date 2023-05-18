package io.github.nuclearinstruments.spectrum;

import io.github.NuclearInstruments.jscisdk.*;
import java.io.FileWriter;
import java.io.IOException;

public class Spectrum {

    public static boolean WriteFile(String filepath, String content) {
        try {
            FileWriter myWriter = new FileWriter(filepath);
            myWriter.write(content);
            myWriter.close();
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }
    
    public static void main(String[] args) throws SciSDKException, InterruptedException {
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

        // configure firmware register
        sdk.SetRegister("board0:/Registers/noisepower", 100);
        sdk.SetRegister("board0:/Registers/centroid", 1000);

        // set board parameters
        sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.rebin", "0");
        sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.limitmode", "freerun");
        sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.limit", "100");

        // execute command reset
        sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "");

        // execute command start
        sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.start", "");

        for (int i = 0; i < 10; i++) {
            System.out.println("Wait " + i + "s/10");
            Thread.sleep(1000);
        }

        // allocate buffer
        Ref<SpectrumDecodedBuffer> buf = new Ref<>(new SpectrumDecodedBuffer());
        res = sdk.AllocateBuffer("board0:/MMCComponents/Spectrum_0", buf);

        if (res == 0) {
            // read data
            res = sdk.ReadData("board0:/MMCComponents/Spectrum_0", buf);
            if (res == 0) {
                String str_tmp = "";
                SpectrumDecodedBuffer buf_obj = buf.get();
                for (int i = 0; i < buf_obj.GetInfo().GetTotalBins(); i++) {
                    str_tmp += buf_obj.GetData()[i] + "\n";
                }
                WriteFile("output.txt", str_tmp);
            } else {
                Ref<String> error_description = new Ref<>("");
                sdk.s_error(res, error_description);
                System.out.println("Read data error: " + error_description.get());
            }
            
            sdk.FreeBuffer("board0:/MMCComponents/Spectrum_0", buf);
        } else {
            Ref<String> error_description = new Ref<>("");
            sdk.s_error(res, error_description);
            System.out.println("Allocate buffer error: " + error_description.get());
        }
    }
}
