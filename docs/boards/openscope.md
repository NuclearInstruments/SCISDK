# OpenScope Board Driver

Caen board uses a custom endpoint to readout data with the host using accelerated DMA transfers and DDR4 memory. 
This driver is compatible with the following boards:
- V/XV2740
- DT2740
- V/VX2745
- DT2745
- VX2730
- DT2730

The SciCompiler project must be created as Scope project. DPP firmware uses the OpenDPP driver.

It is important to note that OpenScope readout endpoint allows a much faster and deeper waveform readout data than Oscilloscope or Digitizer SciCompiler embedded IP. 

## Access to board endpoints

Refere to the [Board endpoint driver](board_endpoint.md) documentation to understand how to configure and read board specific parameters.

Each board has a specific API set to configure framework paramenters that are outside the scope of SciCompiler project.
This board parameters are accessible through the following path:

`board0:/boardapi/`

This path is a virtual path that is mapped to the board specific framework control endpoint. All read/write operations are performed directly to the board SDK and it is managed by the SDK.

For FELib compatible board (CAEN X27XX) the board specific API is accessible through the following path:

`board0:/boardapi/felib/`

any path after the last `/` is mapped to the board specific SDK. For example:

`board0:/boardapi/felib/ch/0/par/ChEnable`

ask to the FELib SDK to set/read the `ChEnable` parameter of the channel 0.

The documentation of the board specific API is available in the [board SDK documentation](https://www.caen.it/products/caen-felib-library/)


**Data type in SetParameter must be STRING, indeed just the SCISDK_SetParameterString function is supported because the felib requires all parameters to be a string.**

## OpenScope data structure

The OpenScope driver uses a custom data structure to store the waveform readout from the board.

```c
	typedef struct  {
		uint64_t timestamp;
		uint32_t trigger_id;
		size_t event_size;
		uint16_t** waveform;
		size_t* n_samples;
		size_t* n_allocated_samples;
		unsigned n_channels;
	} SCISDK_FE_SCOPE_EVENT;


```

`timestamp`: A 64-bit unsigned integer representing the timestamp of the event in nanoseconds.

`trigger_id`: A 32-bit unsigned integer representing the trigger identifier for the event.

`event_size`: A size_t representing the size of the event data in bytes.

`waveform`: A pointer to an array of pointers to 16-bit unsigned integers, where each element in the array represents a channel of the oscilloscope and each element in the array of pointers represents a sample in that channel. In order to access to the j-th sample of the i-th channel use the following syntax: `event->waveform[i][j]`

`n_samples`: A pointer to a size_t representing the number of samples in each channel of the oscilloscope.

`n_allocated_samples`: A pointer to a size_t representing the number of samples that have been allocated for each channel of the oscilloscope.

`n_channels`: An unsigned integer representing the number of channels in the oscilloscope. Since the event consists of data from all channels, this number also corresponds to the length of the arrays `n_allocated_samples`, `n_samples`, and `waveform`.

## Run Control
The digitizers must be armed before can start any acquisition:

```c++
int ret;
ret = SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/armacquisition", _sdk);
```
In case of start source is set to software start, this will start an acquisition run:
```c++
ret = SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/swstartacquisition", _sdk_);
```
Similarly, to send a software stop and disarm the acquisition:
```c++
ret = SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/swstopacquisition", _sdk);
ret = SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/disarmacquisition", _sdk);
```
## Read Data Loop

To read data from the digitizer as soon as it becomes available, the easier approach is to implement a loop around the `SCISDK_ReadData()` function, as demonstrated in the provided demo.

If an event is already available in the endpoint, the function returns immediately with `NI_OK`. If no events are available within 10 seconds, it returns `NI_TIMEOUT`. Once acquisition has stopped, any remaining data stored in the digitizer can still be read until the function returns `NI_LAST`, indicating that no more data is available from that acquisition session.

These return values can be used to build an efficient acquisition loop. The following code snippet illustrates this logic:

```c++
while (true) {
    ret = SCISDK_ReadData("board0:/boardapi", evnt, _sdk);
    switch (ret) {
        case NI_OK:
            // Process the event data here, or proceed to the next read.
            break;
        case NI_TIMEOUT:
            // No event was available within the timeout window.
            break;
        case NI_LAST:
            printf("\nAcquisition complete. No more data available.\n");
            return ret;
    }
}
```

## Code Examples

This example configure the board to acquire 50 samples. The acquisition is triggered by a software trigger and the test pulse is enabled. The test pulse period is 100 us and the test pulse width is 1 us.
The data is readout and saved in a text file, one channel for column, one sample per row.

```c++
    void* _sdk = SCISDK_InitLib();

	int res = 0;
	bool acq_status = true;
	std::ofstream out("c:/temp/output.csv");
	out << "Trigger ID	Timestamp" << endl;
	
	res = SCISDK_AddNewDevice("10.105.250.18", "V2740", "SciSDKDev2740Wave.json", "board0", _sdk);
	SCISDK_s_error(res, &ret, _sdk);
	if (res) std::exit(ret);


	SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/reset", "", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/readout.datatype", "scope", _sdk);

	SCISDK_SetParameterString("board0:/boardapi/felib/ch/0..63/par/ChEnable", "true", _sdk);

	SCISDK_SetParameterString("board0:/boardapi/felib/par/RecordLengthS", "50", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/par/PreTriggerS", "4", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/par/AcqTriggerSource", "SwTrg | TestPulse", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/par/TestPulsePeriod", "100000", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/par/TestPulseWidth", "1000", _sdk);
	SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/armacquisition", "", _sdk);
	SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/swstartacquisition", "", _sdk);
	//let run the board for 1 sec
	Sleep(1000);
	//disarm acquisition
	SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/armacquisition", "", _sdk);
	SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/swstartacquisition", "", _sdk);

	SCISDK_FE_SCOPE_EVENT* evnt;
	SCISDK_AllocateBufferSize("board0:/boardapi", T_BUFFER_TYPE_DECODED, (void**)&evnt, _sdk, 50);
	while (acq_status) {
    	ret = SCISDK_ReadData("board0:/boardapi", evnt, _sdk);
    	switch (ret) {
        	case NI_OK:
            	// Process the event data here, or proceed to the next read.
				out << evnt->trigger_id << "\t" << std::hex << evnt->timestamp << endl;
            	break;
        	case NI_TIMEOUT:
            	// No event was available within the timeout window.
            	break;
        	case NI_LAST:
            	printf("\nAcquisition completed. No more data available.\n");
            	acq_status = false;
				break;
			default:
				out << "Error during the acquisition\n" << std::endl;
				out.close();
				return res;
		}
	}
	out.close();
```