// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SCISDK_DLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SCISDK_DLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef __cplusplus
    #ifdef _MSC_VER
        #ifdef SCISDK_DLL_EXPORTS
            #define SCISDK_DLL_API extern "C" //__declspec(dllexport)// __declspec(dllexport)
        #else
            #define SCISDK_DLL_API extern "C" //__declspec(dllimport)
        #endif
    #else
        #ifdef SCISDK_DLL_EXPORTS
            #define SCISDK_DLL_API __attribute__((visibility("default")))
        #else
            #define SCISDK_DLL_API
        #endif
    
    #endif
#else
    #define SCISDK_DLL_API
#endif

#include "Matlab_scisdk_defines_flat.h"

/**
 * @brief Initialize the SciSDK handles accolating the resources. 
 * This function must be called before any other function.
 * 
 * @return void* pointer to the SciSDK handle 
 */
SCISDK_DLL_API void * SCISDK_InitLib();

/**
 * @brief Release the SciSDK handles and free the resources.
 * 
 * @param handle            handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h   
 */
SCISDK_DLL_API int SCISDK_FreeLib(void *handle);

/**
 * @brief Connect to and hardware device. The function tries to connect to the device,
 * decode the SciCompiler json file and build the tree of memory mapped resources defined in the JSON file.
 * The function create a node for each resource and add it to the tree. 
 * A top node, with name specified in Name field, for the device is also created and added to the tree.
 * All the nodes found in the json are added as children of the top node.
 * 
 * The connection method is specified in the Device path field.
 * To connect to a device connected to the USB port, the Device path field must be set to "usb:<pid>" 
 * where <pid> is the PID of the device. Please pay attention to the fact that some devices emulate
 * ethernet on usb bus, so the connection must be done using the ethernet protocol.
 * 
 * To connect to a device connected to the ethernet port, the Device path field must be set to "<ip>:<port>"
 * where the <ip> is the IP address of the device and <port> is the port number of the server listening 
 * on the device. If port is not specified, the default port for the specific device is used.
 * 
 * <b>This function will load the dinamic library specified for the specific hardware device
 * default path. If the library is found in the execution path of the user software, the library will be loaded
 * Otherwise the library will be loaded searching into the system path.
 * Please pay attention to the fact that the libraries must be compiled with the same architecture used by the 
 * user software.</b>
 * 
 * @param DevicePath        Device path to connect to. Must be in the format "usb:<pid>" or "<ip>:<port>" 
 * @param DeviceModel       String containing the device model. Must be one of the supported devices.
 *                          The supported devices are: DT1260, R5560, R5560SE, DT5560, DT5550, 
 *                          DT5550W-CITIROC, DT5550W-PETIROC, V2740, V2745, DT2740, DT2745 
 * @param JSONFwFilePath    Path to the JSON file containing the firmware description.
 * @param Name              Name of the board. This name will be used to identify the device.
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * 
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_AddNewDevice(char *DevicePath, char *DeviceModel, char *JSONFwFilePath, char *Name, void *handle);

/**
 * @brief Remove a device from the tree of memory mapped resources.
 * The function will call the device specific library disconnect function and 
 * will release the resources allocated by the library for the device.
 * 
 * @param name              Name of the board to remove.
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_DetachDevice(char* name, void*handle);

/**
 * @brief Set the value of a parameter for the specific SciCompiler Memory Mapped Component or
 * route the value to external board library to set board specific parameters.
 * This function accept a \b string as value. Please note that the behaviour of this function
 * is different if the path is a SciSDK managed node or a device specific node managed by the external
 * library. 
 * The SciSDK managed nodes perform a conversion of the string to the correct type and then set the value
 * automatically.
 * If the operation is routed to the external library (ie: ../felib/..) SciSDK has not the information 
 * reguarding the data type required by the external library indeed the che correct SetParamter function 
 * must be call in agreement with the external library required value type.
 * 
 * The SciSDK check that the value is in the correct range or in the acceptance list 
 * for the specific parameter. If the value is not in the correct range or in the acceptance list,
 * the function will return an error and the value will not be set.
 * 
 * @param Path              The path to the parameter to set. The path must be in the format 
 *                          <device name>:.../<resource name>.<parameter name>
 *                          \n
 *                          for example: /board0/MMCComponents/Oscilloscope_0.trigger_mode
 *                          
 * @param Value             The value to set. The value must be a \b string.
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_SetParameterString(char* Path, char* value, void* handle);


/**
 * @brief Set the value of a parameter for the specific SciCompiler Memory Mapped Component or
 * route the value to external board library to set board specific parameters.
 * This function accept a \b integer as value. Please note that the behaviour of this function
 * is different if the path is a SciSDK managed node or a device specific node managed by the external
 * library. 
 * The SciSDK managed nodes perform a conversion of the string to the correct type and then set the value
 * automatically.
 * If the operation is routed to the external library (ie: ../felib/..) SciSDK has not the information 
 * reguarding the data type required by the external library indeed the che correct SetParamter function 
 * must be call in agreement with the external library required value type.
 * 
 * The SciSDK check that the value is in the correct range for the specific parameter. 
 * If the value is not in the correct range the function will return an error and the value will not be set.
 * 
 * @param Path              The path to the parameter to set. The path must be in the format 
 *                          <device name>:.../<resource name>.<parameter name>
 *                          \n
 *                          for example: /board0/MMCComponents/Oscilloscope_0.trigger_level
 *                          
 * @param Value             The value to set. The value must be a \b integer.
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_SetParameterInteger(char* Path, int value, void*handle);

/**
 * @brief Set the value of a parameter for the specific SciCompiler Memory Mapped Component or
 * route the value to external board library to set board specific parameters.
 * This function accept a \b integer as value. Please note that the behaviour of this function
 * is different if the path is a SciSDK managed node or a device specific node managed by the external
 * library.
 * The SciSDK managed nodes perform a conversion of the string to the correct type and then set the value
 * automatically.
 * If the operation is routed to the external library (ie: ../felib/..) SciSDK has not the information
 * reguarding the data type required by the external library indeed the che correct SetParamter function
 * must be call in agreement with the external library required value type.
 *
 * The SciSDK check that the value is in the correct range for the specific parameter.
 * If the value is not in the correct range the function will return an error and the value will not be set.
 *
 * @param Path              The path to the parameter to set. The path must be in the format
 *                          <device name>:.../<resource name>.<parameter name>
 *                          \n
 *                          for example: /board0/MMCComponents/Oscilloscope_0.trigger_level
 *
 * @param Value             The value to set. The value must be a \b uint32_t.
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h
 */
