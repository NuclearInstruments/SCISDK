# Logic Analyser driver

The logic analyser allows to acquire digital signals and download them to the user PC.

The logic analyser can be configured to acquire multiple digital channels.
The acquisition can be triggered by an external signal, by a software trigger or by edge detection on the digital channels.

## Parameters
The following parameters can be configured:

| Parameter             | Access Mode | Description                                                                              | Default value |
| --------------------- | ----------- | ---------------------------------------------------------------------------------------- | ------------- |
| trigger_mode          | R/W         | set trigger mode: software, external, edge                                               | software      |
| trigger_rising_mask   | R/W         | set rising edge trigger enable mask (string of 0/1, one per trace)                      | all zeros     |
| trigger_falling_mask  | R/W         | set falling edge trigger enable mask (string of 0/1, one per trace)                     | all zeros     |
| acq_mode              | R/W         | set acquisition mode: blocking, non-blocking                                             | blocking      |
| timeout               | R/W         | set timeout for blocking acquisition in ms                                               | 5000          |
| ntraces               | R           | get number of digital traces (signals)                                                   |               |
| nsamples              | R           | get number of logical samples                                                            |               |
| buffer_type           | R           | get buffer type: SCISDK_LOGICANALYSER_DECODED_BUFFER                                     |               |

### Trigger Modes
Following trigger modes are available:
- **software** triggers the acquisition immediately when the start command is issued. This mode is useful for free-running acquisitions.

- **external** triggers the acquisition when the external trigger signal is received. The external trigger is applied to the TRIGGER input on the Logic Analyser block.

- **edge** triggers the acquisition when one of the enabled digital channels detects a rising or falling edge. The channels to monitor are configured using the trigger masks.

### Trigger Masks
When using **edge** trigger mode, you can configure which channels should trigger the acquisition:
- **trigger_rising_mask**: A string of '0' and '1' characters, one per digital trace. Set to '1' to enable rising edge trigger on that channel.
- **trigger_falling_mask**: A string of '0' and '1' characters, one per digital trace. Set to '1' to enable falling edge trigger on that channel.

Example: For 44 traces, to trigger on rising edge of BUS_0 and BUS_10:
```
trigger_rising_mask = "10000000001000000000000000000000000000000000"
```

The trigger will fire when ANY of the enabled channels detects the specified edge.

### Acquisition Modes
Following acquisition modes are available:
- **blocking** mode waits for the acquisition to complete before returning from the ReadData function. The **timeout** can be set with the timeout parameter.

- **non-blocking** mode returns immediately from the ReadData function. If trigger does not occur within the timeout, the function returns NI_TIMEOUT

## Commands
The following commands are available:

| Command               | Description                                                                    | Parameter |
| --------------------- | ------------------------------------------------------------------------------ | --------- |
| start                 | start the logic analyser acquisition and arm the trigger                       |           |
| reset                 | reset the logic analyser and clear the FIFO                                    |           |

## Output data Format
### Decoded Data
The data output structure is the following:
```c
typedef struct {
    uint32_t magic;
    uint32_t *data;
    uint64_t timecode;
    struct  {
        uint32_t samples;
        uint32_t ntraces;
        uint32_t words_per_sample;
    } info;
} SCISDK_LOGICANALYSER_DECODED_BUFFER;
```

**magic**: magic number to identify the buffer type. It is set to BUFFER_TYPE_LOGICANALYSER_DECODED

**data**: array of raw samples from the logic analyser. Each logical sample can occupy multiple 32-bit words if the number of traces exceeds 32.

**timecode**: the timestamp of the first sample acquisition.

**info**: contains information about the logic analyser buffer:
- **samples**: the number of logical samples acquired
- **ntraces**: the number of digital traces (signals) defined in the configuration
- **words_per_sample**: number of 32-bit words per logical sample (calculated as (ntraces + 31) / 32)

### Data Organization
The data is organized as consecutive words, with multiple words per sample if ntraces > 32.

**Example with nsamples=1024, ntraces=44:**
- `words_per_sample = 2` (44 traces need 2×32-bit words)
- Total array size = 1024 × 2 = 2048 words

```
data[0]    = sample 0, bits 0-31
data[1]    = sample 0, bits 32-43
data[2]    = sample 1, bits 0-31
data[3]    = sample 1, bits 32-43
...
data[2046] = sample 1023, bits 0-31
data[2047] = sample 1023, bits 32-43
```

