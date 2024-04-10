# 2D Histogram

 The 2D histogram is a matrix organized in rows and columns and allows to count the number of events in a two-dimensional space. 
 The typical application is to accumulate in FPGA a multi-parameters analysis like a Pulse Shape vs Energy (PSD), time of flight vs energy, or rise time vs energy. 
 The histogram is a configurable size memory managed by the IP core in the fpga. The spectrum core increment the bin of the histogram specified by the x and y values of the event.

 The FPGA elements has the following inputs:
  - `x`: a 16 bit value representing the x position of the event in the 2D matrix
  - `y`: a 16 bit value representing the y position of the event in the 2D matrix
  - `dv`: a trigger that validate the event and increment the bin of the histogram

An energy spectrum has the following parameters taken from the JSON file:
 - the number of bins on X
 - the number of bins on Y
 - the number of bits on the z-axis

Number of bins and number of bits define the memory occupancy of the spectrum indeed they are defined at compile time by SciCompiler.


# Parameters
The following parameters can be configured:

| Parameter         | Acces Mode | Description                                                                              | Default value |
| ----------------- | ---------- | ---------------------------------------------------------------------------------------- | ------------- |
| limitmode         | R/W        | spectrum integration limit target: freerun, time_ms, total_count, peak_count             | freerun       |
| limit             | R/W        | target value when limit mode is set to: time_ms, total_count, peak_count                 | 0             |
| binsX             | R          | number of bins of the histogram X                                                        |               |
| binsY             | R          | number of bins of the histogram Y                                                        |               |
| max_conts         | R          | maximum number of counts y-axes on the histogram                                         |               |
| buffer_type       | R          | type of the buffer: (always decoded)                                                     |               |



### Limit mode
The limit mode is used to set the integration time of the spectrum. The limit mode can be set to:
 - freerun: the spectrum is integrated until the user stop it
 - time_ms: the spectrum is integrated for a time in milliseconds
 - total_count: the spectrum is integrated until the total number of counts is reached (integrating all the bins in the histogram)
 - peak_count: the spectrum is integrated until the peak count is reached (the y-value of the bin with the highest count)

Limit must be congirured accordingly to the limit mode. The limit mode is set by the parameter `limitmode`. The limit is set by the parameter `limit`.
`limit` parameter is a 32 bit in value value. The limit is expressed in milliseconds for `time_ms` mode, in counts for `total_count` and `peak_count` mode.
In order to use the limit, the two parameters `limitmode` and `limit` must be set before the spectrum is started. 

The correct procedure to set (for example) the time limit is:
- set `limitmode` to `time_ms`
- set `limit` to the desired value (ie 10000 for 10 seconds)
- start the spectrum with `start` command
- poll the status of the spectrum until it is stopped

During the running phase it is possible to:
- stop the spectrum with `stop` command (abort)
- restart the limit counter with `reset_counters` command




## Commands
The following commands are available:

| Command               | Description                                                                    | Parameter |
| --------------------- | ------------------------------------------------------------------------------ | --------- |
| start                 | start the spectrum integration                                                 |           |
| stop                  | suspend the spectrum integration                                               |           |
| reset                 | reinitialize all bins in the histogram to 0                                    |           |
| reset_counters        | reset all limit counters and statistics counters in order to restart limits    |           |


### Resets the spectrum
`start` and `stop` commands do not reset the spectrum. They should be consideder as a start and pause of the spectrum. A `start` command after a `stop` command will continue the integration from the last bin value.
In order to reset the spectrum it is necessary to use the `reset` command. The `reset` command reset all the bins in the histogram to 0. The `reset` command is useful to restart the spectrum from scratch.
`reset` command execute also the `reset_counters` command.

### Reset counters
This command is useful to reset all limit counters and statistics counters in order to restart limits. This command is executed by `reset` command implicitally.
When this command is executed the progress in the status is also reset to 0.


## Output data Format

Spectrum component support only decoded output data format.
### Decoded Data
The data output structure is the following:

```c
	typedef struct {
		uint32_t magic;
		uint32_t *data;
		uint64_t timecode;
		uint32_t inttime;
		struct {
			uint32_t buffer_size;
			uint32_t total_bins;
			uint32_t valid_bins;
            uint32_t binsX;
            uint32_t binsY;
		} info;
	}SCISDK_2DHISTOGRAM_DECODED_BUFFER;
```

The `magic` field is a 32 bit value that identify the data format.
The `data` field is a pointer to the data buffer. The data buffer is an array of 32 bit values. The number of elements in the array is defined by the `total_bins` field. The `valid_bins` field is the number of bins that contains data. The `valid_bins` field is always less or equal to `total_bins` field. 

