# OpenDPP Board Driver

Caen board uses a custom endpoint to readout data with the host using accelerated DMA transfers and DDR4 memory. 
This driver is compatible with the following boards:
- V2740
- DT2740
- V2745
- DT2745
- VX2730
- DT2730

The SciCompiler project must be created as DPP project. Scope firmware uses the OpenScope driver.

## Access to board endpoints

Refere to the [Board endpoint driver](board_endpoint.md) documentation to understand how to configure and read board specific parameters.

Each board has a specific API set to configure framework paramenters that are outside the scope of SciCompiler project.
This board parameters are accessible through the following path:

`board0:/boardapi/`

This path is a virtual path that is mapped to the board specific framework control endpoint. All read/write operations are performed directly to the board SDK and it is managed by the SDK.

For FELib compatible board (CAEN X274X) the board specific API is accessible through the following path:

`board0:/boardapi/felib/`

any path after the last `/` is mapped to the board specific SDK. For example:

`board0:/boardapi/felib/ch/0/par/ChEnable`

ask to the FELib SDK to set/read the `ChEnable` parameter of the channel 0.

The documentation of the board specific API is available in the [board SDK documentation](https://www.caen.it/products/caen-felib-library/)


**Data type in SetParameter must be STRING, indeed just the SCISDK_SetParameterString function is supported because the felib requires all parameters to be a string.**

## OpenDPP data structure

The OpenDPP driver uses a custom data structure to store the data readout from the board. Aldo the documentation of felib explain that the user may customize which field are stored in the data structure, the OpenDPP driver uses the following fixed structure:

```c
typedef struct {
		uint8_t channel;
		uint64_t timestamp;
		uint64_t timestamp_ns;
		uint16_t fine_timestamp;
		uint16_t energy;
		uint16_t flags_b;
		uint8_t flags_a;
		uint16_t psd;
		uint64_t* user_info;
		uint32_t user_info_size;
		bool truncated;
		uint16_t* waveform;
		uint32_t waveform_size;
		bool board_fail;
		bool special_event;
		uint32_t event_size;
		bool flush;
		uint32_t aggregate_counter;
	} SCISDK_FE_OPENDPP_EVENT;
```

`channel`: Channel (7 bits)

`timestamp`: Timestamp (48 bits)

`timestamp_ns`: Timestamp in nanoseconds (51 bits)

`fine_timestamp`: Fine timestamp (10 bits)

`energy`: Energy (16 bits)

`flags_b`: Event low priority flags (12 bits). See Low Priority.

`flags_a`: Event high priority flags (8 bits). See High Priority.

`psd`: Pulse Shape Discrimination (16 bits)

`user_info`: User Info word

`user_info_size`: Number of User Info word

`truncated`: Set if the event is truncated

`waveform`:  Waveform samples

`waveform_size`: Number of waveform samples.

`board_fail`: Set if the logical AND between of the ErrorFlag and ErrorFlagDataMask is not 0. Present in all the events

`special_event`: Set if Special event

`event_size`: Total event raw size, useful for statistics and to discriminate between compressed events (if == 8, see EnDataReduction) and standard events (if > 8).

`flush bool`: Flush of the aggregate word. Shared by all the events in the aggregate. Useful for debug purposes.

`aggregate_counter`: Aggregate counter section in the Event Aggregate structure. Shared by all the events

Refer to the [FELib documentation](https://www.caen.it/products/caen-felib-library/) for more information.

## Run Control
The digitizers must be armed before can start any acquisition:

```c
int ret;
ret = SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/armacquisition", _sdk);
```
In case of start source is set to software start, this will start an acquisition run:
```c
ret = SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/swstartacquisition", _sdk_);
```
Similarly, to send a software stop and disarm the acquisition:
```c
ret = SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/swstopacquisition", _sdk);
ret = SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/disarmacquisition", _sdk);
```
## Read Data Loop

To read data from the digitizer as soon as it becomes available, the easier approach is to implement a loop around the `SCISDK_ReadData()` function, as demonstrated in the provided demo.

If an event is already available in the endpoint, the function returns immediately with `NI_OK`.  The main difference compared to the OpenScope framework is that, in this case, the event consists of information from only a single channel. If no events are available within 10 seconds, it returns `NI_TIMEOUT`. Once acquisition has stopped, any remaining data stored in the digitizer can still be read until the function returns `NI_LAST`, indicating that no more data is available from that acquisition session. 

These return values can be used to build an efficient acquisition loop. The following code snippet illustrates this logic:

```c
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
In case of success the function will return one event at the time.

## Code Examples

Configure the board to readout channel 0,1,2 and store the data in a csv file, trigger comes from SciCompiler trigger to each channel FIFO.

```c++
	void* _sdk = SCISDK_InitLib();

    uint32_t v;
	string test;
	int res = 0;
	char* ret;
	bool acq_status = true;
	std::ofstream out("c:/temp/output.csv");
	
	out << "Ch	Timestamp" << endl;


	res = SCISDK_AddNewDevice("10.105.250.18", "V2740", "C:/RegisterFile.json", "board0", _sdk);
	SCISDK_s_error(res, &ret, _sdk);
	if (res) std::exit(ret);
	SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/reset", "", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/readout.datatype", "dpp", _sdk);
    SCISDK_SetParameterString("board0:/boardapi/felib/par/TrgOutMode", "UserTrgout", _sdk);


	SCISDK_SetParameterString("board0:/boardapi/felib/ch/0..63/par/ChEnable", "false", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/ch/0/par/ChEnable", "true", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/ch/1/par/ChEnable", "true", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/ch/2/par/ChEnable", "true", _sdk);
	
    SCISDK_SetParameterString("board0:/boardapi/felib/par/EnDataReduction", "true", _sdk);

	SCISDK_SetParameterString("board0:/boardapi/felib/par/TestPulsePeriod", "100000", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/par/TestPulseWidth", "1000", _sdk);

	SCISDK_SetParameterString("board0:/boardapi/felib/par/ITLAMask", "0x1", _sdk);

	SCISDK_SetParameterString("board0:/boardapi/felib/par/ITLAPairLogic", "None", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/par/ITLAMajorityLev", "0", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/par/ITLAMainLogic", "OR", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/par/ITLAGateWidth", "800", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/par/ITLAPolarity", "Direct", _sdk);

	SCISDK_SetParameterString("board0:/boardapi/felib/par/AcqTriggerSource", "ITLA", _sdk);

	SCISDK_ExecuteCommand("board0:/boardapi/felib/par/startsource", "SWcmd", _sdk);
	SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/armacquisition", "", _sdk);
	SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/swstartacquisition", "", _sdk);
	//let run the board for 1 sec
	Sleep(1000);
	//disarm acquisition
	SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/armacquisition", "", _sdk);
	SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/swstartacquisition", "", _sdk);

	SCISDK_FE_OPENDPP_EVENT* dpp_evnt;

	SCISDK_AllocateBuffer("board0:/boardapi", T_BUFFER_TYPE_DECODED, (void**)&dpp_evnt, _sdk);

	while (acq_status) {
    	ret = SCISDK_ReadData("board0:/boardapi", dpp_evnt, _sdk);
    	switch (ret) {
        	case NI_OK:
				out << (int)dpp_evnt->channel << "\t" << std::hex << dpp_evnt->timestamp << endl;
            	// Process the event data here, or proceed to the next read.
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