### Extracting Individual Signals
To extract a specific signal from the data, use the bitstart and bitwidth information from the ChannelsList in the JSON configuration:

```c
// To extract signal i from logical sample j:
int word_idx = j * words_per_sample + (bitstart_i / 32);
int bit_pos = bitstart_i % 32;
uint32_t value = (data[word_idx] >> bit_pos) & ((1 << bitwidth_i) - 1);
```

**Example**: Extract BUS_5 (bitstart=5, bitwidth=1) from sample 100 with words_per_sample=2:
```c
int word_idx = 100 * 2 + (5 / 32) = 200;
int bit_pos = 5 % 32 = 5;
uint32_t bus5_value = (data[200] >> 5) & 0x1;
```

## Basic Examples
### C
```c
SCISDK_SetParameterString("board0:/MMCComponents/LogicAnalyser_0.trigger_mode", "software", _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/LogicAnalyser_0.acq_mode", "blocking", _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/LogicAnalyser_0.timeout", 1000, _sdk);

SCISDK_LOGICANALYSER_DECODED_BUFFER *lb;
int res = SCISDK_AllocateBuffer("board0:/MMCComponents/LogicAnalyser_0", T_BUFFER_TYPE_DECODED, (void**)&lb, _sdk);

if (res != NI_OK) {
    printf("Error allocating buffer\n");
    return -1;
}

res = SCISDK_ExecuteCommand("board0:/MMCComponents/LogicAnalyser_0.start", "", _sdk);
if (res != NI_OK) {
    printf("Error starting logic analyser\n");
    return -1;
}

res = SCISDK_ReadData("board0:/MMCComponents/LogicAnalyser_0", (void *)lb, _sdk);

if (res == NI_OK) {
    printf("Acquired %d samples\n", lb->info.samples);
    printf("Number of traces: %d\n", lb->info.ntraces);
    printf("Words per sample: %d\n", lb->info.words_per_sample);

    // Example: extract first bit of each sample (BUS_0)
    for (int i = 0; i < lb->info.samples; i++) {
        int word_idx = i * lb->info.words_per_sample;
        uint32_t bit_value = lb->data[word_idx] & 0x1;
        printf("Sample %d, BUS_0 = %d\n", i, bit_value);
    }
}

SCISDK_FreeBuffer("board0:/MMCComponents/LogicAnalyser_0", T_BUFFER_TYPE_DECODED, (void**)&lb, _sdk);
```

### C++
```c++
sdk->SetParameter("board0:/MMCComponents/LogicAnalyser_0.trigger_mode", "software");
sdk->SetParameter("board0:/MMCComponents/LogicAnalyser_0.acq_mode", "blocking");
sdk->SetParameter("board0:/MMCComponents/LogicAnalyser_0.timeout", 1000);

SCISDK_LOGICANALYSER_DECODED_BUFFER *lb;
int res = sdk->AllocateBuffer("board0:/MMCComponents/LogicAnalyser_0", T_BUFFER_TYPE_DECODED, (void**)&lb);

if (res != NI_OK) {
    cout << "Error allocating buffer" << endl;
    return -1;
}

res = sdk->ExecuteCommand("board0:/MMCComponents/LogicAnalyser_0.start", "");
res = sdk->ReadData("board0:/MMCComponents/LogicAnalyser_0", (void *)lb);

if (res == NI_OK) {
    cout << "Acquired " << lb->info.samples << " samples" << endl;
    // Process data...
}

sdk->FreeBuffer("board0:/MMCComponents/LogicAnalyser_0", T_BUFFER_TYPE_DECODED, (void**)&lb);
```

