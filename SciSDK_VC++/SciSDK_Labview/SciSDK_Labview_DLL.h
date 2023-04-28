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

/* LabVIEW created typedefs */
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef struct {
	int32_t dimSize;
	long Numeric[1];
} OneDLongArray;
typedef OneDLongArray** OneDLongArrayHdl;

typedef struct {
	int32_t dimSize;
	unsigned long Numeric[1];
} OneDUnsignedLongArray;
typedef OneDUnsignedLongArray** OneDUnsignedLongArrayHdl;

typedef struct {
	int32_t dimSizes[2];
	long Numeric[1];
} TwoDLongArray;
typedef TwoDLongArray** TwoDLongArrayHdl;


/* LabView Buffers typedefs */
typedef struct {
	uint32_t magic;
	uint32_t trigger_position;
	uint32_t timecode;
	uint32_t samples_analog;
	uint32_t samples_digital;
	uint32_t tracks_analog_per_channel;
	uint32_t tracks_digital_per_channel;
	uint32_t channels;
	TwoDLongArrayHdl analog;
	TwoDLongArrayHdl digital_data;
} TD_OSCILLOSCOPE;

typedef struct {
	uint32_t magic;
	OneDUnsignedLongArrayHdl data;
	uint32_t timecode;
	uint32_t inttime;
	uint32_t buffer_size;
	uint32_t total_bins;
	uint32_t valid_bins;
} TD_SPECTRUM;

typedef struct {

} TD_BUFFER;

typedef struct {

} TD_DIGITIZER;

typedef struct {

} TD_FFT;

typedef struct {

} TD_LIST;

typedef struct {

} TD_OSCILLOSCOPE_DUAL;

typedef struct {

} TD_RATEMETER;

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

// read functions
SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadOscilloscope(char* Path, TD_OSCILLOSCOPE* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadSpectrum(char* Path, TD_SPECTRUM* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadBuffer(char* Path, TD_BUFFER* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadDigitizer(char* Path, TD_DIGITIZER* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadFFT(char* Path, TD_FFT* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadList(char* Path, TD_LIST* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadOscilloscopeDual(char* Path, TD_OSCILLOSCOPE_DUAL* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadRatemeter(char* Path, TD_RATEMETER* buffer, void* handle);
#pragma pack(pop)