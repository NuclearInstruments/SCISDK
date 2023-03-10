# List driver

The list allows to transfer data from FPGA to the PC in list mode.

In respect to Custom Packet or Frame, the list trasfer data in a raw mode: on the FPGA side it is possible to specify the number of bits of the inputs from 32 to 1024. The number of bit is defined in SciCompier and can not be changed at runtime.

On PC side the list provide a stream of bytes. Every time the list is readout every byte in the fifo is transferred to the PC.

The list is a FIFO buffer. The buffer is allocated in the FPGA and the data are transferred to the PC. On some board it is possible to enable DMA to speed up transfer. 
On the PC side a buffer can be allocated using the `SCISDK_AllocateBuffer` function. The buffer is freed using the `SCISDK_FreeBuffer` function.

Because the List is a FIFO is not possible to know a priori the number of bytes that will be transferred. The  `SCISDK_AllocateBuffer` take indeed an extra parameter to specify the size of the buffer (in FPGA word) on the PC side; the `SCISDK_ReadData` function will try to read the maximum number of bytes that can fit in the buffer. 

*It is important to note that the buffer size is specified in FPGA word and not in bytes. The FPGA word size is specified in SciCompiler and it is the size of the data input signal. This is always a multiple of 32 bits (DWORD). All transfer will always be done with a size multiple of the FPGA word size (input signal size). This ensure that is not possible to read partial world that will corrupt data in an urecoverable way*

The readout can be configure to work in two different mode:
- blocking : the `SCISDK_ReadData` function will block until the buffer is full or the timeout is reached.
- non-blocking : the `SCISDK_ReadData` function will return immediately with the number of bytes read.

More over the list can be configured to work:
- polling : the `SCISDK_ReadData` function will read the data from the hardware FIFO when the user call the function.
- threaded : on start command, the list will start a thread that will read the data from the hardware FIFO and store it in an internal buffer (thread buffer). The `SCISDK_ReadData` function will read the data from the thread buffer.
Thread mode increase the perfomance of the list because, while the use elaborate the previous data, the list accumulate next data in the thread buffer.

## Parameters
The following parameters can be configured:

| Parameter             | Acces Mode | Description                                                                              | Default value |
| --------------------- | ---------- | ---------------------------------------------------------------------------------------- | ------------- |
| acq_len               | R/W        | maximum number of sample to be transferred per physical FIFO access                      | 1024          |
| acq_mode              | R/W        | select blocking/non-blocking mode                                                        | blocking      |
| timeout               | R/W        | set the timout in ms in blocking mode                                                    | 100           |
| thread                | R/W        | enable the internal thread: true/false                                                   | false         |
| high_performance      | R/W        | if true the internal FIFO access lock the bus in priority mode.                          | false         |
| threaded_buffer_size  | R/W        | size in dword of the internal buffer                                                     | 100000        |
| buffer_type           | R          | get buffer type: SCISDK_OSCILLOSCOPE_RAW_BUFFER or SCISDK_OSCILLOSCOPE_DECODED_BUFFER    |               |

### Acquisition length
The `acq_len` parameter is the maximum number of sample to be transferred per physical FIFO access. This parameter has an impact on the transfer speed. As bigger is the value, as faster is the transfer. Bigger value will lock the bus for a longer time and can cause a loss of data if the readout channel is shared between multiple readout block (for example multiple list or custom packet).
The raccomanded value is the 90% size of the size of the internal FIFO.
This parameter is expressed in double word (32 bit).


### Blocking/Non blocking mode

In blocking mode the `SCISDK_ReadData` function will block until the buffer is full or the timeout is reached. In non-blocking mode the `SCISDK_ReadData` function will return immediately with the number of bytes read.
The maximum number of bytes that can be read is the size of the buffer allocated with the function `SCISDK_AllocateBuffer`.
In blocking mode the timeout is set with the `timeout` parameter. The timeout is expressed in ms. In non-blocking mode the timeout is not used.
In both mode, if the buffer is fill with even just a single word the function will return NI_OK. 
If there is no data at all the read function will return NI_NO_DATA_AVAILABLE.

### Threaded mode

