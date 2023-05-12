/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "SciSDK Library", "index.html", [
    [ "SciSDK Library Introduction", "index.html", [
      [ "1. Introduction", "index.html#autotoc_md1", null ],
      [ "2. SciSDK vs Legacy SDK", "index.html#autotoc_md2", [
        [ "2.1. Legacy SDK", "index.html#autotoc_md3", null ],
        [ "2.2. SciSDK", "index.html#autotoc_md4", null ]
      ] ],
      [ "3. SciSDK Structure", "index.html#autotoc_md5", [
        [ "File structure", "index.html#autotoc_md6", null ]
      ] ],
      [ "4. Windows: Installation from binary package", "index.html#autotoc_md7", null ],
      [ "5. Linux: Installation from binary package", "index.html#autotoc_md8", null ],
      [ "6. Compiling from source", "index.html#autotoc_md9", [
        [ "6.1. Compiling for Windows", "index.html#autotoc_md10", null ],
        [ "6.2. Compiling for Linux", "index.html#autotoc_md11", null ],
        [ "6.3. Compiling on Raspberry Pi", "index.html#autotoc_md12", null ],
        [ "6.4. Cross-compiling for arm, aaarch64", "index.html#autotoc_md13", null ]
      ] ],
      [ "7. Usage", "index.html#autotoc_md14", [
        [ "7.1. Create a VS2022 project linking to the library", "index.html#autotoc_md15", null ],
        [ "7.2. Create a g++ project linking to the library", "index.html#autotoc_md16", null ],
        [ "7.3. Instantiate the library", "index.html#autotoc_md17", null ],
        [ "7.4. Connecting to a board", "index.html#autotoc_md18", null ],
        [ "7.5. Set/Get a register value", "index.html#autotoc_md19", null ],
        [ "7.6. Allocate buffers for readout", "index.html#autotoc_md20", null ],
        [ "7.7. Readout data from a board", "index.html#autotoc_md21", null ]
      ] ],
      [ "8 Use with Java", "index.html#autotoc_md22", [
        [ "8.1 Include in an Ant managed project", "index.html#autotoc_md23", null ],
        [ "8.2 Include in a Maven managed project", "index.html#autotoc_md24", null ],
        [ "8.3. Instantiate the library", "index.html#autotoc_md25", null ],
        [ "8.4 Connecting to a board", "index.html#autotoc_md26", null ],
        [ "8.5 Set/Get a register value", "index.html#autotoc_md27", null ],
        [ "8.6 Allocate buffers for readout", "index.html#autotoc_md28", null ],
        [ "8.7 Readout data from a board", "index.html#autotoc_md29", null ]
      ] ],
      [ "9 Use in LabView", "index.html#autotoc_md30", [
        [ "9.1 Installation", "index.html#autotoc_md31", null ],
        [ "9.2 Instantiate the library", "index.html#autotoc_md32", null ],
        [ "9.3 Connecting to board", "index.html#autotoc_md33", null ],
        [ "9.4 Get a register value", "index.html#autotoc_md34", null ],
        [ "9.5 Set a register value", "index.html#autotoc_md35", null ],
        [ "9.6 Readout data from a board", "index.html#autotoc_md36", null ]
      ] ]
    ] ],
    [ "SciCompiler internal bus", "md_docs_mmc.html", null ],
    [ "Simple Register", "md_docs_registers.html", [
      [ "Endpoint", "md_docs_registers.html#autotoc_md39", null ],
      [ "Methods", "md_docs_registers.html#autotoc_md40", null ],
      [ "Example", "md_docs_registers.html#autotoc_md41", [
        [ "C", "md_docs_registers.html#autotoc_md42", null ],
        [ "C++", "md_docs_registers.html#autotoc_md43", null ],
        [ "C Sharp", "md_docs_registers.html#autotoc_md44", null ],
        [ "Python", "md_docs_registers.html#autotoc_md45", null ],
        [ "JAVA", "md_docs_registers.html#autotoc_md46", null ]
      ] ]
    ] ],
    [ "Register File", "md_docs_registers_file.html", [
      [ "Endpoint", "md_docs_registers_file.html#autotoc_md48", null ],
      [ "Methods", "md_docs_registers_file.html#autotoc_md49", null ],
      [ "Example", "md_docs_registers_file.html#autotoc_md50", [
        [ "C", "md_docs_registers_file.html#autotoc_md51", null ],
        [ "C++", "md_docs_registers_file.html#autotoc_md52", null ],
        [ "C Sharp", "md_docs_registers_file.html#autotoc_md53", null ],
        [ "Python", "md_docs_registers_file.html#autotoc_md54", [
          [ "JAVA", "md_docs_registers_file.html#autotoc_md55", null ]
        ] ]
      ] ]
    ] ],
    [ "Oscilloscope driver", "md_docs_oscilloscope.html", [
      [ "Parameters", "md_docs_oscilloscope.html#autotoc_md57", [
        [ "Decimation", "md_docs_oscilloscope.html#autotoc_md58", null ],
        [ "Trigger", "md_docs_oscilloscope.html#autotoc_md59", null ],
        [ "Acquisition Modes", "md_docs_oscilloscope.html#autotoc_md60", null ],
        [ "Polarity", "md_docs_oscilloscope.html#autotoc_md61", null ],
        [ "Auto Arm", "md_docs_oscilloscope.html#autotoc_md62", null ],
        [ "Data Processing", "md_docs_oscilloscope.html#autotoc_md63", null ]
      ] ],
      [ "Commands", "md_docs_oscilloscope.html#autotoc_md64", null ],
      [ "Output data Format", "md_docs_oscilloscope.html#autotoc_md65", [
        [ "Raw Data", "md_docs_oscilloscope.html#autotoc_md66", null ],
        [ "Decoded", "md_docs_oscilloscope.html#autotoc_md67", null ]
      ] ],
      [ "Basic Examples", "md_docs_oscilloscope.html#autotoc_md68", [
        [ "C", "md_docs_oscilloscope.html#autotoc_md69", null ],
        [ "C++", "md_docs_oscilloscope.html#autotoc_md70", null ],
        [ "Python", "md_docs_oscilloscope.html#autotoc_md71", null ],
        [ "C Sharp", "md_docs_oscilloscope.html#autotoc_md72", null ],
        [ "VB.NET", "md_docs_oscilloscope.html#autotoc_md73", null ],
        [ "JAVA", "md_docs_oscilloscope.html#autotoc_md74", null ],
        [ "Labview", "md_docs_oscilloscope.html#autotoc_md75", null ]
      ] ],
      [ "Additional Examples", "md_docs_oscilloscope.html#autotoc_md76", [
        [ "Trigger on external signal", "md_docs_oscilloscope.html#autotoc_md77", null ],
        [ "Trigger on analog signal", "md_docs_oscilloscope.html#autotoc_md78", null ],
        [ "Non blocking mode", "md_docs_oscilloscope.html#autotoc_md79", null ],
        [ "Processing decoded data and store in a file, one channel per column", "md_docs_oscilloscope.html#autotoc_md80", null ],
        [ "Processing decoded data and store in a file, one channel per column", "md_docs_oscilloscope.html#autotoc_md81", null ]
      ] ]
    ] ],
    [ "Oscilloscope Dual driver", "md_docs_oscilloscope_dual.html", [
      [ "Data structure", "md_docs_oscilloscope_dual.html#autotoc_md83", null ],
      [ "Raw data formware", "md_docs_oscilloscope_dual.html#autotoc_md84", null ]
    ] ],
    [ "Wave Digitizer driver", "md_docs_digitizer.html", [
      [ "Parameters", "md_docs_digitizer.html#autotoc_md86", [
        [ "Acquisition length", "md_docs_digitizer.html#autotoc_md87", null ],
        [ "Blocking/Non blocking mode", "md_docs_digitizer.html#autotoc_md88", null ],
        [ "Enable channels", "md_docs_digitizer.html#autotoc_md89", null ]
      ] ],
      [ "Data processing mode", "md_docs_digitizer.html#autotoc_md90", null ],
      [ "Commands", "md_docs_digitizer.html#autotoc_md91", null ],
      [ "Buffer Allocation", "md_docs_digitizer.html#autotoc_md92", null ],
      [ "Output data Format", "md_docs_digitizer.html#autotoc_md93", null ],
      [ "Basic Example", "md_docs_digitizer.html#autotoc_md94", [
        [ "C", "md_docs_digitizer.html#autotoc_md95", null ],
        [ "C++", "md_docs_digitizer.html#autotoc_md96", null ],
        [ "Python", "md_docs_digitizer.html#autotoc_md97", null ],
        [ "CSharp", "md_docs_digitizer.html#autotoc_md98", null ],
        [ "VB.NET", "md_docs_digitizer.html#autotoc_md99", null ],
        [ "JAVA", "md_docs_digitizer.html#autotoc_md100", null ],
        [ "Labview", "md_docs_digitizer.html#autotoc_md101", null ]
      ] ],
      [ "Additional Examples", "md_docs_digitizer.html#autotoc_md102", [
        [ "Read decoded data and write on file, on channel per column", "md_docs_digitizer.html#autotoc_md103", null ],
        [ "Read raw data and write on file", "md_docs_digitizer.html#autotoc_md104", null ]
      ] ]
    ] ],
    [ "Custom Packet driver", "md_docs_custompacket.html", [
      [ "Parameters", "md_docs_custompacket.html#autotoc_md106", [
        [ "Acquisition length", "md_docs_custompacket.html#autotoc_md107", null ],
        [ "Blocking/Non blocking mode", "md_docs_custompacket.html#autotoc_md108", null ],
        [ "Threaded mode", "md_docs_custompacket.html#autotoc_md109", null ],
        [ "High performance mode", "md_docs_custompacket.html#autotoc_md110", null ]
      ] ],
      [ "Allignment check", "md_docs_custompacket.html#autotoc_md111", null ],
      [ "Data processing mode", "md_docs_custompacket.html#autotoc_md112", null ],
      [ "Commands", "md_docs_custompacket.html#autotoc_md113", null ],
      [ "Buffer Allocation", "md_docs_custompacket.html#autotoc_md114", null ],
      [ "Output data Format", "md_docs_custompacket.html#autotoc_md115", [
        [ "Raw Data", "md_docs_custompacket.html#autotoc_md116", null ],
        [ "Decoded Data", "md_docs_custompacket.html#autotoc_md117", null ]
      ] ],
      [ "Basic Examples", "md_docs_custompacket.html#autotoc_md118", [
        [ "C", "md_docs_custompacket.html#autotoc_md119", null ],
        [ "C++", "md_docs_custompacket.html#autotoc_md120", null ],
        [ "Python", "md_docs_custompacket.html#autotoc_md121", null ],
        [ "C Sharp", "md_docs_custompacket.html#autotoc_md122", null ],
        [ "VB.NET", "md_docs_custompacket.html#autotoc_md123", null ],
        [ "JAVA", "md_docs_custompacket.html#autotoc_md124", null ],
        [ "Labview Single", "md_docs_custompacket.html#autotoc_md125", null ],
        [ "Labview Multiple", "md_docs_custompacket.html#autotoc_md126", null ]
      ] ],
      [ "Additional Examples", "md_docs_custompacket.html#autotoc_md127", [
        [ "Decode list data in C", "md_docs_custompacket.html#autotoc_md128", null ],
        [ "Use non-blocking mode in C", "md_docs_custompacket.html#autotoc_md129", null ],
        [ "Dump raw data in C and than open it in python and decode it", "md_docs_custompacket.html#autotoc_md130", null ]
      ] ]
    ] ],
    [ "Energy Spectrum", "md_docs_spectrum.html", [
      [ "Parameters", "md_docs_spectrum.html#autotoc_md132", [
        [ "Commands", "md_docs_spectrum.html#autotoc_md136", [
          [ "Rebinning", "md_docs_spectrum.html#autotoc_md133", null ],
          [ "Min and Max", "md_docs_spectrum.html#autotoc_md134", null ],
          [ "Limit mode", "md_docs_spectrum.html#autotoc_md135", null ],
          [ "Resets the spectrum", "md_docs_spectrum.html#autotoc_md137", null ],
          [ "Reset counters", "md_docs_spectrum.html#autotoc_md138", null ]
        ] ],
        [ "Output data Format", "md_docs_spectrum.html#autotoc_md139", [
          [ "Decoded Data", "md_docs_spectrum.html#autotoc_md140", null ]
        ] ],
        [ "Status", "md_docs_spectrum.html#autotoc_md141", null ],
        [ "Basic Examples", "md_docs_spectrum.html#autotoc_md142", [
          [ "C", "md_docs_spectrum.html#autotoc_md143", null ],
          [ "C++", "md_docs_spectrum.html#autotoc_md144", null ],
          [ "Python", "md_docs_spectrum.html#autotoc_md145", null ],
          [ "C Sharp", "md_docs_spectrum.html#autotoc_md146", null ],
          [ "VB.NET", "md_docs_spectrum.html#autotoc_md147", null ],
          [ "JAVA", "md_docs_spectrum.html#autotoc_md148", null ],
          [ "Labview", "md_docs_spectrum.html#autotoc_md149", null ]
        ] ],
        [ "Additional Examples", "md_docs_spectrum.html#autotoc_md150", [
          [ "Print spectrum", "md_docs_spectrum.html#autotoc_md151", null ],
          [ "Limit and progress", "md_docs_spectrum.html#autotoc_md152", null ],
          [ "Rebin example", "md_docs_spectrum.html#autotoc_md153", null ]
        ] ]
      ] ]
    ] ],
    [ "Time of Flight Spectrum", "md_docs_tof_spectrum.html", [
      [ "Parameters", "md_docs_tof_spectrum.html#autotoc_md155", [
        [ "Commands", "md_docs_tof_spectrum.html#autotoc_md158", [
          [ "Bin width", "md_docs_tof_spectrum.html#autotoc_md156", null ],
          [ "Start delay", "md_docs_tof_spectrum.html#autotoc_md157", null ],
          [ "Resets the histogram", "md_docs_tof_spectrum.html#autotoc_md159", null ]
        ] ],
        [ "Output data Format", "md_docs_tof_spectrum.html#autotoc_md160", [
          [ "Decoded Data", "md_docs_tof_spectrum.html#autotoc_md161", null ]
        ] ],
        [ "Status", "md_docs_tof_spectrum.html#autotoc_md162", null ],
        [ "Basic Examples", "md_docs_tof_spectrum.html#autotoc_md163", [
          [ "C", "md_docs_tof_spectrum.html#autotoc_md164", null ],
          [ "C++", "md_docs_tof_spectrum.html#autotoc_md165", null ],
          [ "Python", "md_docs_tof_spectrum.html#autotoc_md166", null ],
          [ "C Sharp", "md_docs_tof_spectrum.html#autotoc_md167", null ],
          [ "VB.NET", "md_docs_tof_spectrum.html#autotoc_md168", null ],
          [ "JAVA", "md_docs_tof_spectrum.html#autotoc_md169", null ],
          [ "Labview", "md_docs_tof_spectrum.html#autotoc_md170", null ]
        ] ],
        [ "Additional Examples", "md_docs_tof_spectrum.html#autotoc_md171", [
          [ "Print spectrum", "md_docs_tof_spectrum.html#autotoc_md172", null ]
        ] ]
      ] ]
    ] ],
    [ "List driver", "md_docs_list.html", [
      [ "Parameters", "md_docs_list.html#autotoc_md174", [
        [ "Acquisition length", "md_docs_list.html#autotoc_md175", null ],
        [ "Blocking/Non blocking mode", "md_docs_list.html#autotoc_md176", null ],
        [ "Threaded mode", "md_docs_list.html#autotoc_md177", null ],
        [ "High performance mode", "md_docs_list.html#autotoc_md178", null ]
      ] ],
      [ "Commands", "md_docs_list.html#autotoc_md179", null ],
      [ "Buffer Allocation", "md_docs_list.html#autotoc_md180", null ],
      [ "Output data Format", "md_docs_list.html#autotoc_md181", [
        [ "Raw Data", "md_docs_list.html#autotoc_md182", null ]
      ] ],
      [ "Basic Examples", "md_docs_list.html#autotoc_md183", [
        [ "C", "md_docs_list.html#autotoc_md184", null ],
        [ "C++", "md_docs_list.html#autotoc_md185", null ],
        [ "Python", "md_docs_list.html#autotoc_md186", null ],
        [ "C Sharp", "md_docs_list.html#autotoc_md187", null ],
        [ "VB.NET", "md_docs_list.html#autotoc_md188", null ],
        [ "JAVA", "md_docs_list.html#autotoc_md189", null ],
        [ "Labview", "md_docs_list.html#autotoc_md190", null ]
      ] ],
      [ "Additional Examples", "md_docs_list.html#autotoc_md191", [
        [ "Decode list data in C", "md_docs_list.html#autotoc_md192", null ],
        [ "Use non-blocking mode in C", "md_docs_list.html#autotoc_md193", null ]
      ] ]
    ] ],
    [ "FFT (Power Spectrum) driver", "md_docs_fft.html", [
      [ "Parameters", "md_docs_fft.html#autotoc_md195", [
        [ "Decimation", "md_docs_fft.html#autotoc_md196", null ],
        [ "Acquisition Modes", "md_docs_fft.html#autotoc_md197", null ],
        [ "Auto Arm", "md_docs_fft.html#autotoc_md198", null ],
        [ "Data Processing", "md_docs_fft.html#autotoc_md199", null ]
      ] ],
      [ "Commands", "md_docs_fft.html#autotoc_md200", null ],
      [ "Output data Format", "md_docs_fft.html#autotoc_md201", [
        [ "Raw Data", "md_docs_fft.html#autotoc_md202", null ],
        [ "Decoded", "md_docs_fft.html#autotoc_md203", null ]
      ] ],
      [ "Basic Examples", "md_docs_fft.html#autotoc_md204", [
        [ "C", "md_docs_fft.html#autotoc_md205", null ],
        [ "C++", "md_docs_fft.html#autotoc_md206", null ],
        [ "Python", "md_docs_fft.html#autotoc_md207", null ],
        [ "C Sharp", "md_docs_fft.html#autotoc_md208", null ],
        [ "VB.NET", "md_docs_fft.html#autotoc_md209", null ],
        [ "JAVA", "md_docs_fft.html#autotoc_md210", null ],
        [ "Labview", "md_docs_fft.html#autotoc_md211", null ]
      ] ],
      [ "Additional Examples", "md_docs_fft.html#autotoc_md212", [
        [ "Non blocking mode", "md_docs_fft.html#autotoc_md213", null ],
        [ "Processing decoded data and store in a file", "md_docs_fft.html#autotoc_md214", null ],
        [ "Processing raw data and store in a file", "md_docs_fft.html#autotoc_md215", null ]
      ] ]
    ] ],
    [ "Frame (imaging) driver", "md_docs_frame.html", [
      [ "Parameters", "md_docs_frame.html#autotoc_md217", [
        [ "Blocking/Non blocking mode", "md_docs_frame.html#autotoc_md218", null ],
        [ "Threaded mode", "md_docs_frame.html#autotoc_md219", null ],
        [ "High performance mode", "md_docs_frame.html#autotoc_md220", null ]
      ] ],
      [ "Commands", "md_docs_frame.html#autotoc_md221", null ],
      [ "Buffer Allocation", "md_docs_frame.html#autotoc_md222", null ],
      [ "Output data Format", "md_docs_frame.html#autotoc_md223", [
        [ "Raw Data", "md_docs_frame.html#autotoc_md224", null ],
        [ "Decoded Data", "md_docs_frame.html#autotoc_md225", null ]
      ] ],
      [ "Basic Examples", "md_docs_frame.html#autotoc_md226", [
        [ "C RAW", "md_docs_frame.html#autotoc_md227", null ],
        [ "C DECODED", "md_docs_frame.html#autotoc_md228", null ],
        [ "C++ RAW", "md_docs_frame.html#autotoc_md229", null ],
        [ "Python RAW", "md_docs_frame.html#autotoc_md230", null ],
        [ "Python Decoded", "md_docs_frame.html#autotoc_md231", null ],
        [ "C Sharp RAW", "md_docs_frame.html#autotoc_md232", null ],
        [ "VB.NET RAW", "md_docs_frame.html#autotoc_md233", null ],
        [ "JAVA DECODED", "md_docs_frame.html#autotoc_md234", null ],
        [ "Labview Single", "md_docs_frame.html#autotoc_md235", null ],
        [ "Labview Multiple", "md_docs_frame.html#autotoc_md236", null ]
      ] ],
      [ "Additional Examples", "md_docs_frame.html#autotoc_md237", [
        [ "C read and print decoded data", "md_docs_frame.html#autotoc_md238", null ],
        [ "Example of decoding RAW data in C++", "md_docs_frame.html#autotoc_md239", null ],
        [ "Example of decoding RAW data in Python", "md_docs_frame.html#autotoc_md240", null ]
      ] ]
    ] ],
    [ "Rate Meter", "md_docs_rate_meter.html", [
      [ "Parameters", "md_docs_rate_meter.html#autotoc_md242", [
        [ "Commands", "md_docs_rate_meter.html#autotoc_md243", null ],
        [ "Output data Format", "md_docs_rate_meter.html#autotoc_md244", [
          [ "Decoded Data", "md_docs_rate_meter.html#autotoc_md245", null ]
        ] ],
        [ "Basic Examples", "md_docs_rate_meter.html#autotoc_md246", [
          [ "C", "md_docs_rate_meter.html#autotoc_md247", null ],
          [ "C++", "md_docs_rate_meter.html#autotoc_md248", null ],
          [ "Python", "md_docs_rate_meter.html#autotoc_md249", null ],
          [ "C Sharp", "md_docs_rate_meter.html#autotoc_md250", null ],
          [ "VB.NET", "md_docs_rate_meter.html#autotoc_md251", null ],
          [ "JAVA", "md_docs_rate_meter.html#autotoc_md252", null ],
          [ "Labview", "md_docs_rate_meter.html#autotoc_md253", null ]
        ] ]
      ] ]
    ] ],
    [ "Citiroc Configuration driver", "md_docs_citiroc_config.html", null ],
    [ "Citiroc Frame transfer driver", "md_docs_citiroc_frame.html", null ],
    [ "Petiroc Configuration driver", "md_docs_petiroc_config.html", null ],
    [ "Petiroc Frame Transfer driver", "md_docs_petiroc_frame.html", null ],
    [ "Board Endpoint driver", "md_docs_boardendpoint.html", [
      [ "Supported functions:", "md_docs_boardendpoint.html#autotoc_md259", null ],
      [ "Board specific endpoint", "md_docs_boardendpoint.html#autotoc_md260", null ]
    ] ],
    [ "OpenDPP Board Driver", "md_docs_boards_opendpp.html", [
      [ "Access to board endpoints", "md_docs_boards_opendpp.html#autotoc_md262", null ],
      [ "OpenDPP data structure", "md_docs_boards_opendpp.html#autotoc_md263", null ],
      [ "Code Examples", "md_docs_boards_opendpp.html#autotoc_md264", null ]
    ] ],
    [ "OpenScope Board Driver", "md_docs_boards_openscope.html", [
      [ "Access to board endpoints", "md_docs_boards_openscope.html#autotoc_md266", null ],
      [ "OpenScope data structure", "md_docs_boards_openscope.html#autotoc_md267", null ],
      [ "Code Examples", "md_docs_boards_openscope.html#autotoc_md268", null ]
    ] ],
    [ "DT1260 Board Driver", "md_docs_boards_dt1260.html", null ],
    [ "DT5550W with Citiroc Board Driver", "md_docs_boards_dt5550w_citiroc.html", null ],
    [ "DT5560 / R5560SE Board Driver", "md_docs_boards_x5560se.html", null ],
    [ "R5560  Board Driver", "md_docs_boards_x5560.html", null ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Variables", "functions_vars.html", "functions_vars" ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Enumerator", "globals_eval.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"_n_i_error_code_8h.html",
"md_docs_fft.html#autotoc_md203",
"struct_s_c_i_s_d_k___f_e___o_p_e_n_d_p_p___e_v_e_n_t.html#aa77878aeb359d125ed8e962e1904ecb1"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';