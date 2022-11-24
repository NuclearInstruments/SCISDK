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

### Decimation
The input signal is sampled at a rate ADC sample rate (for example on DT5560 is 125 Msps). The decimation factor can be set to reduce the sampling rate.
0 means no decimation, 1 means 1/2 decimation (i.e. 62.5 Msps), 2 means 1/4 decimation (i.e 31.25 Msps), etc.
Decimation can be used to extend the acquisition time. For example, if the memory size is 1 Ksamples and the decimation factor is 0, the acquisition time is 1 Ksamples/125 Msps = 8 us. If the decimation factor is 1, the acquisition time is 1 Ksamples/62.5 Msps = 16 us.

### Trigger
Following trigger modes are available:
- <b> self trigger </b> acquires data in free running mode. The acquisition is started by the user with the ReadData function

- <b> analog </b> trigger the acquisition when the selected analog channel crosses the trigger level. The trigger level can be set with the trigger_level parameter.
The polarity of the trigger can be set with the trigger_polarity parameter.
Trigger level operare as a leading edge trigger. The trigger is activated when the analog signal crosses the trigger level in the selected polarity.


- <b> digital </b> trigger the acquisition when the selected digital channel transitions from 0 to 1 or from 1 to 0. The polarity of the trigger can be set with the trigger_polarity parameter. The channel is selected with the trigger_channel parameter.

- <b> ext </b> trigger the acquisition when the external trigger is received. The external trigger is applied to the START input on the SciCompiler block

### Acquisition Modes
Following acquisition modes are available:
- <b> blocking </b> mode waits for the acquisition to complete before returning from the ReadData function. The <b>timeout</b> can be set with the timeout parameter. 

- <b> non-blocking </b> mode returns immediately from the ReadData function. If trigger does not occur within the timeout, the function returns NI_TIMEOUT 

### Polarity
The polarity of the trigger can be set with the trigger_polarity parameter. Following values are available:
- <b> pos </b> positive polarity
- <b> neg </b> negative polarity


### Auto Arm
The auto arm feature can be enabled with the auto_arm parameter. When enabled, the acquisition is automatically armed by the ReadData function.

### Data Processing
The data processing mode can be set with the data_processing parameter. Following values are available:
- <b> raw </b> returns the raw data from the oscilloscope. The oscilloscope buffer is not decoded indeed the data is returned as it is stored in the oscilloscope memory. 
- <b> decode </b> returns decoded data from the oscilloscope. The oscilloscpe buffer is decoded internally to the DLL
  
## Commands
The following commands are available:

| Command               | Description                                                                    | Parameter |
| --------------------- | ------------------------------------------------------------------------------ | --------- |
| arm                   | if auto-arm is not enabled, arm the trigger of the oscillocope                 |           |
| reset_read_valid_flag | reset the data ready flag                                                      |           |


## Output data Format
### Raw Data
The data output structure is the following:
```c
	typedef struct {
		uint32_t magic;
		uint32_t *data;
		uint32_t zero_position;
		uint32_t trigger_position;
		uint64_t timecode;
		struct  {
			uint32_t buffer_size;
			uint32_t samples_analog;
			uint32_t samples_digital;
			uint32_t tracks_analog_per_channel;
			uint32_t tracks_digital_per_channel;
			uint32_t channels;
		} info;
	}SCISDK_OSCILLOSCOPE_RAW_BUFFER;
```
`magic` : magic number to identify the buffer type. It is set to SCISDK_OSCILLOSCOPE_RAW_BUFFER_MAGIC
`data` : it's the array of the raw data. The data is stored in the oscilloscope memory as it is. 
The first element of the array is the first sample acquired by the oscilloscope on channel 0; the second element is the second sample acquired by the oscilloscope on channel 0. After the last sample acquired by the oscilloscope on channel 0,  the first sample acquired by the oscilloscope on channel 1 is stored in the data buffer.


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

|31 .. 20|19|18|17|16|15 .. 0|
|--------|--|--|--|--|--------|
|xxxxxxxx|d3|d2|d1|d0| analog |