SCISDK_DLL_API int SCISDK_SetParameterUInteger(char* Path, uint32_t value, void* handle);

/**
 * @brief Set the value of a parameter for the specific SciCompiler Memory Mapped Component or
 * route the value to external board library to set board specific parameters.
 * This function accept a \b double as value. Please note that the behaviour of this function
 * is different if the path is a SciSDK managed node or a device specific node managed by the external
 * library. 
 * The SciSDK managed nodes perform a conversion of the string to the correct type and then set the value
 * automatically.
 * If the operation is routed to the external library (ie: ../felib/..) SciSDK has not the information 
 * reguarding the data type required by the external library indeed the che correct SetParamter function 
 * must be call in agreement with the external library required value type.
 * 
 * The SciSDK check that the value is in the correct range for the specific parameter. 
 * If the value is not in the correct range the function will return an error and the value will not be set.
 * 
 * @param Path              The path to the parameter to set. The path must be in the format 
 *                          <device name>:.../<resource name>.<parameter name>
 *                          \n
 *                          for example: /board0/MMCComponents/Oscilloscope_0.trigger_level
 *                          
 * @param Value             The value to set. The value must be a \b double.
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_SetParameterDouble(char* Path, double value, void*handle);


/**
 * @brief Get the value of a parameter for the specific SciCompiler Memory Mapped Component or
 * route the value to external board library to set board specific parameters.
 * This function return a \b string as value. Please note that the behaviour of this function
 * is different if the path is a SciSDK managed node or a device specific node managed by the external
 * library. 
 * The SciSDK managed nodes perform a conversion of the string to the correct type and then set the value
 * automatically.
 * If the operation is routed to the external library (ie: ../felib/..) SciSDK has not the information 
 * reguarding the data type required by the external library indeed the che correct GetParamter function 
 * must be call in agreement with the external library required value type.
 * 
 * 
 * @param Path              The path to the parameter to get. The path must be in the format 
 *                          <device name>:.../<resource name>.<parameter name>
 *                          \n
 *                          for example: /board0/MMCComponents/Oscilloscope_0.trigger_mode
 *                          
 * @param Value             Pointer to the allocated buffer to receive the value. The value will be a \b string.
 *                          The buffer must be large enough to contain the value.
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_GetParameterString(char* Path, char** value, void* handle);


/**
 * @brief Get the value of a parameter for the specific SciCompiler Memory Mapped Component or
 * route the value to external board library to set board specific parameters.
 * This function return a \b integer as value. Please note that the behaviour of this function
 * is different if the path is a SciSDK managed node or a device specific node managed by the external
 * library. 
 * The SciSDK managed nodes perform a conversion of the string to the correct type and then set the value
 * automatically.
 * If the operation is routed to the external library (ie: ../felib/..) SciSDK has not the information 
 * reguarding the data type required by the external library indeed the che correct GetParamter function 
 * must be call in agreement with the external library required value type.
 * 
 * 
 * @param Path              The path to the parameter to get. The path must be in the format 
 *                          <device name>:.../<resource name>.<parameter name>
 *                          \n
 *                          for example: /board0/MMCComponents/Oscilloscope_0.trigger_value
 *                          
 * @param Value             Pointer to the allocated buffer to receive the value. The value will be a \b integer.
 *                          The buffer must be large enough to contain the value.
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_GetParameterInteger(char* Path, int *value, void*handle);


/**
 * @brief Get the value of a parameter for the specific SciCompiler Memory Mapped Component or
 * route the value to external board library to set board specific parameters.
 * This function return a \b integer as value. Please note that the behaviour of this function
 * is different if the path is a SciSDK managed node or a device specific node managed by the external
 * library.
 * The SciSDK managed nodes perform a conversion of the string to the correct type and then set the value
 * automatically.
 * If the operation is routed to the external library (ie: ../felib/..) SciSDK has not the information
 * reguarding the data type required by the external library indeed the che correct GetParamter function
 * must be call in agreement with the external library required value type.
 *
 *
 * @param Path              The path to the parameter to get. The path must be in the format
 *                          <device name>:.../<resource name>.<parameter name>
 *                          \n
 *                          for example: /board0/MMCComponents/Oscilloscope_0.trigger_value
 *
 * @param Value             Pointer to the allocated buffer to receive the value. The value will be a \b uint32.
 *                          The buffer must be large enough to contain the value.
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h
 */
