# Oscilloscope driver

The oscilloscope allows to acquire waveform and download them to the user PC.

The oscilloscope can be configured to acquire one or multiple channels. 
The acquisition can be triggered by an external signal, by a software trigger, by self trigger or by leading edge trigger.

## Parameters
The following parameters can be configured:

| Parameter         | Acces Mode | Description                                                                              | Default value |
| ----------------- | ---------- | ---------------------------------------------------------------------------------------- | ------------- |
| decimator         | R/W        | set x-axis decimation factor                                                             | 0             |
| pretrigger        | R/W        | set pretrigger memory length (in samples)                                                | 0             |
| trigger_level     | R/W        | set trigger level on selected analog channel                                             | 0             |
| trigger_channel   | R/W        | set trigger channel when trigger mode is set to "analog"                                 | 0             |
| auto_arm          | R/W        | set 1 to enable the auto arm feature                                                     | 1             |
| trigger_mode      | R/W        | set trigger mode: disabled, self, analog, ext, digital                                   | self          |
| data_processing   | R/W        | set data processing mode: raw, decode                                                    | decode        |
| acq_mode          | R/W        | set data processing mode: blocking, non-blocking                                         | blocking      |
| trigger_polarity  | R/W        | set trigger polarity: pos, neg                                                           | pos           |
| timeout           | R/W        | set timeout for blocking acquisition in ms                                               | 5000          |
| nanalog           | R          | get number of analog tracks per channel                                                  |               |
| ndigital          | R          | get number of digital tracks per channel                                                 |               |
| nchannels         | R          | get number of channels                                                                   |               |
| buffer_type       | R          | get buffer type: SCISDK_OSCILLOSCOPE_RAW_BUFFER or SCISDK_OSCILLOSCOPE_DECODED_BUFFER    |               |

## Decimation
The input signal is sampled at a rate ADC sample rate (for example on DT5560 is 125 Msps). The decimation factor can be set to reduce the sampling rate.
0 means no decimation, 1 means 1/2 decimation (i.e. 62.5 Msps), 2 means 1/4 decimation (i.e 31.25 Msps), etc.
Decimation can be used to extend the acquisition time. For example, if the memory size is 1 Ksamples and the decimation factor is 0, the acquisition time is 1 Ksamples/125 Msps = 8 us. If the decimation factor is 1, the acquisition time is 1 Ksamples/62.5 Msps = 16 us.

## Trigger
Following trigger modes are available:
- <b> self trigger </b> acquires data in free running mode. The acquisition is started by the user with the ReadData function

- <b> analog </b> trigger the acquisition when the selected analog channel crosses the trigger level. The trigger level can be set with the trigger_level parameter.
The polarity of the trigger can be set with the trigger_polarity parameter.

- <b> digital </b> trigger the acquisition when the selected digital channel transitions from 0 to 1 or from 1 to 0. The polarity of the trigger can be set with the trigger_polarity parameter. The channel is selected with the trigger_channel parameter.

- <b> ext </b> trigger the acquisition when the external trigger is received. The external trigger is applied to the START input on the SciCompiler block

## Acquisition Modes
Following acquisition modes are available:
- <b> blocking </b> mode waits for the acquisition to complete before returning from the ReadData function. The <b>timeout</b> can be set with the timeout parameter. 

- <b> non-blocking </b> mode returns immediately from the ReadData function. If trigger does not occur within the timeout, the function returns NI_NO_DATA_AVAILABLE 

