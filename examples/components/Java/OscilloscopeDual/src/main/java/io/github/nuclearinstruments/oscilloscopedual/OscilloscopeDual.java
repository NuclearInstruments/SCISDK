package io.github.nuclearinstruments.oscilloscopedual;

import io.github.NuclearInstruments.jscisdk.*;
import java.io.FileWriter;
import java.io.IOException;

public class OscilloscopeDual {

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

        // set board parameters
        sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self");
        sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", 3000);
        sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0);
        sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150);
        sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0);
        sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode");
        sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking");
        sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.timeout", 1000);

        // allocate buffer
        Ref<OscilloscopeDualDecodedBuffer> buf = new Ref<>(new OscilloscopeDualDecodedBuffer());
        res = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", buf);

        if (res == 0) {

            sdk.ExecuteCommand("board0:/MMCComponents/Oscilloscope_0.reset_read_valid_flag", "");
            res = sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", buf);
            if (res == 0) {
                // read channels and store data in one file for each channel
                OscilloscopeDualDecodedBuffer buf_obj = buf.get();
                for (int i = 0; i < buf_obj.GetInfo().GetChannels(); i++) {
                    String str_tmp = "";
                    for (int j = 0; j < buf_obj.GetInfo().GetSamplesAnalog(); j++) {
                        str_tmp += buf_obj.GetAnalog()[j + i * (int) buf_obj.GetInfo().GetSamplesAnalog()] + "\n";
                    }
                    WriteFile("analog" + i + ".txt", str_tmp);
                }
            } else {
                System.out.println("Error trying to read oscilloscope data");
                Ref<String> error_description = new Ref<>("");
                sdk.s_error(res, error_description);
                System.out.println("ERROR DESCRIPTION: " + error_description.get());
            }
        } else {
            System.out.println("Error trying to allocate oscilloscope buffer");
            Ref<String> error_description = new Ref<>("");
            sdk.s_error(res, error_description);
            System.out.println("ERROR DESCRIPTION: " + error_description.get());
        }
        sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", buf);
    }
}
