# Wave Digitizer driver

The Wave Digitizer componet has the function to acquire multiple channels capturing waveform from each one. It is similar to the oscilloscope, but, while the oscilloscope purpose is to monitor the internal process of the firmware, and indeed is not very fast in transfering data, digitizer function is optimized to maximize the speed and the memory usage of the device.
In the Wave Digitizer exist a shared memory for all channels for which the Wave Digitizer has been created. At compile time, SciCompiler defines the total size of this memory and the maximum number of channels. At runtime the user can configure if to enable one, two, four channels. The memory will be partitioned in function of the enable channels.

It is designed as following: a packet creator block serializes the enabled channels in a common FIFO and transfer data to the PC.

Every time a start signal is triggered, the timestamp, the hits and user data are captured, and enqueued in the output FIFO.

It is only possible the specify the number of channels (N) starting from the channel 0. So If N = 2, CH0 and CH1 will be dumped, if N = 4, CH0, CH1, CH2, CH3 will be dumped.

Example with four channels:

- All four channels enabled
![](img/dgz01.png)  

- Only CH0 and CH1 enabled
![](img/dgz02.png)  

It is not possible to configre a pre-trigger on the Wave Digitizer. The trigger is always on the first sample. To implement a pre-trigger, it necessary to connect, on each input channel, a delay block with the desired delay (SciCompiler provide fixed and programmable delay block).





The readout can be configure to work in two different mode:
- blocking : the `SCISDK_ReadData` function will block until the buffer is full or the timeout is reached.
- non-blocking : the `SCISDK_ReadData` function will return immediately with the number of bytes read.


**Raw Mode**
In raw mode the digitizer do not process packet and provide to the user application the raw data. The user application must be able to process the data and extract the information. Raw mode can be useful in order to save at the maximum speed the data to disk. The user application can be a simple script that save the data in a file. A second application can be used to process the data and extract the waveforms.


## Parameters
The following parameters can be configured:

| Parameter             | Acces Mode | Description                                                                              | Default value |
| --------------------- | ---------- | ---------------------------------------------------------------------------------------- | ------------- |
| acq_len               | R/W        | maximum number of sample to be transferred per physical FIFO access                      | 1024          |
| acq_mode              | R/W        | select blocking/non-blocking mode                                                        | blocking      |
| timeout               | R/W        | set the timout in ms in blocking mode                                                    | 100           |
| high_performance      | R/W        | if true the internal FIFO access lock the bus in priority mode.                          | false         |
| check_align_word      | R/W        | if true, check the packet alignment                                                      | false         |
| data_processing       | R/W        | set data processing mode: raw, decode                                                    | decode        |
| enabledch             | R/W        | number of enabled channels.                                                              | --all ch--    |
| buffer_type           | R          | get buffer type: SCISDK_OSCILLOSCOPE_RAW_BUFFER or SCISDK_OSCILLOSCOPE_DECODED_BUFFER    |               |
| channel_count         | R          | get buffer the number of channel created with SciCompiler                                |               |
| fifo_size             | R          | get buffer the total number of samples in the output fifo created with SciCompiler       |               |
| max_ch_samples        | R          | return the maximum number of samples per channel can be read                             |               |

### Acquisition length
The `acq_len` parameter set the number of sample per channels to be read. 
The maximum value depends by tge number of channels enabled. The maximum value is the total size of the shared memory divided by the number of channels enabled.
The driver suggest the maximum value for this parameter reading the `max_ch_samples` parameter.

### Blocking/Non blocking mode

In blocking mode the `SCISDK_ReadData` function will block until the buffer is full or the timeout is reached. In non-blocking mode the `SCISDK_ReadData` function will return immediately with the number of bytes read.
The maximum number of bytes that can be read is the size of the buffer allocated with the function `SCISDK_AllocateBuffer`.
In blocking mode the timeout is set with the `timeout` parameter. The timeout is expressed in ms. In non-blocking mode the timeout is not used.
In both mode, if the buffer is fill with even just a single word the function will return NI_OK. 
If there is no data at all the read function will return NI_NO_DATA_AVAILABLE.

### Enable channels
The `enabledch` parameter is the number of enabled channels. It is only possible the specify the number of channels (N) starting from the channel 0. So If N = 2, CH0 and CH1 will be dumped, if N = 4, CH0, CH1, CH2, CH3 will be dumped.
Valid number are from 1 to the maximum number of channels created in SciCompiler.

## Data processing mode
The `data_processing` parameter can be used to set the data processing mode. The data processing mode can be set to raw or decode.
In raw mode the data are not processed and the user will receive the raw data from the FPGA. In decode mode the data are processed and the user will receive the data as they are pushed in the FPGA fifo, loosing the concept of packet. The custom packet behavior is the same of the list
In decode mode the data are analized, divided in packet and it is possible to check the alignment to an aligment word.