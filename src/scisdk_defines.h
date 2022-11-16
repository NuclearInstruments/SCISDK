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
		uint32_t magic;
		int32_t *analog;
		uint8_t *digital;
		uint32_t trigger_position;
		uint64_t timecode;
		struct {
			uint32_t samples_analog;
			uint32_t samples_digital;
			uint32_t tracks_analog_per_channel;
			uint32_t tracks_digital_per_channel;
			uint32_t channels;
		} info;
	}SCISDK_OSCILLOSCOPE_DUAL_DECODED_BUFFER;

	typedef struct {
		uint32_t magic;
		uint32_t *data;
		uint32_t zero_position;
		uint32_t trigger_position;
		uint64_t timecode;
		struct {
			uint32_t buffer_size;
			uint32_t samples_analog;
			uint32_t samples_digital;
			uint32_t tracks_analog_per_channel;
			uint32_t tracks_digital_per_channel;
			uint32_t channels;
		} info;
	}SCISDK_OSCILLOSCOPE_DUAL_RAW_BUFFER;


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

	typedef struct {
		uint32_t *row;
		uint32_t n;
	}SCISDK_CP_PACKET;

	typedef struct {
		uint32_t magic;
		SCISDK_CP_PACKET *data;
		struct {
			uint32_t buffer_size;
			uint32_t packet_size;
			uint32_t valid_data;
		} info;
	}SCISDK_CP_DECODED_BUFFER;

	typedef struct {
		uint32_t magic;
		uint32_t *data;
		struct {
			uint32_t buffer_size;
			uint32_t valid_data;
		} info;
	}SCISDK_CP_RAW_BUFFER;

	typedef struct {
		uint32_t magic;
		double *data;
		struct {
			uint32_t buffer_size;
			uint32_t valid_data;
			uint32_t nchannels;
		}info;
	}SCISDK_RM_RAW_BUFFER;

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

	typedef struct {
		bool running;
		bool completed;
		uint32_t progress;
		uint32_t peak_max;				//future develop
		uint32_t total_counter;			//future develop
		double integration_time;		//future develop
	}SCISDK_SPECTRUM_STATUS;


	typedef struct {
		uint32_t magic;
		double *mag;
		double *ph;
		uint64_t timecode;
		struct {
			uint32_t samples;
			uint32_t channels;
		} info;
	}SCISDK_FFT_DECODED_BUFFER;

	typedef struct {
		uint32_t magic;
		uint32_t *data;
		uint64_t timecode;
		struct {
			uint32_t buffer_size;
			uint32_t samples;
			uint32_t channels;
		} info;
	}SCISDK_FFT_RAW_BUFFER;

	typedef struct {
		bool armed;
		bool ready;
		bool running;
	}SCISDK_FFT_STATUS;



	typedef struct {
		uint32_t *pixel;
		uint32_t n;
		struct {
			uint64_t timestamp;
			uint64_t trigger_count;
			uint64_t event_count;
			uint64_t hits;
		};
	}SCISDK_FRAME_PACKET;

	typedef struct {
		uint32_t magic;
		SCISDK_FRAME_PACKET *data;
		struct {
			uint32_t buffer_size;
			uint32_t valid_data;
		} info;
	}SCISDK_FRAME_DECODED_BUFFER;

	typedef struct {
		uint32_t magic;
		uint32_t *data;
		struct {
			uint32_t buffer_size;
			uint32_t packet_size;
			uint32_t valid_data;
		} info;
	}SCISDK_FRAME_RAW_BUFFER;

	typedef struct {
		uint16_t hg;
		uint16_t lg;
		uint8_t  hit;
	} SCISDK_CITIROC_PIXEL;
	typedef struct {
		SCISDK_CITIROC_PIXEL *pixel;
		uint32_t n;
		struct {
			uint8_t asic;
			uint64_t timestamp_from_t0;
			uint64_t timestamp_from_run;
			uint32_t event_id;
			uint32_t trigger_count;
			uint32_t validation_counter;
			uint32_t flags;
		}info;
	}SCISDK_CITIROC_PACKET;

	typedef struct {
		uint32_t magic;
		SCISDK_CITIROC_PACKET *data;
		struct {
			uint32_t buffer_size;
			uint32_t valid_data;
		} info;
	}SCISDK_CITIROC_DECODED_BUFFER;

	typedef struct {
		uint32_t magic;
		uint32_t *data;
		struct {
			uint32_t buffer_size;
			uint32_t packet_size;
			uint32_t valid_data;
		} info;
	}SCISDK_CITIROC_RAW_BUFFER;


	typedef struct {
		uint16_t charge;
		uint16_t fine;
		uint16_t coarse;
		uint8_t  hit;
	} SCISDK_PETIROC_PIXEL;
	typedef struct {
		SCISDK_PETIROC_PIXEL *pixel;
		uint32_t n;
		struct {
			uint8_t asic;
			uint64_t timestamp_from_t0;
			uint64_t timestamp_from_run;
			uint32_t event_id;
			uint32_t trigger_count;
			uint32_t validation_counter;
			uint32_t flags;
		}info;
	}SCISDK_PETIROC_PACKET;

	typedef struct {
		uint32_t magic;
		SCISDK_PETIROC_PACKET *data;
		struct {
			uint32_t buffer_size;
			uint32_t valid_data;
		} info;
	}SCISDK_PETIROC_DECODED_BUFFER;

	typedef struct {
		uint32_t magic;
		uint32_t *data;
		struct {
			uint32_t buffer_size;
			uint32_t packet_size;
			uint32_t valid_data;
		} info;
	}SCISDK_PETIROC_RAW_BUFFER;	



	typedef struct  {
		uint64_t timestamp;
		uint32_t trigger_id;
		size_t event_size;
		uint16_t** waveform;
		size_t* n_samples;
		size_t* n_allocated_samples;
		unsigned n_channels;
	} SCISDK_FE_SCOPE_EVENT;
#endif
