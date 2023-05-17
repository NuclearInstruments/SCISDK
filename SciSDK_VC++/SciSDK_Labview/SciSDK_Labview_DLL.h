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

#include "./Labview_cintools/lv_prolog.h"

SCISDKLABVIEW_DLL_API void* last_handle = 0;

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
	int32_t Numeric[1];
} OneDIntArray;
typedef OneDIntArray** OneDIntArrayHdl;

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

typedef struct {
	int32_t dimSizes[2];
	unsigned long Numeric[1];
} TwoDUnsignedLongArray;
typedef TwoDUnsignedLongArray** TwoDUnsignedLongArrayHdl;

typedef struct {
	int32_t dimSize;
	double Numeric[1];
} OneDDoubleArray;
typedef OneDDoubleArray** OneDDoubleArrayHdl;

typedef struct {
	int32_t dimSize;
	int8_t Numeric[1];
} OneDCharArray;
typedef OneDCharArray** OneDCharArrayHdl;

typedef struct {
	int32_t dimSize;
	char* array[1];
}OneDStringArray;
typedef OneDStringArray** OneDStringArrayHdl;

/* LabView Buffers typedefs */
typedef struct {
	uint32_t magic;
	uint64_t trigger_position;
	uint64_t timecode;
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
	uint32_t timecode;
	uint32_t inttime;
	uint32_t buffer_size;
	uint32_t total_bins;
	uint32_t valid_bins;
	OneDUnsignedLongArrayHdl data;
} TD_SPECTRUM;

typedef struct {
	TwoDLongArrayHdl analog;
	uint32_t magic;
	uint32_t hits;
	uint32_t timecode;
	uint32_t counter;
	uint32_t user;
	uint32_t samples;
	uint32_t valid_samples;
	uint32_t channels;
	uint32_t enabled_channels;
} TD_DIGITIZER;

typedef struct {
	uint32_t magic;
	uint64_t timecode;
	uint32_t samples;
	uint32_t channels;
	OneDDoubleArrayHdl mag;
	OneDDoubleArrayHdl ph;
} TD_FFT;

typedef struct {
	uint32_t magic;
	uint32_t buffer_size;
	uint32_t samples;
	uint32_t valid_samples;
	uint32_t channels;
	OneDCharArrayHdl data;
} TD_LIST;

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
} TD_OSCILLOSCOPE_DUAL;

typedef struct {
	uint32_t magic;
	uint32_t buffer_size;
	uint32_t valid_data;
	uint32_t nchannels;
	OneDDoubleArrayHdl data;
} TD_RATEMETER;

typedef struct {
	uint32_t magic;
	uint32_t buffer_size;
	uint32_t packet_size;
	uint32_t valid_data;
	TwoDUnsignedLongArrayHdl data;
} TD_CUSTOMPACKETMULTIPLE;

typedef struct {
	uint32_t magic;
	uint32_t buffer_size;
	uint32_t packet_size;
	uint32_t valid_data;
	OneDUnsignedLongArrayHdl data;
}TD_CUSTOMPACKETSINGLE;

typedef struct {
	uint64_t n;
	uint64_t timestamp;
	uint64_t trigger_count;
	uint64_t event_count;
	uint64_t hits;
	OneDUnsignedLongArrayHdl pixel;
}FRAME_PACKET;

typedef struct {
	int32_t dimSize;
	FRAME_PACKET packets[1];
}FrameArray;
typedef FrameArray** FrameArrayHdl;

typedef struct {
	uint32_t magic;
	uint32_t buffer_size;
	uint32_t valid_data;
	FrameArrayHdl data;
} TD_FRAME_MULTIPLE;

typedef struct {
	uint32_t magic;
	uint32_t buffer_size;
	uint64_t n;
	uint64_t timestamp;
	uint64_t trigger_count;
	uint64_t event_count;
	uint64_t hits;
	OneDUnsignedLongArrayHdl pixel;
}TD_FRAME_SINGLE;

typedef struct {
	bool armed;
	bool ready;
	bool running;
} TD_OSCILLOSCOPE_STATUS;

typedef struct {
	bool running;
	bool completed;
	uint32_t progress;
	uint32_t peak_max;
	uint32_t total_counter;
	double integration_time;
} TD_SPECTRUM_STATUS;

typedef struct {
	bool armed;
	bool ready;
	bool running;
} TD_FFT_STATUS;

typedef struct {
	char* name;
	char* path;
	char* type;
} TD_COMPONENT;
typedef TD_COMPONENT;

typedef struct {
	int32_t dimSize;
	TD_COMPONENT array[1];
} TD_COMPONENTARRAY;

#include "./Labview_cintools/lv_epilog.h"

SCISDKLABVIEW_DLL_API void* LV_SCISDK_InitLib();

SCISDKLABVIEW_DLL_API int LV_SCISDK_FreeLib(void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_AddNewDevice(char* DevicePath, char* DeviceModel, char* JSONFwFilePath, char* Name, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_DetachDevice(char* name, void* handle);

// parameters
SCISDKLABVIEW_DLL_API int LV_SCISDK_SetParameterString(char* Path, char* value, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_SetParameterInteger(char* Path, int value, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_SetParameterUInteger(char* Path, unsigned int value, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_SetParameterDouble(char* Path, double value, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterString(char* Path, char* value, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterInteger(char* Path, int* value, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterUInteger(char* Path, unsigned int* value, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterDouble(char* Path, double* value, void* handle);

// read
SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadOscilloscope(char* Path, TD_OSCILLOSCOPE* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadSpectrum(char* Path, TD_SPECTRUM* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadTOFSpectrum(char* Path, TD_SPECTRUM* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadDigitizer(char* Path, TD_DIGITIZER* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadFFT(char* Path, TD_FFT* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadList(char* Path, TD_LIST* buffer, void* handle, int buffer_size);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadOscilloscopeDual(char* Path, TD_OSCILLOSCOPE_DUAL* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadRatemeter(char* Path, TD_RATEMETER* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadCustomPacketMultiple(char* Path, TD_CUSTOMPACKETMULTIPLE* buffer, void* handle, int buffer_size);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadCustomPacketSingle(char* Path, TD_CUSTOMPACKETSINGLE* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadFrameSingle(char* Path, TD_FRAME_SINGLE* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadFrameMultiple(char* Path, TD_FRAME_MULTIPLE* buffer, void* handle, int buffer_size);

// registers
SCISDKLABVIEW_DLL_API int LV_SCISDK_SetRegister(char* Path, uint32_t value, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetRegister(char* Path, uint32_t* value, void* handle);

// execute command
SCISDKLABVIEW_DLL_API int LV_SCISDK_ExecuteCommand(char* Path, char* parameter, void* handle);

// read status
SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadOscilloscopeStatus(char* Path, TD_OSCILLOSCOPE_STATUS* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadSpectrumStatus(char* Path, TD_SPECTRUM_STATUS* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_ReadFFTStatus(char* Path, TD_FFT_STATUS* buffer, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetComponentList(char* name, char* type, TD_COMPONENTARRAY ret, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetAllParameters(char* path, char* ret, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterDescription(char* path, char* ret, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterListOfValues(char* path, char* ret, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterMinimumValue(char* path, double* ret, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParameterMaximumValue(char* path, double* ret, void* handle);

SCISDKLABVIEW_DLL_API int LV_SCISDK_GetParametersProperties(char* path, char* ret, void* handle);

// s_error
SCISDKLABVIEW_DLL_API int LV_SCISDK_GetErrorDescription(int error, char* ret, void* handle);
