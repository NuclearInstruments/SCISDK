# Rate Meter

The rate meter is a simple component that allow to measure the rate of incoming signal
The integration time is configured by SciCompiler at Compile Time.
If the inegration time (configured in clock cycles) is multiple of clock period, the measure can be already considered in Hz, KHz, MHz ...
For example if clock frequency is 125MHz, and integration time is 125e6, the measure is in Hz, if integration time is 125e3, the measure is in KHz, if integration time is 125, the measure is in MHz.

This component support up to 256 channels.
The integration time is common to all channels.
The number in tha data field is the number of transition L->H of the i-th input signal in the integration time.


# Parameters
The following parameters can be configured:

| Parameter         | Acces Mode | Description                                                                              | Default value |
| ----------------- | ---------- | ---------------------------------------------------------------------------------------- | ------------- |
| buffer_type       | R          | type of the buffer: (SCISDK_RM_RAW_BUFFER)                                               |               |




## Commands
The are no commands for this component



## Output data Format

The ToF spectrum uses the same data structures of the energy spectrum
### Decoded Data
The data output structure is the following:

```c
	typedef struct {
		uint32_t magic;					
		double *data;					
		struct {
			uint32_t buffer_size;		
			uint32_t valid_data;		
			uint32_t nchannels;			
		}info;
	}SCISDK_RM_RAW_BUFFER;

```

The `magic` field is a 32 bit value that identify the data format.
The `data` field is a pointer to the data buffer. The data buffer is an array of 32 bit values. The number of elements in the array is defined by the `total_bins` field. The `valid_bins` field is the number of bins that contains data. The `valid_bins` field is always equal to `total_bins` field. 

`timecode` it's the epoch of the readout PC when the data is readout from the fpga. The timecode is expressed in milliseconds.

Data in the `data` array are the counts of each bin. The first element of the array is the bin with the lowest time. The last element of the array is the bin with the highest time in the spectrum.
Each element of the array is a 32 bit value. 
| Data Format |
| ----------- |
| bin 0       |
| bin 1       |
| bin 2       |
| ...         |
| bin n       |


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
The `completed` field is  always false.
 
The `progress` field is unused.

The `peak_max` field is the value of the bin with the highest count. 

The `total_counter` field is the total number of counts in the histogram.

The `integration_time` field is the integration time of the spectrum in milliseconds.

The current version of the library do not report the `peak_max`, `total_counter`,  `integration_time` fields. These fields are always 0.


## Basic Examples
The following example shows how to use the spectrum component.

### C
```c

    SCISDK_SPECTRUM_DECODED_BUFFER *obSpectrum;
    
    int res = SCISDK_AllocateBuffer("board0:/MMCComponents/TOF_0", T_BUFFER_TYPE_DECODED, (void**)&obSpectrum, _sdk);
    if (res != NI_OK) {
        printf("Error allocating buffer\n");
        return -1;
    }
    SCISDK_SetParameterString("board0:/MMCComponents/TOF_0.binwidth", "100",_sdk);
    SCISDK_SetParameterString("board0:/MMCComponents/TOF_0.start_delay", "0",_sdk);
    SCISDK_ExecuteCommand("board0:/MMCComponents/TOF_0.reset", "", _sdk;
    SCISDK_ExecuteCommand("board0:/MMCComponents/TOF_0.start", "", _sdk);
    SCISDK_ReadData("board0:/MMCComponents/TOF_0", (void *)obSpectrum, _sdk);
    
    ....

    SCISDK_FreeBuffer("board0:/MMCComponents/TOF_0", 1, (void**)&obSpectrum, _sdk);

```

### C++
```c++
    SCISDK_SPECTRUM_DECODED_BUFFER *obSpectrum;
    
    int res = sdk.AllocateBuffer("board0:/MMCComponents/TOF_0", T_BUFFER_TYPE_DECODED, (void**)&obSpectrum, _sdk);
    if (res != NI_OK) {
        cout << "Error allocating buffer" << endl;
        return -1;
    }
    sdk.SetParameter("board0:/MMCComponents/TOF_0.binwidth", "100",_sdk);
    sdk.SetParameter("board0:/MMCComponents/TOF_0.start_delay", "0", _sdk);
    sdk.ExecuteCommand("board0:/MMCComponents/TOF_0.reset", "", _sdk;
    sdk.ExecuteCommand("board0:/MMCComponents/TOF_0.start", "", _sdk);
    sdk.ReadData("board0:/MMCComponents/TOF_0", (void *)obSpectrum, _sdk);
    
    ....

    sdk.FreeBuffer("board0:/MMCComponents/TOF_0", 1, (void**)&obSpectrum, _sdk);

```

