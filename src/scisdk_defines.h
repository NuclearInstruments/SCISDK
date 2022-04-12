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


	typedef struct {
		uint32_t magic;
		int32_t *analog;
		uint64_t hits;
		uint64_t timecode;
		uint32_t counter;
		uint32_t user;
		struct {
			uint32_t samples;
			uint32_t valid_samples;
			uint32_t channels;
		} info;
	}SCISDK_DIGITIZER_DECODED_BUFFER;
	
	typedef struct {
		uint32_t magic;
		int32_t *data;
		struct {
			uint32_t buffer_size;
			uint32_t samples;
			uint32_t valid_samples;
			uint32_t channels;
		} info;
	}SCISDK_DIGITIZER_RAW_BUFFER;



	typedef struct {
		uint32_t magic;
		char *data;
		struct {
			uint32_t buffer_size;
			uint32_t samples;
			uint32_t valid_samples;
			uint32_t channels;
		} info;
	}SCISDK_LIST_RAW_BUFFER;

	typedef enum {
		T_BUFFER_TYPE_RAW,
		T_BUFFER_TYPE_DECODED
	} T_BUFFER_TYPE;


	typedef struct {
		uint32_t magic;
		uint32_t *data;
		uint64_t timecode;
		uint32_t inttime;
		struct {
			uint32_t buffer_size;
			uint32_t total_bins;
			uint32_t valid_bins;
		} info;
	}SCISDK_SPECTRUM_DECODED_BUFFER;

#endif