`zero_position` :  the position of the trigger in the data buffer. The trigger position is the position of the first sample acquired by the oscilloscope after the trigger event. The trigger position is expressed in samples.

`trigger_position` : will be set equal to the pre_trigger parameter. The trigger position is expressed in samples. It is relative to the zero position.

`timecode` : the timecode of the trigger event. The timecode is expressed in time code units and depends by what is connected to the timecode input of the oscilloscope.

`info` : it contains information about the oscilloscope buffer. The fields are:
- `buffer_size` : the size of the buffer in bytes
- `samples_analog` : the number of analog samples per channel
- `samples_digital` : the number of digital samples per channel
- `tracks_analog_per_channel` : the number of analog tracks per channel
- `tracks_digital_per_channel` : the number of digital tracks per channel
- `channels` : the number of channels



### Decoded 
Data is returned in the user buffer in the following format:

```c
	typedef struct {
		uint32_t magic;
		int32_t *analog;
		uint8_t *digital;
		uint32_t trigger_position;
		uint64_t timecode;
		struct  {
			uint32_t samples_analog;
			uint32_t samples_digital;
			uint32_t tracks_analog_per_channel;
			uint32_t tracks_digital_per_channel;
			uint32_t channels;
		} info;
	}SCISDK_OSCILLOSCOPE_DECODED_BUFFER;
```

`magic` : magic number to identify the buffer type. It is set to SCISDK_OSCILLOSCOPE_DECODE_BUFFER_MAGIC
`analog` : it's the array of the analog data. 

| Analog Channels | 
| --------- |
| Channel 1 - Analog Word 0| 
| ... |
| Channel 1 - Analog Word 1023| 
| Channel 2 | 
| ... |
| Channel 3 | 
| ... |
| Channel 4 |

The format of the 32 bit analog word is the following (with oscilloscope in 16 bit mode):
|31 .. 16|15 .. 0|
|--------|--------|
|xxxxxxxx| analog |


`digital` : it's the array of the analog data. 

| Digital Channels | 
| --------- |
| Channel 1 - Digital Word 0| 
| ... |
| Channel 1 - Digital Word 1023| 
| Channel 2 | 
| ... |
| Channel 3 | 
| ... |
| Channel 4 |

The format of the 8 bit digital word is the following:
|7 .. 4| 3 | 2 | 1 | 0 |
|------|---|---|---|---|
|xxxxxx| d3| d2| d1| d0|

`position` : the position of the trigger in the data buffer. The trigger position is the position of the first sample acquired by the oscilloscope after the trigger event. The trigger position is expressed in samples. The samples before the trigger position are the pre-trigger samples.

`timecode` : the timecode of the trigger event. The timecode is expressed in time code units and depends by what is connected to the timecode input of the oscilloscope in SciCompiler.

`info` : it contains information about the oscilloscope buffer. The fields are:
- `samples_analog` : the number of analog samples per channel
- `samples_digital` : the number of digital samples per channel
- `tracks_analog_per_channel` : the number of analog tracks per channel
- `tracks_digital_per_channel` : the number of digital tracks per channel
- `channels` : the number of channels



## Basic Examples
### C
```c
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self", _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150, _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0, _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode", _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking", _sdk);

SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
SCISDK_AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob, _sdk);

SCISDK_ReadData("board0:/MMCComponents/Oscilloscope_0", (void *)ob, _sdk);
```

### C++
```c++
sdk->setParameter("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self");
sdk->setParameter("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150);
sdk->setParameter("board0:/MMCComponents/Oscilloscope_0.decimator", 0);
sdk->setParameter("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode");
sdk->setParameter("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking");

SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
sdk->allocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob);
sdk->readData("board0:/MMCComponents/Oscilloscope_0", (void *)ob);
```

### Python
```python
sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing","decode")
sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode","self")
sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150)
sdk.SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0)
sdk.SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking")

res, buf = sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0")
res, buf = sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", buf)
if (res == 0) {
	# do something with the buffer
}
```

### C Sharp