SCISDK_DLL_API int SCISDK_GetParameterUInteger(char* Path, uint32_t* value, void* handle);
/**
 * @brief Get the value of a parameter for the specific SciCompiler Memory Mapped Component or
 * route the value to external board library to set board specific parameters.
 * This function return a \b double as value. Please note that the behaviour of this function
 * is different if the path is a SciSDK managed node or a device specific node managed by the external
 * library. 
 * The SciSDK managed nodes perform a conversion of the string to the correct type and then set the value
 * automatically.
 * If the operation is routed to the external library (ie: ../felib/..) SciSDK has not the information 
 * reguarding the data type required by the external library indeed the che correct GetParamter function 
 * must be call in agreement with the external library required value type.
 * 
 * 
 * @param Path              The path to the parameter to get. The path must be in the format 
 *                          <device name>:.../<resource name>.<parameter name>
 *                          \n
 *                          for example: /board0/MMCComponents/Oscilloscope_0.trigger_mode
 *                          
 * @param Value             Pointer to the allocated buffer to receive the value. The value will be a \b double.
 *                          The buffer must be large enough to contain the value.
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_GetParameterDouble(char* Path, double*value, void*handle);

SCISDK_DLL_API int SCISDK_DecodeData(char* Path, void*buffer_in, void*buffer_out, void*handle);

/**
 * @brief Allocate a buffer to be used to store data from the board for the specific 
 * Memory Mapped Componet (oscilloscope, spectrum, list) or of the board specific endpoint
 * (scope, dpp).
 * The buffer is allocated in function of the actual configuration of MMC/board endpoint.
 * For example, if a 1024 point oscilloscope is configured the buffer will be allocated to
 * store 1024 points (1 analog point + 4 digital point per sample).
 * If the device driver to the MMC/board endpoint support both RAW and DECODED data, it is possible
 * to specify the type of data to allocate the buffer for.
 * 
 * The buffer parameter is a pointer to a data structure  pointer. The function will allocate the structure
 * in the buffer and all array inside the structure.
 * 
 * To release the memory allocated by this function the user must call SCISDK_FreeBuffer(). Do not use free
 * to release the memory allocated by this function.
 * 
 * @param Path              The path to the parameter to get. The path must be in the format 
 *                          <device name>:.../<resource name>
 *                          \n
 *                          for example: /board0/MMCComponents/Oscilloscope_0
 * @param buffer_type       T_BUFFER_TYPE_RAW or T_BUFFER_TYPE_DECODED
 * @param buffer            Pointer to a non allocated struct (ie: SCISDK_OSCILLOSCOPE_DECODED_BUFFER) to be allocated
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_AllocateBuffer(char* Path, T_BUFFER_TYPE buffer_type, void **buffer, void *handle);


/**
 * @brief Allocate a buffer to be used to store data from the board for the specific 
 * Memory Mapped Componet (list) or of the board specific endpoint
 * (scope, dpp).
 * The MMC operating in list mode does not have a maximum number of points to store, indeed an additional
 * parameter (size) is required to allocated a buffer to store the data.
 * The user will than use this buffer as temporany buffer for the readout and will store received data in
 * a processing buffer or a file.
 * 
 * The buffer parameter is a pointer to a data structure pointer. The function will allocate the structure
 * in the buffer and all array inside the structure.
 * 
 * To release the memory allocated by this function the user must call SCISDK_FreeBuffer(). Do not use free
 * to release the memory allocated by this function.
 * 
 * @param Path              The path to the parameter to get. The path must be in the format 
 *                          <device name>:.../<resource name>
 *                          \n
 *                          for example: /board0/MMCComponents/List_0
 * @param buffer_type       T_BUFFER_TYPE_RAW or T_BUFFER_TYPE_DECODED
 * @param buffer            Pointer to a non allocated struct (ie: SCISDK_LIST_RAW_BUFFER) to be allocated
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @param size              Size of the buffer to allocate. The size is specified in elements 
 *                          (ie: 1024 for a 1024 list word, where the list word can be 32/64/128.. bits wide, depending
 *                          on the MMC configuration specified in SciCompiler when the list has been created)
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_AllocateBufferSize(char* Path, T_BUFFER_TYPE buffer_type, void **buffer, void *handle, int size);

/**
 * @brief Set the value of a register in the SciCompiler generated firmware.
 * This function works only on Simple Register, not on Register File and Multichannel Register.
 * The last two types of register are supported as Memory Mapped Component and they are set/get with Set/GetParameter
 * 
 * @param Path              The path to the register
 *                          <device name>:.../Registers/<register name>
 *                          \n
 *                          for example: board0:/Registers/DELAY
 * @param value             The value to set in the register
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_SetRegister(char* Path, uint32_t value, void* handle);

/**
 * @brief Get the value of a register in the SciCompiler generated firmware.
 * This function works only on Simple Register, not on Register File and Multichannel Register.
 * The last two types of register are supported as Memory Mapped Component and they are set/get with Set/GetParameter
 * 
 * @param Path              The path to the register
 *                          <device name>:.../Registers/<register name>
 *                          \n
 *                          for example: board0:/Registers/DELAY
 * @param value             Poiter to the variable to receive the value of the register
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_GetRegister(char* Path, uint32_t*value, void*handle);

/**
 * @brief Release the memory allocated by SCISDK_AllocateBuffer() or SCISDK_AllocateBufferSize()
 * 
 * @param Path              The path to the Memory Mapped component or board endpoint 
 *                          The path must be in the format 
 *                          <device name>:.../<resource name>
 *                          \n
 *                          for example: /board0/MMCComponents/List_0
 * 
 * @param buffer_type       T_BUFFER_TYPE_RAW or T_BUFFER_TYPE_DECODED
 * @param buffer            Pointer to the buffer to be released
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_FreeBuffer(char* Path, int buffer_type, void **buffer, void*handle);

/**
 * @brief Read data from the board for the specific Memory Mapped Componet (oscilloscope, spectrum, list) 
 * or of the board specific endpoint (scope, dpp).
 * The function behavour depends on the configuration of the device driver of the Memory Mapped Component 
 * or board endpoint:
 * + if the device driver is configured to work in RAW mode, the function will read the RAW data from 
 * the board and store it in the buffer. 
 * + If the device driver is configured to work in
 * DECODED mode, the function will read the DECODED data from the board and store it in the buffer.
 * + If the divice driver is configured to operated in block mode, the function will wait all the data from the board
 * to be available before returning (up to the specifed timeout).
 * + If the device driver is configured to operate in non blocking mode, the function will return immediately with
 * the data available in the MMC/endpoint buffers or return NI_NO_DATA_AVAILABLE.
 * + If the device driver is configured to operate in thread mode, the function will not directly read the data from
 * the board, but will return the data available in device driver internal buffers. A thread inside the driver
 * periodically reads the data from the board and stores it in the internal buffers.
 * 
 * @param Path              The path to the Memory Mapped component or board endpoint
 *                          The path must be in the format 
 *                          <device name>:.../<resource name>
 *                          \n
 *                          for example: /board0/MMCComponents/List_0
 * 
 * @param buffer            Pointer to the buffer to be filled with the data read from the board 
 *                          allocated with SCISDK_AllocateBuffer()
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_ReadData(char *Path, void *buffer, void*handle);

/**
 * @brief Query the board for the specific Memory Mapped Componet (oscilloscope, spectrum, list)
 * status using the MMC device driver
 * The status is a structure, different for each MMC device driver, that contains the actual status 
 * of the MMC, (ie: running, stopped, run time, progress, number of samples in the spectrum, etc..)
 * 
 * @param Path              The path to the Memory Mapped component or board endpoint
 * @param buffer            Pointer to the buffer to be filled with the status. The buffer must be allocated
 *                          and must be in the correct format for the specific MMC device driver.
 *                          For example the status buffer for the Spectrum device driver must be
 *                          instantiated as SCISDK_SPECTRUM_STATUS_BUFFER   
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_ReadStatus(char *Path, void *buffer, void*handle);

/**
 * @brief Convert the returned error code in a human readable string explaining the error
 * 
 * @param err_no            The error code returned by the SciSDK functions
 * @param value             Pointer to the buffer to be filled with the error string. It's allocated by the 
 *                          SCISDK_s_error function and must be freed by the caller using the 
 *                          SCISDK_free_string() function
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  The same error code of the parameter err_no  
 */