In threaded mode the list will start a thread that will read the data from the hardware FIFO and store it in an internal buffer (thread buffer). The `SCISDK_ReadData` function will read the data from the thread buffer.
While in polling mode, if the user do not call the `SCISDK_ReadData` function, the FIFO in the FPGA can be full and the data will be lost. In threaded mode the data are stored in the thread buffer automatically. The internall thread will poll the FPGA to check if data is available and will store it in the thread buffer.
Thread mode increase the perfomance of the list because, but consume continuously the communication bandiwdth between the FPGA and the PC. The user lost the control on how much time the specific list will access to the shared bus.

### High performance mode
We discurage to enable this parameter. If enabled, the highe performace mode force the internal physical ReadFIFO function to ignore the word available field of the FIFO. When this parameter is disable, the ReadFIFO function check the word available field of the FIFO and will make a read only if this number if not 0. More over the read size will be the minimum between word available and acq_len parameter.
In this way the ReadFIFO function will not lock the bus for a long time. In high perforamnce mode, the ReadFIFO will always setup a read cycle with size equal to acq_len parameter. This will lock the bus for a long time untill all data are received.
This will increase significantly the transfer speed but no other access, even to read/write register can be done until all data are received or the timeout is reached.
Use this mode only if you are sure that the bus is not shared with other readout block and you don't need 

## Commands
The following commands are available:

| Command               | Description                                                                    | Parameter |
| --------------------- | ------------------------------------------------------------------------------ | --------- |
| start                 | Clear the FIFO and start the acqusition                                        |           |
| stop                  | Stop the acquisition                                                           |           |

Start will clear the list content, create the internal thread and allocate the thread buffer if thread mode is enable and start the acquisition.
All list parameter can not be changed while the acquisition is in progress. Stop the list before change any parameter or the property set will be rejected.

## Buffer Allocation
The buffer is allocated using the `SCISDK_AllocateBuffer` function. The buffer is freed using the `SCISDK_FreeBuffer` function.
Because the List is a FIFO is not possible to know a priori the number of bytes that will be transferred. The  `SCISDK_AllocateBuffer` take indeed an extra parameter to specify the size of the buffer (in word) on the PC side; the `SCISDK_ReadData` function will try to read the maximum number of bytes that can fit in the buffer.

The list support just RAW data. The data struct cointains a char* filed (array of char). The user must cast or convert it to correctly rappresents the data from the FPGA

This is an example on how allocate a List Buffer:
```c
SCISDK_AllocateBuffer(<path to list>, T_BUFFER_TYPE_RAW, (void**)&list_data, <number of word>)
```

The buffer allocated with the `SCISDK_AllocateBuffer` function has a last size parameters containing the number of word the user want to allocate in the buffer.
For example if the user create in SciCompiler a list with 64 bit words and want to allocate a buffer with 1000 words, the function will allocate a buffer with 8000 bytes. The  `SCISDK_ReadData` function will try to fill the buffer with 8000 bytes (1000 words * 8 bytes) each time is called.+
It is crtical to ensure that the transfer will always be aligned to the word size to avoid to read partial words with irrecoverable data loss.

This example allocate a buffer with 1000 words:

```c
SCISDK_LIST_RAW_BUFFER list_data;
SCISDK_AllocateBuffer("board0:/MMCComponents/List_0", T_BUFFER_TYPE_RAW, (void**)&list_data, 1000)
```

## Output data Format
### Raw Data
The data output structure is the following:
```c
	typedef struct {
		uint32_t magic;				
		char *data;						
		struct {
			uint32_t buffer_size;		
			uint32_t samples;			
			uint32_t valid_samples;		
			uint32_t channels;			
		} info;
	}SCISDK_LIST_RAW_BUFFER;
```
`magic` : magic number to identify the buffer type. It is set to SCISDK_LIST_RAW_BUFFER_MAGIC
`data` : it's the array of the raw data. The data is stored as a char array. The user must cast it to the correct type.
For example if input signal size is 64 bit, the user must cast the data to uint64_t*.
This example show how to cast the data to uint64_t*:
```c
uint64_t *data = (uint64_t*)list_data->data;
for (int i = 0; i < list_data->info.valid_samples; i++) {
	printf("%d\n", data[i]);
}
```

