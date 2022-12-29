# Oscilloscope Example
This is a simple example of how to use the oscilloscope. 
This example is console based code.
It configure the oscilloscope to capture all avaialble channels and samples with a self trigger 
(self means with not trigger, it just start to capture the waveform as soon as the script is run).
Read the oscilloscope guide to configure the oscilloscope to your needs.
SciSDK Oscilloscope (https://nuclearinstruments.github.io/SCISDK/md_docs_oscilloscope.html)

## Requirements

- Python 3.7 or higher
- scisdk library installed in the system (https://github.com/NuclearInstruments/SCISDK/releases)
- scisdk python wrapper installed in the system (https://pypi.org/project/scisdk)
```bash
    pip install scisdk
```

## Instructions

- Upload on the board the oscilloscope example firmware/<board>/Oscilloscope
- Select the proper connection string for the board in the script editing the code
- Run the example
- You will get 5 files: analog.txt will contain the analog channels data, digital_1/2/3/4.txt will contain the digital channels data, 
 