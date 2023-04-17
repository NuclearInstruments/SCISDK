package com.nuclearinstruments.jscisdk_test;

import com.nuclearinstruments.jscisdk.*;
import java.io.FileWriter;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class JSciSDK_Test {

    public static void WriteLongArrayToFile(String filename, long[] data) {
        try {
            FileWriter spectrum_writer = new FileWriter(filename);
            for (int i = 0; i < data.length; i++) {
                spectrum_writer.append(data[i] + "\n");
            }
            spectrum_writer.close();
        } catch (IOException ex) {
            Logger.getLogger(JSciSDK_Test.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public static void WriteIntArrayToFile(String filename, int[] data) {
        try {
            FileWriter spectrum_writer = new FileWriter(filename);
            for (int i = 0; i < data.length; i++) {
                spectrum_writer.append(data[i] + "\n");
            }
            spectrum_writer.close();
        } catch (IOException ex) {
            Logger.getLogger(JSciSDK_Test.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public static void main(String[] args) throws SciSDKException {

        // initialize sdk library
        SciSDK sdk = new SciSDK();

        // Add the DT1260 device to sdk
        int res = sdk.AddNewDevice("usb:28645", "DT1260", "RegisterFile.json", "board0");
        if (res != 0) {
            Ref<String> error_description = new Ref<String>("");
            sdk.s_error(res, error_description);
            System.out.println("Error Adding Device: " + error_description);
            return;
        }

        // Set register values
        res = sdk.SetRegister("board0:/Registers/pol", 1);
        res = sdk.SetRegister("board0:/Registers/trgthrs", 2000);
        res = sdk.SetRegister("board0:/Registers/Nsample", 7);
        res = sdk.SetRegister("board0:/Registers/time_hold_bs", 100);
        res = sdk.SetRegister("board0:/Registers/time_integr", 76);
        res = sdk.SetRegister("board0:/Registers/pre_integr", 5);
        res = sdk.SetRegister("board0:/Registers/gain", 30000);
        res = sdk.SetRegister("board0:/Registers/Offset_int", 0);

        // OSCILLOSCOPE
        // set oscilloscope parameters
        res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decoded");
        //res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "raw");
        res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_level", 2000);
        res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self");
        res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 2000);
        res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150);
        int decimator = 1;
        res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", decimator);
        res = sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking");
        res = sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.timeout", 3000);

        // SPECTRUM
        // Set spectrum parameters
        sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.rebin", "0");
        sdk.SetParameterString("board0:/MMCComponents/Spectrum_0.limitmode", "freerun");

        sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.stop", "");
        res = sdk.ExecuteCommand("board0:/MMCComponents/Spectrum_0.reset", "");

        Ref<SpectrumDecodedBuffer> spectrum_buffer = new Ref<>(new SpectrumDecodedBuffer());
        // allocate buffer
        res = sdk.AllocateBuffer("board0:/MMCComponents/Spectrum_0", spectrum_buffer);
        // read data
        res = sdk.ReadData("board0:/MMCComponents/Spectrum_0", spectrum_buffer);

        if (res == 0) {
            WriteLongArrayToFile("spectrum.txt", spectrum_buffer.get().GetData());
        }

        // OSCILLOSCOPE DECODED
        Ref<OscilloscopeDecodedBuffer> oscilloscope_buffer = new Ref<OscilloscopeDecodedBuffer>(new OscilloscopeDecodedBuffer());
        // allocate buffer
        res = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", oscilloscope_buffer);
        // read data
        res = sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", oscilloscope_buffer);

        if (res == 0) {
            // write analog values into a text file
            WriteIntArrayToFile("oscilloscope_decoded_analog.txt", oscilloscope_buffer.get().GetAnalog());
            // write digital values into a text file
            WriteIntArrayToFile("oscilloscope_decoded_digital.txt", oscilloscope_buffer.get().GetDigital());
        }
        
        /*// OSCILLOSCOPE RAW
        Ref<OscilloscopeRawBuffer> oscilloscope_buffer = new Ref<OscilloscopeRawBuffer>(new OscilloscopeRawBuffer());
        // allocate buffer
        res = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", oscilloscope_buffer);
        // read data
        res = sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", oscilloscope_buffer);

        Ref<OscilloscopeDecodedBuffer> buf_out = new Ref<OscilloscopeDecodedBuffer>(new OscilloscopeDecodedBuffer());
        res = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", buf_out);
        res = sdk.DecodeData("board0:/MMCComponents/Oscilloscope_0", oscilloscope_buffer, buf_out);
        
        if (res == 0) {
            // write analog values into a text file
            WriteIntArrayToFile("oscilloscope_decoded_analog.txt", buf_out.get().GetAnalog());
            // write digital values into a text file
            WriteIntArrayToFile("oscilloscope_decoded_digital.txt", buf_out.get().GetDigital());
        }*/

        res = sdk.FreeBuffer("board0:/MMCComponents/Spectrum_0", spectrum_buffer);
        res = sdk.FreeBuffer("board0:/MMCComponents/Oscilloscope_0", oscilloscope_buffer);

        Ref<String> ref_str = new Ref<String>("");
        sdk.GetComponentList("board0", "all", ref_str, true);
        System.out.println("Component list: " + ref_str.get());

        sdk.GetAllParameters("board0:/MMCComponents/Oscilloscope_0", ref_str);
        System.out.println("Get all parameters: " + ref_str.get());

        Ref<Double> ref_double = new Ref<Double>(0.0d);
        res = sdk.GetParameterMaximumValue("board0:/MMCComponents/Oscilloscope_0.trigger_channel", ref_double);
        System.out.println("Parameter max value: " + ref_double.get());

        res = sdk.GetParameterProperties("board0:/MMCComponents/Oscilloscope_0", ref_str);
        System.out.println("Parameter properties: " + ref_str.get());

        sdk.FreeLib();
    }
}
