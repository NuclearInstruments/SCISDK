# OpenScope driver

Caen board uses a custom endpoint to readout data with the host using accelerated DMA transfers and DDR4 memory. 
This driver is compatible with the following boards:
- V2740
- DT2740
- V2745
- DT2745

The SciCompiler project must be created as Scope project. DPP firmware uses the OpenDPP driver.

It is important to note that OpenScope readout endpoint allows a much faster and deeper waveform readout data than Oscilloscope or Digitizer SciCompiler embedded IP. 

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
`waveform`: A pointer to an array of pointers to 16-bit unsigned integers, where each element in the array represents a channel of the oscilloscope and each element in the array of pointers represents a sample in that channel.
`n_samples`: A pointer to a size_t representing the number of samples in each channel of the oscilloscope.
`n_allocated_samples`: A pointer to a size_t representing the number of samples that have been allocated for each channel of the oscilloscope.
`n_channels`: An unsigned integer representing the number of channels in the oscilloscope

in order to access to the j-th sample of the i-th channel use the following syntax:

`event->waveform[i][j]`

## Code Examples

This example configure the board to acquire 50 samples. The acquisition is triggered by a software trigger and the test pulse is enabled. The test pulse period is 100 us and the test pulse width is 1 us.
The data is readout and saved in a text file, one channel for column, one sample per row.

```c++
    void* _sdk = SCISDK_InitLib();

	int res = 0;
	
	res = SCISDK_AddNewDevice("10.105.250.18", "V2740", "SciSDKDev2740Wave.json", "board0", _sdk);
	SCISDK_s_error(res, &res, _sdk);
	if (res) std::exit(res);


	SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/reset", "", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/readout.datatype", "scope", _sdk);

	SCISDK_SetParameterString("board0:/boardapi/felib/ch/0..63/par/ChEnable", "true"), _sdk);

	SCISDK_SetParameterString("board0:/boardapi/felib/par/RecordLengthS", "50", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/par/PreTriggerS", "4", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/par/AcqTriggerSource", "SwTrg | TestPulse", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/par/TestPulsePeriod", "100000", _sdk);
	SCISDK_SetParameterString("board0:/boardapi/felib/par/TestPulseWidth", "1000", _sdk);
	SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/armacquisition", "", _sdk);
	SCISDK_ExecuteCommand("board0:/boardapi/felib/cmd/swstartacquisition", "", _sdk);

	SCISDK_FE_SCOPE_EVENT* evnt;
	SCISDK_AllocateBuffer("board0:/boardapi", T_BUFFER_TYPE_DECODED, (void**)&evnt, 50, _sdk);
	SCISDK_ReadData("board0:/boardapi", evnt, _sdk);

    // save in a text file all data, one channel for column, one sample per row

    ofstream myfile;
    myfile.open("example.txt");

    for (int i = 0; i < evnt->n_samples; ++i) {
        for (int j = 0; j < evnt->n_channels; j++) {
            myfile << evnt->waveform[j][i] << "  ";
        }
        myfile << endl;
    }
    myfile.close();

```

		
	