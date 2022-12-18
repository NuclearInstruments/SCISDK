# Board Endpoint driver

The SciCompiler design are based on two parts:

- a framework, which is not configurable by the user 
- the user firmware, which is the diagram constituting the firmware designed by the user

All configurable and software-readable modules (IPs) have a specific driver within SciSDK. For example, the registers of the user firmware are configurable through the SciSDK Register Driver.
The parameters or endpoints of the framework are managed by the board driver, which exposes a specific endpoint, the `boardapi`: `board0:/boardapi/`

Within the framework, there may be:
- board configuration parameters: amplifier and shaper configuration, offset, I/O setup
- command execution: system reboot, start acquisition
- endpoint reading: reading of the opendpp endpoint or the openscope endpoint"

## Supported functions:

Following the list of supported functions:

- SCISDK_ExecuteCommand("path", "args", sdk)
- SCISDK_SetParameterString("path", "value", sdk)
- SCISDK_SetParameterInteger("path", value, sdk)
- SCISDK_SetParameterDouble("path", value, sdk)
- SCISDK_GetParameterString("path", "value", sdk)
- SCISDK_GetParameterInteger("path", value, sdk)
- SCISDK_GetParameterDouble("path", value, sdk)
- SCISDK_AllocateBuffer("path", buffer_type, buffer, sdk)
- SCISDK_AllocateBufferSize("path", buffer_type, buffer, sdk, size)
- SCISDK_FreeBuffer("path", buffer_type, buffer, sdk)
- SCISDK_ReadData("path", buffer, sdk)
- SCISDK_ReadStatus("path", status, sdk)

Consider that the command are typically executed inside the specific board SDK dll indeed it is not possible to make the automatic format conversion.
The data type must correspond to the datatype of the specific board SDK.

## Board specific endpoint

This are the board specific endpoint for each hardware:
- [OpenDPP](boards/opendpp.md)
- [OpenScope](boards/openscope.md)
- [DT1260](boards/dt1260.md)
- [X5560SE](boards/x5560se.md)
- [R5560](boards/x5560.md)
- [DT5550](boards/dt5550.md)
- [DT5550W - Citiroc](boards/dt5550w_citiroc.md)
- [DT5550W - Petiroc](boards/dt5550w_petiroc.md)