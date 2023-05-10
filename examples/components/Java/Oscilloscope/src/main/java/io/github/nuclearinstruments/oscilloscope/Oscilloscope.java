package io.github.nuclearinstruments.oscilloscope;

import io.github.NuclearInstruments.jscisdk.*;
import java.io.FileWriter;
import java.io.IOException;

public class Oscilloscope {

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

        // set parameters
        res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode");
        res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", 1500);
        res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self");
        res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0);
        res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150);
        res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0);
        res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking");

        // allocate buffer
        Ref<OscilloscopeDecodedBuffer> buf = new Ref<>(new OscilloscopeDecodedBuffer());
        res = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", buf);

        if (res == 0) {
            // read data
            res = sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", buf);

            if (res == 0) {
                OscilloscopeDecodedBuffer buf_obj = buf.get();
                // store analog data inside a file
                String analog_str = "";
                for (int i = 0; i < buf_obj.GetAnalog().length; i++) {
                    analog_str += buf_obj.GetAnalog()[i] + "\n";
                }
                WriteFile("analog.txt", analog_str);

                // store digital data inside a file
                String digital_str = "";
                for (int i = 0; i < buf_obj.GetDigital().length; i++) {
                    digital_str += buf_obj.GetDigital()[i] + "\n";
                }
                WriteFile("digital.txt", digital_str);
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
