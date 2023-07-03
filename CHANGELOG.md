# scisdk (1.2.32)
  * changed build script to include -DSCISDK_DLL_EXPORTS in the default build procedure
  * corrected libSCRDK.so name in the hal file
  * documentation on how to buil il Linux

# scisdk (1.2.31)

  * Readme coincidence-trigger example (incomplete)
  * Fixed C interface of DLL to avoid name mangling. Must be compiled with  `make CPPFLAGS+=-DSCISDK_DLL_EXPORTS`
  * Python minimum version change: 3.5
  * node JS support
  * started to create typescript wrapper
  * Matlab support- started to create examples
  * Cern ROOT support
  * labview - docs
  * java - updated example dependency



# scisdk (1.2.28)

  * aggiunto labview to setup
  * java - added FFT example
  * java - docs
  * java - examples, added status struct
  * java - custom packet example
  * java - added frame, fixed custom packet decoded
  * labview support


# scisdk (1.2.27)

  * Example of charge integration analysis with classes.
  * transistor reset: fit, export spectrum, icr ocr
  * updated transistor reset example
  * fit on spectrum
  * adding file .NIU of the examples and gitignore in the firmware direcories
  * Fava wrapper
  * Example read oscilloscope c++


# scisdk (1.2.26)

  * compile c# debug
  * Trasnistor reset example with pyqt readout
  * new examples
  * changed register in uint32
  * c# wrapper corrected the digital decode
  * Small patch on c++ counter list
  * fixed bug writing 0 to reg
  * C++ counter list
  * python and c# counter list
  * Python file counter list readout
  * Change in documentation
  * modified setup and build script
  * path c++ examples
  * Static library for C++ in Windows Visual c++
  * modified architetcture arm64



# scisdk (2023.2.10.1)

  * fixed bug writing 0 to reg
  * Prova di lettura della lista
  * C++ counter list
  * python e c# counter list
  * Python file counter list readout
  * Readme counter list
  * Change in documentation
  * Sistemato i vari programmi
  * modified setup and build script
  * path c++ examples
  * moved examples, application notes
  * add repo.sh
  * Static library for C++ in Windows Visual c++
  * modified architetcture arm64
  * add debs
  * release files
  * changed parellel compile script
  * moved to docker heavy
  * separated linux arch
  * Update compile_all_architecures.sh
  * same change to oscilloscope dual
  * driver dt5560,
  * removed all printf
  * removed printf components
  * driver dt5560se analog control
  * release with zynq
  * o3 optimization im makefile
  * add compile_for_zynq.sh
  * Fixed ReadData Bug
  * Fixed bug on cmd_stop of FrameTransfer
  * fixed bug of ReadStatus in SciSDK_DLL.cpp
  * compile linux change makefile
  * change docs
  * add driver model dt5560se
  * board driver dt1260
  * fft example fw dt1260
  * patched frame driver
  * added example of custom packet (+fw for dt1260)
  * modified list driver to avoid to read from fifo if data are not available in blocking mode
  * scf saved for tof spectrum example
  * tof spectrum python + fw dt1260
  * python example ratemeter + fw (DT1260)
  * registers example python + fw(DT1260)
  * example spectrum (DT1260)
  * python packager
  * create whl packet
  * add digitizer example
  * linux compile script
  * compile on linux
  * linux compatibile
  * rimiosso ratemeter
  * Merge branch 'linux' into 'master'
  * bug fix di tof spectrum e oscilloscope dual
  * fix bug custom packet




# scisdk (1.0.2.0)
* Oscilloscope guide
* Legacy library documentation and introduction to the new library
* Added documentation
* Removed zip
* DPP trigger parameters
* Introduced CAEN DPP
* Fixed command sequence for acquiring on CAEN FELib
* Integrated FE CAEN board control part
* Fixed minor issues on the CAEN part
* Added bd_feelib and started BE API support
* Integrated FEElib for scicompiler support
* Added dt1260 driver and started x5560
* Fixed buffer bugs
* Other allocation issues in the wrapper fixed
* Small allocated strings in the return to DLL CPP
* List bug fixed
* Register enumeration
* Added instrument parameter initialization, push for server change
* Spectrum modifications: initializations
* Added bins and max counts
* Added read state function
* Porting on Linux, to be tested
* Started board driver
* Added references to CITIROC and PETIROC frame
* Added PETIROC frame
* Added CITIROC frame, modified error in non-threaded read frame
* Added CITIROC frame
* Digitizer bugs: maximum size, channel spacing dependent on acq_len
* Fixed digitizer WinForm C# example
* Moved "for" loop
* Completed frame
* Frame in progress
* Work on frame
* Added driver file for frame
* Console C# example
* Register start CITIROC config
* Renamed CITIROC config
* Updated .gitignore
* Python console and C# console/WinForm examples for TOF spectrum
* Added TOF spectrum
* Updated digitizer to match what was done with resource explorer
* C# dual oscilloscope console example
* C# DLL - fixed s_error issue
* Fixed buffer allocation in Python
* Completed Python wrapper library
* Using DLL from a different folder than the executable file's execution folder
* Python wrapper methods for setting and getting parameters
* Fixed C# library
* Sent previous changes
* C# DLL wrapper - finished writing methods
* C# SDK - buffer allocation for all nodes
* Started creating C# wrapper DLL for SciSDK
* C# oscilloscope example - fixed zoom
* Started creating function to list registers and MMCCOMPONENTS
* Digital trace display
* Fixed folder structure of C# oscilloscope example, added console-only example project
* Completed analog trace graph display
* Started graph display
* Completed reading data in C# oscilloscope example
* Read-only properties for oscilloscope
* C# oscilloscope example - created form for selecting board, JSON file, and connection type
* Started creating C# oscilloscope example
* SciSDK_DLL
* Started creating wrapper for library
* Dual oscilloscope
* Fixed handles
* Integrated DT5560
* Runtime loading of DLLs
* Register read/write
* CITIROC write completed
* Finished decoding
* DT5550W-CITIROC bitstream generation
* Class to handle rate meter
* Class to handle custom packet
* Added spectrum
* Completed spectrum, added FFT
* Add spectrum
* List with threadless read and non-blocking with status register control
* List
* Parameter limits
* Digitizer
* Completed oscilloscope
* Oscilloscope reading OK, decode also OK, trigger still not working
* Oscilloscope (not tested)
* Registers and oscilloscope draft
* Added functions to access register
* Started oscilloscope implementation
* Initial commit

