#ifndef HEADER_H_SCISDK_DEFINES
#define HEADER_H_SCISDK_DEFINES
#include <stdint.h>

	typedef struct {
		uint32_t magic;
		int32_t *analog;
		uint8_t *digital;
		uint32_t trigger_position;
		uint64_t timecode;
		struct  {
			uint32_t samples_analog;
			uint32_t samples_digital;
			uint32_t tracks_analog_per_channel;
			uint32_t tracks_digital_per_channel;
			uint32_t channels;
		} info;
	}SCISDK_OSCILLOSCOPE_DECODED_BUFFER;

	typedef struct {
		uint32_t magic;
		uint32_t *data;
		uint32_t zero_position;
		uint32_t trigger_position;
		uint64_t timecode;
		struct  {
			uint32_t buffer_size;
			uint32_t samples_analog;
			uint32_t samples_digital;
			uint32_t tracks_analog_per_channel;
			uint32_t tracks_digital_per_channel;
			uint32_t channels;
		} info;
	}SCISDK_OSCILLOSCOPE_RAW_BUFFER;

	typedef struct {
		bool armed;
		bool ready;
		bool running;
	}SCISDK_OSCILLOSCOPE_STATUS;
	
	typedef enum {
		T_BUFFER_TYPE_RAW,
		T_BUFFER_TYPE_DECODED
	} T_BUFFER_TYPE;
#endif