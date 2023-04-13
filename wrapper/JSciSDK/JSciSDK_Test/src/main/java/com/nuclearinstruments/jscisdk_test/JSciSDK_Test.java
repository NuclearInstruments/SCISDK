package com.nuclearinstruments.jscisdk_test;

import com.nuclearinstruments.jscisdk.SciSDK;
import com.nuclearinstruments.jscisdk.SpectrumDecodedBuffer;
import java.util.concurrent.atomic.*;

public class JSciSDK_Test {

    public static void main(String[] args) {

        // initialize sdk library
        SciSDK sdk = new SciSDK();

        // Add the DT1260 device to sdk
        int res = sdk.AddNewDevice("usb:28645", "DT1260", "RegisterFile.json", "board0");
        if (res != 0) {
            AtomicReference<String> error_description = new AtomicReference<String>("");
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

        SpectrumDecodedBuffer buffer = new SpectrumDecodedBuffer();
        AtomicReference<SpectrumDecodedBuffer> buf_atm = new AtomicReference<>(buffer);
        res = sdk.AllocateBuffer("board0:/MMCComponents/Spectrum_0", buf_atm);
        buffer=buf_atm.get();
        AtomicReference<String> error_description = new AtomicReference<String>("");
        sdk.s_error(res, error_description);
        System.out.println("error description: " + error_description);
        System.out.println("res: " + res);
        System.out.println("magic: " + (buf_atm.get().magic & 0xFFFFFFFFL));
        System.out.println("buffer size: " + buf_atm.get().info.GetBufferSize());
        System.out.println("inttime: " + buf_atm.get().GetIntTime());
        sdk.FreeLib();
    }
}