### Python
```python
sdk.SetParameterString("board0:/MMCComponents/LogicAnalyser_0.trigger_mode", "software")
sdk.SetParameterString("board0:/MMCComponents/LogicAnalyser_0.acq_mode", "blocking")
sdk.SetParameterInteger("board0:/MMCComponents/LogicAnalyser_0.timeout", 1000)

res, buf = sdk.AllocateBuffer("board0:/MMCComponents/LogicAnalyser_0")
if res != 0:
    print("Error allocating buffer")
    exit(1)

res = sdk.ExecuteCommand("board0:/MMCComponents/LogicAnalyser_0.start", "")
res, buf = sdk.ReadData("board0:/MMCComponents/LogicAnalyser_0", buf)

if res == 0:
    print(f"Acquired {buf.info.samples} samples")
    print(f"Number of traces: {buf.info.ntraces}")
    print(f"Words per sample: {buf.info.words_per_sample}")

    # Extract BUS_0 (bit 0) from each sample
    for i in range(buf.info.samples):
        word_idx = i * buf.info.words_per_sample
        bit_value = buf.data[word_idx] & 0x1
        print(f"Sample {i}, BUS_0 = {bit_value}")

sdk.FreeBuffer("board0:/MMCComponents/LogicAnalyser_0", buf)
```

### C Sharp
```csharp
sdk.SetParameter("board0:/MMCComponents/LogicAnalyser_0.trigger_mode", "software");
sdk.SetParameter("board0:/MMCComponents/LogicAnalyser_0.acq_mode", "blocking");
sdk.SetParameter("board0:/MMCComponents/LogicAnalyser_0.timeout", 1000);

SciSDKLogicAnalyserDecodedBuffer buffer = new SciSDKLogicAnalyserDecodedBuffer();
sdk.AllocateBuffer("board0:/MMCComponents/LogicAnalyser_0", BufferType.BUFFER_TYPE_DECODED, ref buffer);

sdk.ExecuteCommand("board0:/MMCComponents/LogicAnalyser_0.start", "");

if (sdk.ReadData("board0:/MMCComponents/LogicAnalyser_0", ref buffer) == 0)
{
    Console.WriteLine($"Acquired {buffer.info.samples} samples");
    // Process data...
}

sdk.FreeBuffer("board0:/MMCComponents/LogicAnalyser_0", BufferType.BUFFER_TYPE_DECODED, ref buffer);
```

## Additional Examples
### Trigger on external signal
```c
SCISDK_SetParameterString("board0:/MMCComponents/LogicAnalyser_0.trigger_mode", "external", _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/LogicAnalyser_0.acq_mode", "blocking", _sdk);
SCISDK_SetParameterInteger("board0:/MMCComponents/LogicAnalyser_0.timeout", 5000, _sdk);

SCISDK_LOGICANALYSER_DECODED_BUFFER *lb;
SCISDK_AllocateBuffer("board0:/MMCComponents/LogicAnalyser_0", T_BUFFER_TYPE_DECODED, (void**)&lb, _sdk);

SCISDK_ExecuteCommand("board0:/MMCComponents/LogicAnalyser_0.start", "", _sdk);
SCISDK_ReadData("board0:/MMCComponents/LogicAnalyser_0", (void *)lb, _sdk);

SCISDK_FreeBuffer("board0:/MMCComponents/LogicAnalyser_0", T_BUFFER_TYPE_DECODED, (void**)&lb, _sdk);
```

### Trigger on edge detection
```c
// Configure to trigger on rising edge of BUS_0, BUS_2, BUS_5 (assuming 44 traces)
char rising_mask[45] = "10101000000000000000000000000000000000000000";
char falling_mask[45] = "00000000000000000000000000000000000000000000";

SCISDK_SetParameterString("board0:/MMCComponents/LogicAnalyser_0.trigger_mode", "edge", _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/LogicAnalyser_0.trigger_rising_mask", rising_mask, _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/LogicAnalyser_0.trigger_falling_mask", falling_mask, _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/LogicAnalyser_0.acq_mode", "blocking", _sdk);

SCISDK_LOGICANALYSER_DECODED_BUFFER *lb;
SCISDK_AllocateBuffer("board0:/MMCComponents/LogicAnalyser_0", T_BUFFER_TYPE_DECODED, (void**)&lb, _sdk);

SCISDK_ExecuteCommand("board0:/MMCComponents/LogicAnalyser_0.start", "", _sdk);
SCISDK_ReadData("board0:/MMCComponents/LogicAnalyser_0", (void *)lb, _sdk);

SCISDK_FreeBuffer("board0:/MMCComponents/LogicAnalyser_0", T_BUFFER_TYPE_DECODED, (void**)&lb, _sdk);
```

