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
        [ "6.2.1 Example: how to compile and install the DT1260 libraries and SciSDK on Ubuntu", "index.html#autotoc_md12", [
          [ "UPDATE YOUR SYSTEM", "index.html#autotoc_md13", null ],
          [ "INSTALL FTDI", "index.html#autotoc_md14", null ],
          [ "COMPILE SCIDK (DT1260 Libraries)", "index.html#autotoc_md15", null ],
          [ "COMPILE SCISDK (SciCompiler Development Kit)", "index.html#autotoc_md16", null ],
          [ "CREATE A CUSTOM APPLICATION", "index.html#autotoc_md17", null ]
        ] ]
      ] ],
      [ "7 Use with Python", "index.html#autotoc_md18", null ],
      [ "8 Use with Java", "index.html#autotoc_md19", [
        [ "8.1 Include in an Ant managed project", "index.html#autotoc_md20", null ],
        [ "8.2 Include in a Maven managed project", "index.html#autotoc_md21", null ],
        [ "8.3. Instantiate the library", "index.html#autotoc_md22", null ],
        [ "8.4 Connecting to a board", "index.html#autotoc_md23", null ],
        [ "8.5 Set/Get a register value", "index.html#autotoc_md24", null ],
        [ "8.6 Allocate buffers for readout", "index.html#autotoc_md25", null ],
        [ "8.7 Readout data from a board", "index.html#autotoc_md26", null ]
      ] ],
      [ "9 Use in LabView", "index.html#autotoc_md27", [
        [ "9.1 Install automatically with SciSDK setup", "index.html#autotoc_md28", null ],
        [ "9.2 Manually add VIs to palette", "index.html#autotoc_md29", null ],
        [ "9.3 Use SciSDK VIs without add them to LabView Palette", "index.html#autotoc_md30", null ],
        [ "9.4 Instantiate the library", "index.html#autotoc_md31", null ],
        [ "9.5 Connecting to board", "index.html#autotoc_md32", null ],
        [ "9.6 Get a register value", "index.html#autotoc_md33", null ],
        [ "9.7 Set a register value", "index.html#autotoc_md34", null ],
        [ "9.8 Readout data from a board", "index.html#autotoc_md35", null ]
      ] ]
    ] ],
    [ "SciCompiler internal bus", "md_docs_mmc.html", null ],
    [ "Simple Register", "md_docs_registers.html", [
      [ "Endpoint", "md_docs_registers.html#autotoc_md38", null ],
      [ "Methods", "md_docs_registers.html#autotoc_md39", null ],
      [ "Example", "md_docs_registers.html#autotoc_md40", [
        [ "C", "md_docs_registers.html#autotoc_md41", null ],
        [ "C++", "md_docs_registers.html#autotoc_md42", null ],
        [ "C Sharp", "md_docs_registers.html#autotoc_md43", null ],
        [ "Python", "md_docs_registers.html#autotoc_md44", null ],
        [ "JAVA", "md_docs_registers.html#autotoc_md45", [
          [ "Labview", "md_docs_registers.html#autotoc_md46", null ]
        ] ]
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
          [ "JAVA", "md_docs_registers_file.html#autotoc_md55", null ],
          [ "Labview", "md_docs_registers_file.html#autotoc_md56", null ]
        ] ]
      ] ]
    ] ],
    [ "Oscilloscope driver", "md_docs_oscilloscope.html", [
      [ "Parameters", "md_docs_oscilloscope.html#autotoc_md58", [
        [ "Decimation", "md_docs_oscilloscope.html#autotoc_md59", null ],
        [ "Trigger", "md_docs_oscilloscope.html#autotoc_md60", null ],
        [ "Acquisition Modes", "md_docs_oscilloscope.html#autotoc_md61", null ],
        [ "Polarity", "md_docs_oscilloscope.html#autotoc_md62", null ],
        [ "Auto Arm", "md_docs_oscilloscope.html#autotoc_md63", null ],
        [ "Data Processing", "md_docs_oscilloscope.html#autotoc_md64", null ]
      ] ],
      [ "Commands", "md_docs_oscilloscope.html#autotoc_md65", null ],
      [ "Output data Format", "md_docs_oscilloscope.html#autotoc_md66", [
        [ "Raw Data", "md_docs_oscilloscope.html#autotoc_md67", null ],
        [ "Decoded", "md_docs_oscilloscope.html#autotoc_md68", null ]
      ] ],
      [ "Basic Examples", "md_docs_oscilloscope.html#autotoc_md69", [
        [ "C", "md_docs_oscilloscope.html#autotoc_md70", null ],
        [ "C++", "md_docs_oscilloscope.html#autotoc_md71", null ],
        [ "Python", "md_docs_oscilloscope.html#autotoc_md72", null ],
        [ "C Sharp", "md_docs_oscilloscope.html#autotoc_md73", null ],
        [ "VB.NET", "md_docs_oscilloscope.html#autotoc_md74", null ],
        [ "JAVA", "md_docs_oscilloscope.html#autotoc_md75", null ],
        [ "Labview", "md_docs_oscilloscope.html#autotoc_md76", null ]
      ] ],
      [ "Additional Examples", "md_docs_oscilloscope.html#autotoc_md77", [
        [ "Trigger on external signal", "md_docs_oscilloscope.html#autotoc_md78", null ],
        [ "Trigger on analog signal", "md_docs_oscilloscope.html#autotoc_md79", null ],
        [ "Non blocking mode", "md_docs_oscilloscope.html#autotoc_md80", null ],
        [ "Processing decoded data and store in a file, one channel per column", "md_docs_oscilloscope.html#autotoc_md81", null ],
        [ "Processing decoded data and store in a file, one channel per column", "md_docs_oscilloscope.html#autotoc_md82", null ]
      ] ]
    ] ],
    [ "Oscilloscope Dual driver", "md_docs_oscilloscope_dual.html", [
      [ "Data structure", "md_docs_oscilloscope_dual.html#autotoc_md84", null ],
      [ "Raw data formware", "md_docs_oscilloscope_dual.html#autotoc_md85", null ]
    ] ],
    [ "Wave Digitizer driver", "md_docs_digitizer.html", [
      [ "Parameters", "md_docs_digitizer.html#autotoc_md87", [
        [ "Acquisition length", "md_docs_digitizer.html#autotoc_md88", null ],
        [ "Blocking/Non blocking mode", "md_docs_digitizer.html#autotoc_md89", null ],
        [ "Enable channels", "md_docs_digitizer.html#autotoc_md90", null ]
      ] ],
      [ "Data processing mode", "md_docs_digitizer.html#autotoc_md91", null ],
      [ "Commands", "md_docs_digitizer.html#autotoc_md92", null ],
      [ "Buffer Allocation", "md_docs_digitizer.html#autotoc_md93", null ],
      [ "Output data Format", "md_docs_digitizer.html#autotoc_md94", null ],
      [ "Basic Example", "md_docs_digitizer.html#autotoc_md95", [
        [ "C", "md_docs_digitizer.html#autotoc_md96", null ],
        [ "C++", "md_docs_digitizer.html#autotoc_md97", null ],
        [ "Python", "md_docs_digitizer.html#autotoc_md98", null ],
        [ "CSharp", "md_docs_digitizer.html#autotoc_md99", null ],
        [ "VB.NET", "md_docs_digitizer.html#autotoc_md100", null ],
        [ "JAVA", "md_docs_digitizer.html#autotoc_md101", null ],
        [ "Labview", "md_docs_digitizer.html#autotoc_md102", null ]
      ] ],
      [ "Additional Examples", "md_docs_digitizer.html#autotoc_md103", [
        [ "Read decoded data and write on file, on channel per column", "md_docs_digitizer.html#autotoc_md104", null ],
        [ "Read raw data and write on file", "md_docs_digitizer.html#autotoc_md105", null ]
      ] ]
    ] ],
    [ "Custom Packet driver", "md_docs_custompacket.html", [
      [ "Parameters", "md_docs_custompacket.html#autotoc_md107", [
        [ "Acquisition length", "md_docs_custompacket.html#autotoc_md108", null ],
        [ "Blocking/Non blocking mode", "md_docs_custompacket.html#autotoc_md109", null ],
        [ "Threaded mode", "md_docs_custompacket.html#autotoc_md110", null ],
        [ "High performance mode", "md_docs_custompacket.html#autotoc_md111", null ]
      ] ],
      [ "Allignment check", "md_docs_custompacket.html#autotoc_md112", null ],
      [ "Data processing mode", "md_docs_custompacket.html#autotoc_md113", null ],
      [ "Commands", "md_docs_custompacket.html#autotoc_md114", null ],
      [ "Buffer Allocation", "md_docs_custompacket.html#autotoc_md115", null ],
      [ "Output data Format", "md_docs_custompacket.html#autotoc_md116", [
        [ "Raw Data", "md_docs_custompacket.html#autotoc_md117", null ],
        [ "Decoded Data", "md_docs_custompacket.html#autotoc_md118", null ]
      ] ],
      [ "Basic Examples", "md_docs_custompacket.html#autotoc_md119", [
        [ "C", "md_docs_custompacket.html#autotoc_md120", null ],
        [ "C++", "md_docs_custompacket.html#autotoc_md121", null ],
        [ "Python", "md_docs_custompacket.html#autotoc_md122", null ],
        [ "C Sharp", "md_docs_custompacket.html#autotoc_md123", null ],
        [ "VB.NET", "md_docs_custompacket.html#autotoc_md124", null ],
        [ "JAVA", "md_docs_custompacket.html#autotoc_md125", null ],
        [ "Labview Single", "md_docs_custompacket.html#autotoc_md126", null ],
        [ "Labview Multiple", "md_docs_custompacket.html#autotoc_md127", null ]
      ] ],
      [ "Additional Examples", "md_docs_custompacket.html#autotoc_md128", [
        [ "Decode list data in C", "md_docs_custompacket.html#autotoc_md129", null ],
        [ "Use non-blocking mode in C", "md_docs_custompacket.html#autotoc_md130", null ],
        [ "Dump raw data in C and than open it in python and decode it", "md_docs_custompacket.html#autotoc_md131", null ]
      ] ]
    ] ],
    [ "Energy Spectrum", "md_docs_spectrum.html", [
      [ "Parameters", "md_docs_spectrum.html#autotoc_md133", [
        [ "Commands", "md_docs_spectrum.html#autotoc_md137", [
          [ "Rebinning", "md_docs_spectrum.html#autotoc_md134", null ],
          [ "Min and Max", "md_docs_spectrum.html#autotoc_md135", null ],
          [ "Limit mode", "md_docs_spectrum.html#autotoc_md136", null ],
          [ "Resets the spectrum", "md_docs_spectrum.html#autotoc_md138", null ],
          [ "Reset counters", "md_docs_spectrum.html#autotoc_md139", null ]
        ] ],
        [ "Output data Format", "md_docs_spectrum.html#autotoc_md140", [
          [ "Decoded Data", "md_docs_spectrum.html#autotoc_md141", null ]
        ] ],
        [ "Status", "md_docs_spectrum.html#autotoc_md142", null ],
        [ "Basic Examples", "md_docs_spectrum.html#autotoc_md143", [
          [ "C", "md_docs_spectrum.html#autotoc_md144", null ],
          [ "C++", "md_docs_spectrum.html#autotoc_md145", null ],
          [ "Python", "md_docs_spectrum.html#autotoc_md146", null ],
          [ "C Sharp", "md_docs_spectrum.html#autotoc_md147", null ],
          [ "VB.NET", "md_docs_spectrum.html#autotoc_md148", null ],
          [ "JAVA", "md_docs_spectrum.html#autotoc_md149", null ],
          [ "Labview", "md_docs_spectrum.html#autotoc_md150", null ]
        ] ],
        [ "Additional Examples", "md_docs_spectrum.html#autotoc_md151", [
          [ "Print spectrum", "md_docs_spectrum.html#autotoc_md152", null ],
          [ "Limit and progress", "md_docs_spectrum.html#autotoc_md153", null ],
          [ "Rebin example", "md_docs_spectrum.html#autotoc_md154", null ]
        ] ]
      ] ]
    ] ],
    [ "Time of Flight Spectrum", "md_docs_tof_spectrum.html", [
      [ "Parameters", "md_docs_tof_spectrum.html#autotoc_md156", [
        [ "Commands", "md_docs_tof_spectrum.html#autotoc_md159", [
          [ "Bin width", "md_docs_tof_spectrum.html#autotoc_md157", null ],
          [ "Start delay", "md_docs_tof_spectrum.html#autotoc_md158", null ],
          [ "Resets the histogram", "md_docs_tof_spectrum.html#autotoc_md160", null ]
        ] ],
        [ "Output data Format", "md_docs_tof_spectrum.html#autotoc_md161", [
          [ "Decoded Data", "md_docs_tof_spectrum.html#autotoc_md162", null ]
        ] ],
        [ "Status", "md_docs_tof_spectrum.html#autotoc_md163", null ],
        [ "Basic Examples", "md_docs_tof_spectrum.html#autotoc_md164", [
          [ "C", "md_docs_tof_spectrum.html#autotoc_md165", null ],
          [ "C++", "md_docs_tof_spectrum.html#autotoc_md166", null ],
          [ "Python", "md_docs_tof_spectrum.html#autotoc_md167", null ],
          [ "C Sharp", "md_docs_tof_spectrum.html#autotoc_md168", null ],
          [ "VB.NET", "md_docs_tof_spectrum.html#autotoc_md169", null ],
          [ "JAVA", "md_docs_tof_spectrum.html#autotoc_md170", null ],
          [ "Labview", "md_docs_tof_spectrum.html#autotoc_md171", null ]
        ] ],
        [ "Additional Examples", "md_docs_tof_spectrum.html#autotoc_md172", [
          [ "Print spectrum", "md_docs_tof_spectrum.html#autotoc_md173", null ]
        ] ]
      ] ]
    ] ],
    [ "List driver", "md_docs_list.html", [
      [ "Parameters", "md_docs_list.html#autotoc_md175", [
        [ "Acquisition length", "md_docs_list.html#autotoc_md176", null ],
        [ "Blocking/Non blocking mode", "md_docs_list.html#autotoc_md177", null ],
        [ "Threaded mode", "md_docs_list.html#autotoc_md178", null ],
        [ "High performance mode", "md_docs_list.html#autotoc_md179", null ]
      ] ],
      [ "Commands", "md_docs_list.html#autotoc_md180", null ],
      [ "Buffer Allocation", "md_docs_list.html#autotoc_md181", null ],
      [ "Output data Format", "md_docs_list.html#autotoc_md182", [
        [ "Raw Data", "md_docs_list.html#autotoc_md183", null ]
      ] ],
      [ "Basic Examples", "md_docs_list.html#autotoc_md184", [
        [ "C", "md_docs_list.html#autotoc_md185", null ],
        [ "C++", "md_docs_list.html#autotoc_md186", null ],
        [ "Python", "md_docs_list.html#autotoc_md187", null ],
        [ "C Sharp", "md_docs_list.html#autotoc_md188", null ],
        [ "VB.NET", "md_docs_list.html#autotoc_md189", null ],
        [ "JAVA", "md_docs_list.html#autotoc_md190", null ],
        [ "Labview", "md_docs_list.html#autotoc_md191", null ]
      ] ],
      [ "Additional Examples", "md_docs_list.html#autotoc_md192", [
        [ "Decode list data in C", "md_docs_list.html#autotoc_md193", null ],
        [ "Use non-blocking mode in C", "md_docs_list.html#autotoc_md194", null ]
      ] ]
    ] ],
    [ "FFT (Power Spectrum) driver", "md_docs_fft.html", [
      [ "Parameters", "md_docs_fft.html#autotoc_md196", [
        [ "Decimation", "md_docs_fft.html#autotoc_md197", null ],
        [ "Acquisition Modes", "md_docs_fft.html#autotoc_md198", null ],
        [ "Auto Arm", "md_docs_fft.html#autotoc_md199", null ],
        [ "Data Processing", "md_docs_fft.html#autotoc_md200", null ]
      ] ],
      [ "Commands", "md_docs_fft.html#autotoc_md201", null ],
      [ "Output data Format", "md_docs_fft.html#autotoc_md202", [
        [ "Raw Data", "md_docs_fft.html#autotoc_md203", null ],
        [ "Decoded", "md_docs_fft.html#autotoc_md204", null ]
      ] ],
      [ "Basic Examples", "md_docs_fft.html#autotoc_md205", [
        [ "C", "md_docs_fft.html#autotoc_md206", null ],
        [ "C++", "md_docs_fft.html#autotoc_md207", null ],
        [ "Python", "md_docs_fft.html#autotoc_md208", null ],
        [ "C Sharp", "md_docs_fft.html#autotoc_md209", null ],
        [ "VB.NET", "md_docs_fft.html#autotoc_md210", null ],
        [ "JAVA", "md_docs_fft.html#autotoc_md211", null ],
        [ "Labview", "md_docs_fft.html#autotoc_md212", null ]
      ] ],
      [ "Additional Examples", "md_docs_fft.html#autotoc_md213", [
        [ "Non blocking mode", "md_docs_fft.html#autotoc_md214", null ],
        [ "Processing decoded data and store in a file", "md_docs_fft.html#autotoc_md215", null ],
        [ "Processing raw data and store in a file", "md_docs_fft.html#autotoc_md216", null ]
      ] ]
    ] ],
    [ "Frame (imaging) driver", "md_docs_frame.html", [
      [ "Parameters", "md_docs_frame.html#autotoc_md218", [
        [ "Blocking/Non blocking mode", "md_docs_frame.html#autotoc_md219", null ],
        [ "Threaded mode", "md_docs_frame.html#autotoc_md220", null ],
        [ "High performance mode", "md_docs_frame.html#autotoc_md221", null ]
      ] ],
      [ "Commands", "md_docs_frame.html#autotoc_md222", null ],
      [ "Buffer Allocation", "md_docs_frame.html#autotoc_md223", null ],
      [ "Output data Format", "md_docs_frame.html#autotoc_md224", [
        [ "Raw Data", "md_docs_frame.html#autotoc_md225", null ],
        [ "Decoded Data", "md_docs_frame.html#autotoc_md226", null ]
      ] ],
      [ "Basic Examples", "md_docs_frame.html#autotoc_md227", [
        [ "C RAW", "md_docs_frame.html#autotoc_md228", null ],
        [ "C DECODED", "md_docs_frame.html#autotoc_md229", null ],
        [ "C++ RAW", "md_docs_frame.html#autotoc_md230", null ],
        [ "Python RAW", "md_docs_frame.html#autotoc_md231", null ],
        [ "Python Decoded", "md_docs_frame.html#autotoc_md232", null ],
        [ "C Sharp RAW", "md_docs_frame.html#autotoc_md233", null ],
        [ "VB.NET RAW", "md_docs_frame.html#autotoc_md234", null ],
        [ "JAVA DECODED", "md_docs_frame.html#autotoc_md235", null ],
        [ "Labview Single", "md_docs_frame.html#autotoc_md236", null ],
        [ "Labview Multiple", "md_docs_frame.html#autotoc_md237", null ]
      ] ],
      [ "Additional Examples", "md_docs_frame.html#autotoc_md238", [
        [ "C read and print decoded data", "md_docs_frame.html#autotoc_md239", null ],
        [ "Example of decoding RAW data in C++", "md_docs_frame.html#autotoc_md240", null ],
        [ "Example of decoding RAW data in Python", "md_docs_frame.html#autotoc_md241", null ]
      ] ]
    ] ],
    [ "Rate Meter", "md_docs_rate_meter.html", [
      [ "Parameters", "md_docs_rate_meter.html#autotoc_md243", [
        [ "Commands", "md_docs_rate_meter.html#autotoc_md244", null ],
        [ "Output data Format", "md_docs_rate_meter.html#autotoc_md245", [
          [ "Decoded Data", "md_docs_rate_meter.html#autotoc_md246", null ]
        ] ],
        [ "Basic Examples", "md_docs_rate_meter.html#autotoc_md247", [
          [ "C", "md_docs_rate_meter.html#autotoc_md248", null ],
          [ "C++", "md_docs_rate_meter.html#autotoc_md249", null ],
          [ "Python", "md_docs_rate_meter.html#autotoc_md250", null ],
          [ "C Sharp", "md_docs_rate_meter.html#autotoc_md251", null ],
          [ "VB.NET", "md_docs_rate_meter.html#autotoc_md252", null ],
          [ "JAVA", "md_docs_rate_meter.html#autotoc_md253", null ],
          [ "Labview", "md_docs_rate_meter.html#autotoc_md254", null ]
        ] ]
      ] ]
    ] ],
    [ "Citiroc Configuration driver", "md_docs_citiroc_config.html", null ],
    [ "Citiroc Frame transfer driver", "md_docs_citiroc_frame.html", null ],
    [ "Petiroc Configuration driver", "md_docs_petiroc_config.html", null ],
    [ "Petiroc Frame Transfer driver", "md_docs_petiroc_frame.html", null ],
    [ "Board Endpoint driver", "md_docs_boardendpoint.html", [
      [ "Supported functions:", "md_docs_boardendpoint.html#autotoc_md260", null ],
      [ "Board specific endpoint", "md_docs_boardendpoint.html#autotoc_md261", null ]
    ] ],
    [ "OpenDPP Board Driver", "md_docs_boards_opendpp.html", [
      [ "Access to board endpoints", "md_docs_boards_opendpp.html#autotoc_md263", null ],
      [ "OpenDPP data structure", "md_docs_boards_opendpp.html#autotoc_md264", null ],
      [ "Code Examples", "md_docs_boards_opendpp.html#autotoc_md265", null ]
    ] ],
    [ "OpenScope Board Driver", "md_docs_boards_openscope.html", [
      [ "Access to board endpoints", "md_docs_boards_openscope.html#autotoc_md267", null ],
      [ "OpenScope data structure", "md_docs_boards_openscope.html#autotoc_md268", null ],
      [ "Code Examples", "md_docs_boards_openscope.html#autotoc_md269", null ]
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
"md_docs_fft.html#autotoc_md202",
"struct_s_c_i_s_d_k___f_e___o_p_e_n_d_p_p___e_v_e_n_t.html#a0735fd381d74266b5a08dbb247ed6023"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';