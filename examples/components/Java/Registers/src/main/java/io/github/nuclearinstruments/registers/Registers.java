package io.github.nuclearinstruments.registers;

import io.github.NuclearInstruments.jscisdk.*;

public class Registers {

    public static void main(String[] args) throws SciSDKException {
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

        int regA = 100;
        int regB = 151;
        sdk.SetRegister("board0:/Registers/A", regA);
        sdk.SetRegister("board0:/Registers/B", regB);

        Ref<Integer> value = new Ref<>(0);
        sdk.GetRegister("board0:/Registers/C", value);
        System.out.println("Register C = (A+B) value is: " + value.get());

        // write and readback channel registers (from 0 to 5)
        for (int i = 0; i < 6; i++) {
            res = sdk.SetParameterInteger("board0:/MMCComponents/CREG_0.CH" + i, i + 10);
            if (res == 0) {
                System.out.println("Register CREG_0.CH" + i + " value has been successully set to " + (i + 10) + "");
            } else {
                System.out.println("Error while trying to write register value");
            }
        }

        for (int i = 0; i < 6; i++) {
            res = sdk.GetParameterInteger("board0:/MMCComponents/CREG_1.CH" + i, value);
            if (res == 0) {
                System.out.println("Register CREG_0.CH" + i + " value is: " + value.get());
            } else {
                System.out.println("Error while trying to read register value");
            }
        }

        // set register file REG_0...3 and readback 5..6th registers
        res = sdk.SetParameterInteger("board0:/MMCComponents/REGFILE_0.REG_0", 10);
        res = sdk.SetParameterInteger("board0:/MMCComponents/REGFILE_0.REG_1", 20);
        res = sdk.SetParameterInteger("board0:/MMCComponents/REGFILE_0.REG_2", 50);
        res = sdk.SetParameterInteger("board0:/MMCComponents/REGFILE_0.REG_3", 5);

        for (int i = 0; i < 2; i++) {
            res = sdk.GetParameterInteger("board0:/MMCComponents/REGFILE_0.REG_" + (i + 4), value);
            if (res == 0) {
                System.out.println("Register REGFILE_0.REG_" + (i + 5) + " value is " + value.get());
            } else {
                System.out.println("Error while trying to read register value");
            }
        }

        sdk.DetachDevice("board0");
    }
}