```csharp
sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing","decode");
sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking");
sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.timeout", 1000);
sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.decimator", 0);
sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_mode","self");
sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150);

// allocate buffer
SciSDKOscilloscopeDecodedBuffer buffer;
buffer = new SciSDKOscilloscopeDecodedBuffer();
sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", BufferType.BUFFER_TYPE_DECODED, ref buffer);
if (sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", ref buffer) == 0)
{
	// do something with the buffer
}
```

### VB.NET


```vb
sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.data_processing","decode")
sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking")
sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.timeout", 1000)
sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.decimator", 0)
sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.trigger_mode","self")
sdk.SetParameter("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150)

'allocate buffer
dim buffer as new SciSDKOscilloscopeDecodedBuffer()
sdk.AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", BufferType.BUFFER_TYPE_DECODED, buffer)
if (sdk.ReadData("board0:/MMCComponents/Oscilloscope_0", buffer) == 0)
{
	'do something with the buffer
}
```


## Additional Examples
### Trigger on external signal
```c
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "ext", _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150, _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0, _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode", _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking", _sdk);

SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
SCISDK_AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob, _sdk);

SCISDK_ReadData("board0:/MMCComponents/Oscilloscope_0", (void *)ob, _sdk);
```

### Trigger on analog signal
```c
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "analog", _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_channel", 0, _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_level", 3000, _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150, _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0, _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode", _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking", _sdk);

SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
SCISDK_AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob, _sdk);

SCISDK_ReadData("board0:/MMCComponents/Oscilloscope_0", (void *)ob, _sdk);
```

### Non blocking mode
```c
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "ext", _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150, _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0, _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode", _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "non-blocking", _sdk);

SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
SCISDK_AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob, _sdk);

// poll ReadData until it returns 0 for a maximum of 10000ms in non-blocking mode
int res = NI_TIMEOUT;
int timeout = 10000;
while ((res == NI_TIMEOUT) && (timeout > 0))
{
	res = SCISDK_ReadData("board0:/MMCComponents/Oscilloscope_0", (void *)ob, _sdk);
	timeout -= 10;
	
	/*
	 * do something else
	 */

	if (res == NI_OK) {
		
	} else {
		Sleep(10);
	}
} 
```

### Processing decoded data and store in a file, one channel per column

The following example acquire a waveform from an oscilloscope with multiple channels and store the data in a file. The data is stored in a file with one column per channel. The first column is the time (in sample), the other columns are the data from the channels.



```c
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self", _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150, _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0, _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode", _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking", _sdk);

SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
SCISDK_AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob, _sdk);

res = SCISDK_ReadData("board0:/MMCComponents/Oscilloscope_0", (void *)ob, _sdk);

if (res == NI_OK) {
	FILE *fp = fopen("data.txt", "w");
	if (fp != NULL) {
		//header of the file
		fprintf(fp, "    time");
		for (int i = 0; i < ob->n_channels; i++) {
			fprintf(fp, "    CH%02d", i);
		}
		//Data, fixed column width of 8
		for (int i = 0; i < ob->info.samples_analog; i++) {
			fprintf(fp, "%8d\t", i);
			for (int j = 0; j < ob->info.channels; j++) {
				fprintf(fp, "%8d\t", ob->analog[j][i]);
			}
			fprintf(fp, "\n");
		}
		fclose(fp);
	}
}


```

### Processing decoded data and store in a file, one channel per column
Is not required to release and reallocate the buffer, it is possible to reuse the same buffer for multiple acquisitions.

```c
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.trigger_mode", "self", _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.pretrigger", 150, _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/Oscilloscope_0.decimator", 0, _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.data_processing", "decode", _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/Oscilloscope_0.acq_mode", "blocking", _sdk);

SCISDK_OSCILLOSCOPE_DECODED_BUFFER *ob;
SCISDK_AllocateBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob, _sdk);

int res = NI_OK;
int count = 1000;
while (count-- > 0) {
	res = SCISDK_ReadData("board0:/MMCComponents/Oscilloscope_0", (void *)ob, _sdk);
	if (res == NI_OK) {
		//do something with the data
	}
}

SCISDK_FreeBuffer("board0:/MMCComponents/Oscilloscope_0", T_BUFFER_TYPE_DECODED, (void**)&ob, _sdk);

```