`timecode` it's the epoch of the readout PC when the data is readout from the fpga. The timecode is expressed in milliseconds.

Data in the `data` array are the counts of each bin. The first element of the array is the bin in position X0 Y0. The last element of the array is the bin with the X_MAX Y_MAX.
Each element of the array is a 32 bit value.

For example for a 2d histogram with 128 bins on X and 64 bins on Y the data format is the following:
| Data Format |
| ----------- |
| X0, Y0      |
| X1, Y0      |
| ...         |
| X127, Y0    |
| X0, Y1      |
| X1, Y1      |
| ...         |
| X0, Y63     |
| ...         |
| X127, Y63   |

## Status
The status of the spectrum is stored in a SCISDK_SPECTRUM_STATUS structure.
    
```c
    typedef struct {
		bool running;					
		bool completed;					
		uint32_t progress;				
		uint32_t peak_max;				
		uint32_t total_counter;			
		double integration_time;		
    }SCISDK_SPECTRUM_STATUS;
```

The `running` field is true if the spectrum is running.
The `completed` field is true if the spectrum run is completed. This field has meaning only if the spectrum `limitmode` is:
 - `total_count`
 - `peak_count`
 - `time_ms`

 `completed` is false until the internal limit counter do no reach the limit value. 
 
The `progress` field is the progress of the spectrum integration in respect of the value of `limit`. The progress is expressed in percentage. 

The `peak_max` field is the value of the bin with the highest count. 

The `total_counter` field is the total number of counts in the histogram.

The `integration_time` field is the integration time of the spectrum in milliseconds.

The current version of the library do not report the `peak_max`, `total_counter`,  `integration_time` fields. These fields are always 0.


## Basic Examples
The following example shows how to use the spectrum component.

### C
```c

    SCISDK_2DHISTOGRAM_DECODED_BUFFER *hist2d;
    
    int res = SCISDK_AllocateBuffer("board0:/MMCComponents/Hist2D_0", T_BUFFER_TYPE_DECODED, (void**)&hist2d, _sdk);
    if (res != NI_OK) {
        printf("Error allocating buffer\n");
        return -1;
    }
    SCISDK_SetParameterString("board0:/MMCComponents/Hist2D_0.limitmode", "freerun", _sdk);
    SCISDK_ExecuteCommand("board0:/MMCComponents/Hist2D_0.reset", "", _sdk;
    SCISDK_ExecuteCommand("board0:/MMCComponents/Hist2D_0.start", "", _sdk);
    SCISDK_ReadData("board0:/MMCComponents/Hist2D_0", (void *)hist2d, _sdk);
    
    ....

    SCISDK_FreeBuffer("board0:/MMCComponents/Hist2D_0", 1, (void**)&hist2d, _sdk);

```
### Python
```python
    import matplotlib.pyplot as plt
    import time
    from scisdk.scisdk import SciSDK
    from scisdk.scisdk_defines import *

    res, buf2d = sdk.AllocateBuffer("board0:/MMCComponents/Hist2D_0", T_BUFFER_TYPE_DECODED)
    if res != 0:
        print("Error allocating buffer")
        return -1
    sdk.SetParameter("board0:/MMCComponents/Hist2D_0.limitmode", "freerun")
    sdk.ExecuteCommand("board0:/MMCComponents/Hist2D_0.reset", "")
    sdk.ExecuteCommand("board0:/MMCComponents/Hist2D_0.start", "")
    res, buf2d = sdk.ReadData("board0:/MMCComponents/Hist2D_0", buf2d)
    
    time.sleep(10)  # wait 10 seconds

    # convert the buffer to a 2D array
    S=[]
    if res2d == 0:
        S = [0 for i in range(buffer_hist.info.binsY)] 
        for i in range(buffer_hist.info.binsY):
            S[i] = [0 for j in range(buffer_hist.info.binsX)]
        for i in range(buffer_hist.info.binsY):
            for j in range(buffer_hist.info.binsX):
                S[i][j] = buffer_hist.data[i*buffer_hist.info.binsX+j]


    fig, ax = plt.subplots(1, 1)
    # plot heatmap
    cax = ax[0].imshow(S, cmap='jet', interpolation='nearest')
    # add colorbar
    fig.colorbar(cax, ax=ax[0])


    sdk.FreeBuffer("board0:/MMCComponents/Hist2D_0", 1, buf2d)

```
