#pragma pack(1)

#ifdef _MSC_VER
#ifdef SCISDKLABVIEW_EXPORTS
#define SCISDKLABVIEW_DLL_API extern "C" __declspec(dllexport)// __declspec(dllexport)
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

//#include "extcode.h"
//#include "lv_prolog.h"

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
//
//
//
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
//	
//}LV_OSCILLOSCOPE_DECODED_BUFFER;
//
//typedef struct {
//	long dimSize;
//	long Numeric[1];
//}Analog;
//typedef Analog** analoghdl;
//
//typedef struct {
//	long dimSize;
//	int Numeric[1];
//} TD3;
//typedef TD3** TD3Hdl;
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;


typedef struct {
	uint32_t samples_analog;					/**< Number of samples in analog buffer*/
	uint32_t samples_digital;					/**< Number of samples in digital buffer*/
	uint32_t tracks_analog_per_channel;			/**< Number of analog tracks per channel*/
	uint32_t tracks_digital_per_channel;		/**< Number of digital tracks per channel*/
	uint32_t channels;
							/**< Number of channels*/
} LV_OSCILLOSCOPE_DECODED_BUFFER_INFO;

typedef struct {
	uint32_t magic;									/**< Magic number to identify the data type*/
	int32_t* analog;								/**< Pointer to analog data allocated by the AllocateBuffer function*/
	uint8_t* digital;								/**< Pointer to digital data allocated by the AllocateBuffer function*/
	uint32_t trigger_position;						/**< Trigger position in samples*/
	uint32_t timecode;								/**< Timecode of the first sample. Taken by timecome input in SciCompiler firmware*/
	LV_OSCILLOSCOPE_DECODED_BUFFER_INFO info;
}LV_OSCILLOSCOPE_DECODED_BUFFER;

//typedef struct {
//	unsigned int samples_analog;					/**< Number of samples in analog buffer*/
//	unsigned int samples_digital;					/**< Number of samples in digital buffer*/
//	unsigned int tracks_analog_per_channel;			/**< Number of analog tracks per channel*/
//	unsigned int tracks_digital_per_channel;		/**< Number of digital tracks per channel*/
//	unsigned int channels;							/**< Number of channels*/
//} LV_OSCILLOSCOPE_DECODED_BUFFER_INFO;
//
//typedef struct {
//	unsigned int magic;
//	int* analog;
//	signed char* digital;
//	unsigned int trigger_position;
//	unsigned long timecode;
//	unsigned long placeholder;
//	LV_OSCILLOSCOPE_DECODED_BUFFER_INFO info;
//}LV_OSCILLOSCOPE_DECODED_BUFFER;

SCISDKLABVIEW_DLL_API int LV_SCISDK_AllocateBuffer_Oscilloscope(char* Path, int buffer_type, LV_OSCILLOSCOPE_DECODED_BUFFER* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadData(char* Path, LV_OSCILLOSCOPE_DECODED_BUFFER* buffer, void* handle);
//
//SCISDKLABVIEW_DLL_API int LV_SCISDK_FreeBuffer()