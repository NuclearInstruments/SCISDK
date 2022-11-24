# Oscilloscope Dual driver

The oscilloscope allows to acquire waveform and download them to the user PC.
The oscilloscope dual packetize the data in the FPGA buffer to double the memory depth of the device.
The driver masks the Oscilloscope Dual packetization and provides decoded data exactly as the Oscilloscope driver.
Only raw data are different.

The number of channels is multiple of 2, whith a minimum of 2 channels.

Refere to the [Oscilloscope driver](oscilloscope.md) for more information.

## Data structure

The two data structure for decoded and raw data are:
- RAW: SCISDK_OSCILLOSCOPE_DUAL_RAW_BUFFER
- DECODED: SCISDK_OSCILLOSCOPE_DUAL_DECODED_BUFFER

# Raw data formware

| Raw Memory | 
| --------- |
| Channel 1 - Word 0| 
| ... |
| Channel 1 - Word 1023| 
| Channel 2 | 
| ... |
| Channel 3 | 
| ... |
| Channel 4 |

Each sample will be organized in the data buffer as follows:


|31|30|29 ... 16|15|14|13 .. 0|
|--|--|---------|--|--|-------|
|     *CH 1*  |||    *CH 2* |||
|d1|d0|   AN    |d1|d0|   AN  |

