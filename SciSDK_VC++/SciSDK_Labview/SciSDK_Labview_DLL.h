#pragma pack(push) 
#pragma pack(1)

#ifdef _MSC_VER
	#ifdef SCISDKLABVIEW_EXPORTS
		#define SCISDKLABVIEW_DLL_API extern "C" __declspec(dllexport)
	#else
		#define SCISDKLABVIEW_DLL_API extern "C" __declspec(dllimport)
	#endif
#else	
	#ifdef SCISDKLABVIEW_EXPORTS
		#define SCISDKLABVIEW_DLL_API __attribute__((visibility("default")))
	#else
		#define SCISDKLABVIEW_DLL_API
	#endif
#endif

SCISDKLABVIEW_DLL_API void* LV_SCISDK_InitLib();

SCISDKLABVIEW_DLL_API int LV_SCISDK_FreeLib(void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_AddNewDevice(char* DevicePath, char* DeviceModel, char* JSONFwFilePath, char* Name, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_DetachDevice(char* name, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_SetParameterString(char* Path, char* value, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_SetParameterInteger(char* Path, int value, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_SetParameterUInteger(char* Path, unsigned int value, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_SetParameterDouble(char* Path, double value, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterString(char* Path, char* value, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterInteger(char* Path, int* value, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterUInteger(char* Path, unsigned int* value, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterDouble(char* Path, double* value, void* handle);

//typedef struct {
//	unsigned int magic;									/**< Magic number to identify the data type*/
//	//int* analog;								/**< Pointer to analog data allocated by the AllocateBuffer function*/
//	//signed char* digital;								/**< Pointer to digital data allocated by the AllocateBuffer function*/
//	unsigned int trigger_position;						/**< Trigger position in samples*/
//	unsigned long long timecode;
//	//LV_OSCILLOSCOPE_DECODED_BUFFER_INFO info;
//	// info section
//	//unsigned int samples_analog;					/**< Number of samples in analog buffer*/
//	//unsigned int samples_digital;					/**< Number of samples in digital buffer*/
//	//unsigned int tracks_analog_per_channel;			/**< Number of analog tracks per channel*/
//	//unsigned int tracks_digital_per_channel;		/**< Number of digital tracks per channel*/
//	//unsigned int channels;
//}LV_OSCILLOSCOPE_DECODED_BUFFER;

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;


/* LabVIEW created typdefs */
typedef struct {
	int32_t dimSize;
	long Numeric[1];
} TD3;
typedef TD3** TD3Hdl;

typedef struct {
	uint32_t magic;
	uint32_t trigger_position;
	uint32_t timecode;
	uint32_t samples_analog;
	uint32_t samples_digital;
	uint32_t tracks_analog_per_channel;
	uint32_t tracks_digital_per_channel;
	uint32_t channels;
	TD3Hdl analog;
	TD3Hdl digital_data;
} TD_OSCILLOSCOPE;




SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadOscilloscope(char* Path, TD_OSCILLOSCOPE* buffer, void* handle);
#pragma pack(pop)