### Non-blocking mode
```c
SCISDK_SetParameterString("board0:/MMCComponents/LogicAnalyser_0.trigger_mode", "external", _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/LogicAnalyser_0.acq_mode", "non-blocking", _sdk);

SCISDK_LOGICANALYSER_DECODED_BUFFER *lb;
SCISDK_AllocateBuffer("board0:/MMCComponents/LogicAnalyser_0", T_BUFFER_TYPE_DECODED, (void**)&lb, _sdk);

SCISDK_ExecuteCommand("board0:/MMCComponents/LogicAnalyser_0.start", "", _sdk);

// Poll ReadData until it returns 0 for a maximum of 10000ms in non-blocking mode
int res = NI_TIMEOUT;
int timeout = 10000;
while ((res == NI_TIMEOUT) && (timeout > 0))
{
    res = SCISDK_ReadData("board0:/MMCComponents/LogicAnalyser_0", (void *)lb, _sdk);
    timeout -= 10;

    if (res == NI_OK) {
        printf("Data acquired!\n");
        // Process data...
    } else {
        Sleep(10);
        // Do something else while waiting...
    }
}

SCISDK_FreeBuffer("board0:/MMCComponents/LogicAnalyser_0", T_BUFFER_TYPE_DECODED, (void**)&lb, _sdk);
```

### Extract and save all traces to CSV file
```c
SCISDK_SetParameterString("board0:/MMCComponents/LogicAnalyser_0.trigger_mode", "software", _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/LogicAnalyser_0.acq_mode", "blocking", _sdk);

SCISDK_LOGICANALYSER_DECODED_BUFFER *lb;
SCISDK_AllocateBuffer("board0:/MMCComponents/LogicAnalyser_0", T_BUFFER_TYPE_DECODED, (void**)&lb, _sdk);

SCISDK_ExecuteCommand("board0:/MMCComponents/LogicAnalyser_0.start", "", _sdk);
int res = SCISDK_ReadData("board0:/MMCComponents/LogicAnalyser_0", (void *)lb, _sdk);

if (res == NI_OK) {
    FILE *fp = fopen("logic_data.csv", "w");
    if (fp != NULL) {
        // Write header
        fprintf(fp, "Sample");
        for (int trace = 0; trace < lb->info.ntraces; trace++) {
            fprintf(fp, ",BUS_%d", trace);
        }
        fprintf(fp, "\n");

        // Write data
        for (int sample = 0; sample < lb->info.samples; sample++) {
            fprintf(fp, "%d", sample);

            // Extract each trace
            for (int trace = 0; trace < lb->info.ntraces; trace++) {
                int word_idx = sample * lb->info.words_per_sample + (trace / 32);
                int bit_pos = trace % 32;
                uint32_t bit_value = (lb->data[word_idx] >> bit_pos) & 0x1;
                fprintf(fp, ",%d", bit_value);
            }
            fprintf(fp, "\n");
        }
        fclose(fp);
        printf("Data saved to logic_data.csv\n");
    }
}

SCISDK_FreeBuffer("board0:/MMCComponents/LogicAnalyser_0", T_BUFFER_TYPE_DECODED, (void**)&lb, _sdk);
```

### Multiple acquisitions with buffer reuse
It is not required to release and reallocate the buffer, it is possible to reuse the same buffer for multiple acquisitions.

```c
SCISDK_SetParameterString("board0:/MMCComponents/LogicAnalyser_0.trigger_mode", "software", _sdk);
SCISDK_SetParameterString("board0:/MMCComponents/LogicAnalyser_0.acq_mode", "blocking", _sdk);

SCISDK_LOGICANALYSER_DECODED_BUFFER *lb;
SCISDK_AllocateBuffer("board0:/MMCComponents/LogicAnalyser_0", T_BUFFER_TYPE_DECODED, (void**)&lb, _sdk);

int res = NI_OK;
int count = 100;
while (count-- > 0) {
    SCISDK_ExecuteCommand("board0:/MMCComponents/LogicAnalyser_0.start", "", _sdk);
    res = SCISDK_ReadData("board0:/MMCComponents/LogicAnalyser_0", (void *)lb, _sdk);
    if (res == NI_OK) {
        // Process the data...
        printf("Acquisition %d completed\n", 100 - count);
    }
}

SCISDK_FreeBuffer("board0:/MMCComponents/LogicAnalyser_0", T_BUFFER_TYPE_DECODED, (void**)&lb, _sdk);
```
