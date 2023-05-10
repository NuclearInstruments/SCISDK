package io.github.nuclearinstruments.fft;

import io.github.NuclearInstruments.jscisdk.*;
import java.io.FileWriter;
import java.io.IOException;

public class FFT {

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

        sdk.SetParameterString("board0:/MMCComponents/FFT_0.data_processing", "decode");
        sdk.SetParameterString("board0:/MMCComponents/FFT_0.acq_mode", "blocking");
        
        Ref<FFTDecodedBuffer> buf = new Ref<>(new FFTDecodedBuffer());
        res = sdk.AllocateBuffer("board0:/MMCComponents/FFT_0", buf);
        
        if(res == 0){
            res = sdk.ReadData("board0:/MMCComponents/FFT_0", buf);
            if(res == 0){
                FFTDecodedBuffer buf_obj = buf.get();
                String str_tmp = "";
                for (int i = 0; i < buf_obj.GetMag().length; i++) {
                    str_tmp += buf_obj.GetMag()[i] + "\n";
                }
                WriteFile("mag.txt", str_tmp);
                str_tmp = "";
                for (int i = 0; i < buf_obj.GetPh().length; i++) {
                    str_tmp += buf_obj.GetPh()[i] + "\n";
                }
                WriteFile("ph.txt", str_tmp);
            }
        }
    }
}
