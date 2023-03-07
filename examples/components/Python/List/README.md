# List Example
This is a simple example of how to use the list. 
This example download available data from list_0 endpoint and use the struct.unpack to decode the data.
Data are print in console
The example firmware produce a word with:
- 16 bit of peak detector in window of 625000 clock cycles for CH 0
- 16 bit of peak detector in window of 625000 clock cycles for CH 1
- 32 bit event counter


## Requirements

- Python 3.7 or higher
- scisdk library installed in the system (https://github.com/NuclearInstruments/SCISDK/releases)
- scisdk python wrapper installed in the system (https://pypi.org/project/scisdk)
```bash
    pip install scisdk
```

## Instructions

- Upload on the board the list example firmware/<board>/list
- Select the proper connection string for the board in the script editing the code
- Run the example
- check dump data in console
 