SCISDK_DLL_API int SCISDK_s_error(int err_no, char** value, void* handle);

/**
 * @brief Execute a command for the specific Memory Mapped Component device driver/endpoint. 
 * Typically the device drivers of the MMC do not directly support access to the hardware when a single
 * Set/Get Paramter is called but wait for a configuration command to be executed.
 * Refere to the specific Device Driver documentation for the list of supported commands and their usage.
 * 
 * 
 * @param Path              The path to the Memory Mapped component or board endpoint command
 *                          <device name>:.../<resource name>.<command name>
 *                          \n
 *                          for example: /board0/MMCComponents/TOF_0.start
 * @param value             An optional argument for the command. It's type depends on the command
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h   
 */
SCISDK_DLL_API int SCISDK_ExecuteCommand(char* Path, char* value, void* handle);

/**
 * @brief Return the tree of:
 * + Memory Mapped Components
 * + registers
 * + board endpoints
 * + board device driver configuration parameters 
 * for the specified board
 * 
 * Please note that is not possible to get CaenFELib internal configuration parameters using this function.
 * (will appera just /felib/** indicating that this routing point will pass the rest of the path to
 * CaenFELib)
 * 
 * @param name              The name of the board 
 * @param Type              The type of resource to be returned. It can be:
 *                          + registers
 *                          + oscilloscope
 *                          + wavedump
 *                          + listmodule
 *                          + fftmonitor
 *                          + spectrum
 *                          + custompacket
 *                          + mcratemeter
 *                          + citiroccfg
 *                          + petiroccfg
 *                          + registerfile
 *                          + oscilloscopedual
 *                          + tofspectrum
 *                          + frametransfer
 *                          + citirocframe
 *                          + petirocframe    
 * 
 *                          Left empty ("") to get all resources
 * @param ret               Pointer to the buffer to be filled with the tree. It's allocated by the
 *                          SCISDK_GetParameterListOfValues function and must be freed by the caller using the
 *                          SCISDK_free_string() function
 * @param return_json       If true the tree is returned in JSON format, otherwise in CSV format
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_GetComponentList(char* name, char* Type, char** ret, bool return_json, void* handle);

/**
 * @brief Return all configuration parameters for the specified node MMC/endpoint
 * 
 * @param Path              The path to the node MMC/endpoint
 *                          <device name>:.../<resource name>
 *                          \n
 *                          for example: /board0/MMCComponents/Oscilloscope_0
 * @param ret               Pointer to the buffer to be filled with the tree. It's allocated by the
 *                          SCISDK_GetParameterListOfValues function and must be freed by the caller using the
 *                          SCISDK_free_string() function
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_GetAllParameters(char* Path, char **ret, void*handle);

/**
 * @brief Return the description for a specific parameter
 * 
 * @param Path              The path to the paramter
 *                          <device name>:.../<resource name>.<parameter name>
 *                          \n
 *                          for example: /board0/MMCComponents/Oscilloscope_0.trigger_mode
 * @param ret               Pointer to the buffer to be filled with the description. It's allocated by the
 *                          SCISDK_GetParameterListOfValues function and must be freed by the caller using the
 *                          SCISDK_free_string() function
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_GetParameterDescription(char* Path, char** ret, void* handle);

/**
 * @brief If the parameter has a list of possible values, return the list of possible values
 * otherwise return an empty string
 * 
 * @param Path              The path to the paramter
 *                          <device name>:.../<resource name>.<parameter name>
 *                          \n
 *                          for example: /board0/MMCComponents/Oscilloscope_0.trigger_mode
 * @param ret               Pointer to the buffer to be filled with the list of possible values. It's allocated by the
 *                          SCISDK_GetParameterListOfValues function and must be freed by the caller using the
 *                          SCISDK_free_string() function
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_GetParameterListOfValues(char* Path, char** ret, void* handle);

/**
 * @brief If the parameter is numeric, return the minimum acceptable value
 * 
 * @param Path              The path to the paramter
 *                          <device name>:.../<resource name>.<parameter name>
 *                          \n
 *                          for example: /board0/MMCComponents/Oscilloscope_0.trigger_level
 * @param re                Pointer to the double-type variable to be filled with the minimum value.
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_GetParameterMinimumValue(char* Path, double* re, void* handle);

/**
 * @brief If the parameter is numeric, return the maximum acceptable value
 * 
 * @param Path              The path to the paramter
 *                          <device name>:.../<resource name>.<parameter name>
 *                          \n
 *                          for example: /board0/MMCComponents/Oscilloscope_0.trigger_level
 * @param re                Pointer to the double-type variable to be filled with the maximum value.
 * @param handle            Handle to the SciSDK allocated by SCISDK_InitLib()
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_GetParameterMaximumValue(char* Path, double* ret, void* handle);

/**
 * @brief Return a json with all the information about the parameter
 * 
 * @param Path              The path to the paramter
 *                          <device name>:.../<resource name>.<parameter name>
 *                          \n
 *                          for example: /board0/MMCComponents/Oscilloscope_0.trigger_level
 * @param ret               Pointer to the buffer to be filled with the json. It's allocated by the
 *                          SCISDK_GetParametersProperties function and must be freed by the caller using the
 *                          SCISDK_free_string() function
 * @param handle 
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_GetParametersProperties(char *Path, char **ret, void *handle);

/**
 * @brief Free a SciSDK allocated string
 * 
 * @param _str              Pointer to the string to be freed
 * @return                  int error code defined in NIErrorCode.h  
 */
SCISDK_DLL_API int SCISDK_free_string(char * _str);

/**
* @brief Return a csv string with the list of attached devices
* 
* @param ret                Pointer to the buffer to be filled with the csv string. It's allocated by the 
*                           SCISDK_GetAttachedDevicesList function and must be freed by the caller using the
*                           SCISDK_free_string() function
* @param handle 
* @return                   int error code defined in NIErrorCode.h
*/
SCISDK_DLL_API int SCISDK_GetAttachedDevicesList(char** ret, void* handle);

/**
* @brief Return a string with the current version of SciSDK library
* 
* @param ret                Pointer to the buffer to be filled with the version number string. It's allocated by the 
*                           SCISDK_GetLibraryVersion function and must be freed by the caller using the
*                           SCISDK_free_string() function
* 
* @param handle
* @return                   int error code defined in NIErrorCode.h
*/
 SCISDK_DLL_API int SCISDK_GetLibraryVersion(char** ret, void* handle);