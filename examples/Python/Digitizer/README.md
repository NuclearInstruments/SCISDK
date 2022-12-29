# Digitizer Example
This is a simple example of how to use the digitizer. 
This example is based on matplotlib to display an updating window with the oscilloscope waveform.
It configure the digitizer to capture 8000 samples from channel 1.
The trigger register trg_mode is not set indeed by default it is set to 0: that means auto mode. 
(Look at the firmware)



## Requirements

- Python 3.7 or higher
- scisdk library installed in the system (https://github.com/NuclearInstruments/SCISDK/releases)
- matplotlib installed in the system (https://pypi.org/project/matplotlib)
```bash
    pip install matplotlib
```
- scisdk python wrapper installed in the system (https://pypi.org/project/scisdk)
```bash
    pip install scisdk
```

## Instructions

- Upload on the board the digitizer example firmware/<board>/Digitizer
- Select the proper connection string for the board in the script editing the code
- Run the example
- A window will open with the oscilloscope waveform
 