If the size of the input signal is greater than 64 bit, several programming language offer buffer data type or the user can treat it as an array of char.
Another option is to use pack struct.
Immagine that the input size is 128 bits organized as follows:

- 64 bit for the timestamp
- 32 bit for the energy 
- 8 bit for the channel
- 16 bit for the flags

This example show how to cast the data to a pack struct:

```c

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#else
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

PACK(typedef struct DATA_STRUCT {
	uint64_t timestramp;
	uint32_t energy;
	uint8_t channel;
	uint16_t flags;
});

DATA_STRUCT *data = (DATA_STRUCT*)list_data->data;
for (int i = 0; i < list_data->info.valid_samples; i++) {
	printf("%d\n", data[i].timestramp);
	printf("%d\n", data[i].energy);
	printf("%d\n", data[i].channel);
	printf("%d\n", data[i].flags);
}
```

It is even possible to work at bit level to specify exactly the bit meaning of each element
Imagine we have a 32 bit word with following bit organization:
- 8 bit channel
- 1 bit valid
- 1 bit pileup
- 1 bit overflow
- 1 bit veto marker
- 3 bit padding to zero
- 16 bit energy
  
This example show how to cast the data to a pack struct:

```c
PACK(typedef struct DATA_STRUCT {
	uint8_t channel:8;
	uint8_t valid:1;
	uint8_t pileup:1;
	uint8_t overflow:1;
	uint8_t veto_marker:1;
	uint8_t padding:3;
	uint16_t energy:16;
});

DATA_STRUCT *data = (DATA_STRUCT*)list_data->data;

for (int i = 0; i < list_data->info.valid_samples; i++) {
	printf("%d\n", data[i].channel);
	printf("%d\n", data[i].valid);
	printf("%d\n", data[i].pileup);
	printf("%d\n", data[i].overflow);
	printf("%d\n", data[i].veto_marker);
	printf("%d\n", data[i].energy);
}
```




## Basic Examples
### C
```c
SCISDK_LIST_RAW_BUFFER *lrb;
ret = SCISDK_AllocateBufferSize((char*)("board0:/MMCComponents/List_0"), T_BUFFER_TYPE_RAW, (void**)&lrb, _sdk, 1024);

if (ret != NI_OK) {
	printf("Error allocating buffer\n");
	return -1;
}

SCISDK_SetParameterString("board0:/MMCComponents/List_0.thread", "false", _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/List_0.timeout", 5000, _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/List_0.acq_mode", "blocking", _sdk);
SCISDK_ExecuteCommand("board0:/MMCComponents/List_0.start", "", _sdk);

ret = SCISDK_ReadData("board0:/MMCComponents/List_0", (void *)lrb, _sdk);	
		
```

### C++
```cpp
SCISDK_LIST_RAW_BUFFER *lrb;
sdk->AllocateBuffer("board0:/MMCComponents/List_0", T_BUFFER_TYPE_RAW, (void**)&lrb, 1024);
sdk->SetParameter("board0:/MMCComponents/List_0.thread", "false");
sdk->SetParameter("board0:/MMCComponents/List_0.timeout", 5000);
sdk->SetParameter("board0:/MMCComponents/List_0.acq_mode", "blocking");
sdk->ExecuteCommand("board0:/MMCComponents/List_0.start", "");

int res = sdk->ReadData("board0:/MMCComponents/List_0", (void *)lrb);

if (res != NI_OK) {
	printf("Error reading data\n");
	return -1;
}
```

### Python
```python
res, lrb = sdk.AllocateBuffer("board0:/MMCComponents/List_0", 1024)

if res != 0:
	print("Error allocating buffer")
	exit(-1)

sdk.SetParameterString("board0:/MMCComponents/List_0.thread", "false")
sdk.SetParameterInteger("board0:/MMCComponents/List_0.timeout", 5000)
sdk.SetParameterString("board0:/MMCComponents/List_0.acq_mode", "blocking")
sdk.ExecuteCommand("board0:/MMCComponents/List_0.start", "")

res, lrb = sdk.ReadData("board0:/MMCComponents/List_0", lrb)
```

### C Sharp