### Python
```python
    res, obSpectrum = sdk.AllocateBuffer("board0:/MMCComponents/TOF_0", T_BUFFER_TYPE_DECODED)
    if res != 0:
        print("Error allocating buffer")
        return -1
    sdk.SetParameter("board0:/MMCComponents/TOF_0.binwidth", "100")
    sdk.SetParameter("board0:/MMCComponents/TOF_0.start_delay", "0")
    sdk.ExecuteCommand("board0:/MMCComponents/TOF_0.reset", "")
    sdk.ExecuteCommand("board0:/MMCComponents/TOF_0.start", "")
    res, obSpectrum = sdk.ReadData("board0:/MMCComponents/TOF_0", obSpectrum)
    
    ....

    sdk.FreeBuffer("board0:/MMCComponents/TOF_0", 1, obSpectrum)

```

### C Sharp
```csharp
    SCISDK_SPECTRUM_DECODED_BUFFER obSpectrum;
    
    int res = sdk.AllocateBuffer("board0:/MMCComponents/TOF_0", T_BUFFER_TYPE_DECODED, ref obSpectrum);
    if (res != 0) {
        Console.WriteLine("Error allocating buffer");
        return -1;
    }
    sdk.SetParameter("board0:/MMCComponents/TOF_0.binwidth", "100");
    sdk.SetParameter("board0:/MMCComponents/TOF_0.start_delay", "0");
    sdk.ExecuteCommand("board0:/MMCComponents/TOF_0.reset", "");
    sdk.ExecuteCommand("board0:/MMCComponents/TOF_0.start", "");
    dk.ReadData("board0:/MMCComponents/TOF_0", ref obSpectrum);
    
    ....

    sdk.FreeBuffer("board0:/MMCComponents/TOF_0", 1, ref obSpectrum);

```

### VB.NET
```vb
    Dim obSpectrum As SCISDK_SPECTRUM_DECODED_BUFFER
    
    Dim res As Integer = sdk.AllocateBuffer("board0:/MMCComponents/TOF_0", T_BUFFER_TYPE_DECODED, obSpectrum)
    If res <> 0 Then
        Console.WriteLine("Error allocating buffer")
        Return -1
    End If
    sdk.SetParameter("board0:/MMCComponents/TOF_0.binwidth", "100")
    sdk.SetParameter("board0:/MMCComponents/TOF_0.start_delay", "0")
    sdk.ExecuteCommand("board0:/MMCComponents/TOF_0.reset", "")
    sdk.ExecuteCommand("board0:/MMCComponents/TOF_0.start", "")
    sdk.ReadData("board0:/MMCComponents/TOF_0", obSpectrum)
    
    ....

    sdk.FreeBuffer("board0:/MMCComponents/TOF_0", 1, obSpectrum)

```


## Additional Examples

### Print spectrum
This example print a spectrum every second.
```c

    SCISDK_SPECTRUM_DECODED_BUFFER *obSpectrum;
    
    int res = SCISDK_AllocateBuffer("board0:/MMCComponents/TOF_0", T_BUFFER_TYPE_DECODED, (void**)&obSpectrum, _sdk);
    if (res != NI_OK) {
        printf("Error allocating buffer\n");
        return -1;
    }
    SCISDK_SetParameterString("board0:/MMCComponents/TOF_0.binwidth", "100",_sdk);
    SCISDK_SetParameterString("board0:/MMCComponents/TOF_0.start_delay", "0", _sdk);
    SCISDK_ExecuteCommand("board0:/MMCComponents/TOF_0.reset", "", _sdk;
    SCISDK_ExecuteCommand("board0:/MMCComponents/TOF_0.start", "", _sdk);

    while (1) {
        //Just leave in your code of of two of this sleep functions
        usleep(1000*1000);  // LINUX: sleep for 1s
        Sleep(1000);        // WINDOWS: sleep for 1s 
        int res = SCISDK_ReadData("board0:/MMCComponents/TOF_0", (void *)obSpectrum, _sdk);
        if (res == NI_OK) {
            for (int i = 0; i < obSpectrum->info.valid_bins; i++) {
                printf("[%5d] -- %9d\n", i, obSpectrum->data[i]);
            }
        }
    }
    SCISDK_FreeBuffer("board0:/MMCComponents/TOF_0", 1, (void**)&obSpectrum, _sdk);