```csharp
SciSDKListRawBuffer lrb = new SciSDKListRawBuffer();
sdk.AllocateBuffer("board0:/MMCComponents/List_0", 0, ref lrb, 1024);
sdk.SetParameter("board0:/MMCComponents/List_0.thread", "false");
sdk.SetParameter("board0:/MMCComponents/List_0.timeout", 5000);
sdk.SetParameter("board0:/MMCComponents/List_0.acq_mode", "blocking");
sdk.ExecuteCommand("board0:/MMCComponents/List_0.start", "");

int res = sdk.ReadData("board0:/MMCComponents/List_0", ref lrb);

if (res != 0) {
	Console.WriteLine("Error reading data");
	return -1;
}

```

### VB.NET


```vb

Dim lrb As New SciSDKListRawBuffer()
sdk.AllocateBufferSize("board0:/MMCComponents/List_0", 0, lrb, 1024)
sdk.SetParameter("board0:/MMCComponents/List_0.thread", "false")
sdk.SetParameter("board0:/MMCComponents/List_0.timeout", 5000)
sdk.SetParameter("board0:/MMCComponents/List_0.acq_mode", "blocking")
sdk.ExecuteCommand("board0:/MMCComponents/List_0.start", "")

Dim res As Integer = sdk.ReadData("board0:/MMCComponents/List_0", lrb)

If res <> 0 Then
	Console.WriteLine("Error reading data")
	Return -1
End If

```


## Additional Examples

### Decode list data in C

A multichannel system is used to detect neutron and gamma.
Neutron has a longer dacay time and gamma has a shorter decay time.
The system will use a charge integrator with two time windows to calculate Qshoe and Qlong.

Decode a 64 bit list data with the following bit organization:
- 5 bit channel
- 1 bit valid
- 1 bit pileup
- 1	padding
- 24 bit timestamp
- 16 bit Qshort
- 16 bit Qlong

```c

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#else
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

PACK(typedef struct DATA_STRUCT {
	uint8_t channel:5;
	uint8_t valid:1;
	uint8_t pileup:1;
	uint8_t padding:1;
	uint32_t timestamp:24;
	uint16_t qshort:16;
	uint16_t qlong:16;
}DATA_STRUCT);


SCISDK_LIST_RAW_BUFFER *lrb;
ret = SCISDK_AllocateBufferSize((char*)("board0:/MMCComponents/List_0"), T_BUFFER_TYPE_RAW, (void**)&lrb, _sdk, 1024);

if (ret != NI_OK) {
	printf("Error allocating buffer\n");
	return -1;
}

SCISDK_SetParameterString("board0:/MMCComponents/List_0.thread", "false", _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/List_0.timeout", 5000, _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/List_0.acq_mode", "blocking", _sdk);
SCISDK_ExecuteCommand("board0:/MMCComponents/List_0.start", "", _sdk);

ret = SCISDK_ReadData("board0:/MMCComponents/List_0", (void *)lrb, _sdk);
if (ret == NI_OK) {
	
DATA_STRUCT *data = (DATA_STRUCT*)list_data->data;

	for (int i = 0; i < list_data->info.valid_samples; i++) {
		printf("%d\n", data[i].channel);
		printf("%d\n", data[i].valid);
		printf("%d\n", data[i].pileup);
		printf("%d\n", data[i].timestamp);
		printf("%d\n", data[i].qshort);
		printf("%d\n", data[i].qlong);
	}
}
```

### Use non-blocking mode in C

```c
SCISDK_LIST_RAW_BUFFER *lrb;
ret = SCISDK_AllocateBufferSize((char*)("board0:/MMCComponents/List_0"), T_BUFFER_TYPE_RAW, (void**)&lrb, _sdk, 100);

if (ret != NI_OK) {
	printf("Error allocating buffer\n");
	return -1;
}

SCISDK_SetParameterString("board0:/MMCComponents/List_0.thread", "false", _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/List_0.acq_mode", "non-blocking", _sdk);
SCISDK_ExecuteCommand("board0:/MMCComponents/List_0.start", "", _sdk);

while (1) {
	ret = SCISDK_ReadData("board0:/MMCComponents/List_0", (void *)lrb, _sdk);
	if (ret == NI_OK) {
		uint32_t *data;
		data = (uint32_t*)list_data->data;
		for (int i = 0; i < list_data->info.valid_samples; i++) {
			printf("%d\n", data[i]);
		}
	}
}

