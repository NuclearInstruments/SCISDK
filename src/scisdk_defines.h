#ifndef HEADER_H_SCISDK_DEFINES
#define HEADER_H_SCISDK_DEFINES
#include <stdint.h>

	/**
	 * @brief Oscilloscope decoded data type
	 * @details This type is used to store decoded data from oscilloscope.
	 * The data in this structure is already decoded.
	 * The first sample in the analog/digital buffer rappresent the first sample of the channel 1
	 * 
	 * Example with 3 channels, 1024 samples per channel, pretrigger T
	 * Samples between 0 and T-1 are pre-trigger samples
	 * Samples between T and 1023 are post-trigger samples
	 * 
	 * | Channel 1                      | Channel 2                      | Channel 3                      |
	 * | ------------------------------ | ------------------------------ | ------------------------------ |
	 * | 0  ...  T ............... 1024 | 0  ...  T ............... 1024 | 0  ...  T ............... 1024 |
	 */
	typedef struct {		
		uint32_t magic;									/**< Magic number to identify the data type*/
		int32_t *analog;								/**< Pointer to analog data allocated by the AllocateBuffer function*/
		uint8_t *digital;								/**< Pointer to digital data allocated by the AllocateBuffer function*/
		uint32_t trigger_position;						/**< Trigger position in samples*/
		uint64_t timecode;								/**< Timecode of the first sample. Taken by timecome input in SciCompiler firmware*/		
		struct  {
			uint32_t samples_analog;					/**< Number of samples in analog buffer*/
			uint32_t samples_digital;					/**< Number of samples in digital buffer*/
			uint32_t tracks_analog_per_channel;			/**< Number of analog tracks per channel*/
			uint32_t tracks_digital_per_channel;		/**< Number of digital tracks per channel*/
			uint32_t channels;							/**< Number of channels*/
		} info;
	}SCISDK_OSCILLOSCOPE_DECODED_BUFFER;

	/**
	 * @brief Oscilloscope raw data type
	 * @details This type is used to store raw data from oscilloscope.
	 * The reference document for the raw data format is the  [Oscilloscope driver](oscilloscope.md)
	 */
	typedef struct {
		uint32_t magic;									/**< Magic number to identify the data type*/
		uint32_t *data;									/**< Pointer to data allocated by the AllocateBuffer function*/
		uint32_t zero_position;							/**< Position of the first sample in the buffer (the sample on which the oscilloscope triggered)*/
		uint32_t trigger_position;						/**< Trigger position in samples. It's set equal to the pretrigger*/
		uint64_t timecode;								/**< Timecode of the first sample. Taken by timecome input in SciCompiler firmware*/
		struct  {
			uint32_t buffer_size;						/**< Size of the buffer in samples*/
			uint32_t samples_analog;					/**< Number of analog samples per channel in the buffer*/
			uint32_t samples_digital;					/**< Number of digital samples (4 bit per sample) per channel in the buffer*/
			uint32_t tracks_analog_per_channel;			/**< Number of analog tracks per channel*/
			uint32_t tracks_digital_per_channel;		/**< Number of digital tracks per channel*/
			uint32_t channels;							/**< Number of channels*/
		} info;
	}SCISDK_OSCILLOSCOPE_RAW_BUFFER;

	/**
	 * @brief Oscilloscope decoded data type
	 * @details This type is used to store decoded data from oscilloscope.
	 * The data in this structure is already decoded.
	 * The first sample in the analog/digital buffer rappresent the first sample of the channel 1
	 * 
	 * Example with 3 channels, 1024 samples per channel, pretrigger T
	 * Samples between 0 and T-1 are pre-trigger samples
	 * Samples between T and 1023 are post-trigger samples
	 * 
	 * | Channel 1                      | Channel 2                      | Channel 3                      |
	 * | ------------------------------ | ------------------------------ | ------------------------------ |
	 * | 0  ...  T ............... 1024 | 0  ...  T ............... 1024 | 0  ...  T ............... 1024 |
	 */
	typedef struct {
		uint32_t magic;									/**< Magic number to identify the data type*/
		int32_t *analog;								/**< Pointer to analog data allocated by the AllocateBuffer function*/
		uint8_t *digital;								/**< Pointer to digital data allocated by the AllocateBuffer function*/
		uint32_t trigger_position;						/**< Trigger position in samples*/
		uint64_t timecode;								/**< Timecode of the first sample. Taken by timecome input in SciCompiler firmware*/		
		struct  {
			uint32_t samples_analog;					/**< Number of samples in analog buffer*/
			uint32_t samples_digital;					/**< Number of samples in digital buffer*/
			uint32_t tracks_analog_per_channel;			/**< Number of analog tracks per channel*/
			uint32_t tracks_digital_per_channel;		/**< Number of digital tracks per channel*/
			uint32_t channels;							/**< Number of channels*/
		} info;
	}SCISDK_OSCILLOSCOPE_DUAL_DECODED_BUFFER;

	/**
	 * @brief Oscilloscope raw data type
	 * @details This type is used to store raw data from oscilloscope.
	 * The reference document for the raw data format is the  [Oscilloscope Dual driver](oscilloscope-dual.md)
	 */
	typedef struct {
		uint32_t magic;									/**< Magic number to identify the data type*/
		uint32_t *data;									/**< Pointer to data allocated by the AllocateBuffer function*/
		uint32_t zero_position;							/**< Position of the first sample in the buffer (the sample on which the oscilloscope triggered)*/
		uint32_t trigger_position;						/**< Trigger position in samples. It's set equal to the pretrigger*/
		uint64_t timecode;								/**< Timecode of the first sample. Taken by timecome input in SciCompiler firmware*/
		struct  {
			uint32_t buffer_size;						/**< Size of the buffer in samples*/
			uint32_t samples_analog;					/**< Number of analog samples per channel in the buffer*/
			uint32_t samples_digital;					/**< Number of digital samples (4 bit per sample) per channel in the buffer*/
			uint32_t tracks_analog_per_channel;			/**< Number of analog tracks per channel*/
			uint32_t tracks_digital_per_channel;		/**< Number of digital tracks per channel*/
			uint32_t channels;							/**< Number of channels*/
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
		T_BUFFER_TYPE_RAW = 0,
		T_BUFFER_TYPE_DECODED
	} T_BUFFER_TYPE;

	/**
	 * @brief Spectrum decoded data
	 * @details This type is used to store data from spectrum.
	 * The reference document for the data format is the  [Spectrum](spectrum.md)
	 */
	typedef struct {
		uint32_t magic;					/**< Magic number to identify the data type*/
		uint32_t *data;					/**< Pointer to data allocated by the AllocateBuffer function*/
		uint64_t timecode;				/**< Epoch of the PC related to when the spectrum has been readout*/
		uint32_t inttime;				/**< Integration time in clock cycles (calculated by the FPGA)*/
		struct {
			uint32_t buffer_size;		/**< Size of the data in samples (on sample per bin)*/
			uint32_t total_bins;		/**< Total number of bins*/
			uint32_t valid_bins;		/**< Number of valid bins, depends on rebin factor*/
		} info;
	}SCISDK_SPECTRUM_DECODED_BUFFER;

	/**
	 * @brief Spectrum status structure
	 * @details This type is used to store spectrum status
	 */
	typedef struct {
		bool running;					/**< True if the spectrum is running*/
		bool completed;					/**< True if the limit is not freeruning and the limit has been reached*/
		uint32_t progress;				/**< Progress of the spectrum in respect of the limit in percent*/
		uint32_t peak_max;				/**< Maximum peak value*/
		uint32_t total_counter;			/**< Total number of counts on the whole spectrum*/
		double integration_time;		/**< Integration time in ms*/
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



	/*
	* OPENDPP STRUCT TYPEDEF
	CHANNEL U8 0 Channel (7 bits)
	TIMESTAMP U64 0 Timestamp (48 bits)
	TIMESTAMP_NS U64 0 Timestamp in nanoseconds (51 bits)
	FINE_TIMESTAMP U16 0 Fine timestamp (10 bits)
	ENERGY U16 0 Energy (16 bits)
	FLAGS_B U16 0 Event low priority flags (12 bits). See Low Priority.
	FLAGS_A U8 0 Event high priority flags (8 bits). See High Priority.
	PSD U16 0 Pulse Shape Discrimination (16 bits)
	USER_INFO U64 1 User Info word
	USER_INFO_SIZE SIZE_T 0 Number of User Info word
	TRUNCATED BOOL 0 Set if the event is truncated
	WAVEFORM U16 1 Waveform samples
	WAVEFORM_SIZE SIZE_T 0 Number of waveform samples.
	BOARD_FAIL BOOL 0 Set if the logical AND between of the ErrorFlag and
	ErrorFlagDataMask is not 0. Present in all the events
	SPECIAL_EVENT BOOL 0 Set if Special event
	EVENT_SIZE SIZE_T 0 Total event raw size, useful for statistics and to
	discriminate between compressed events (if == 8, see
	EnDataReduction) and standard events (if > 8).
	FLUSH BOOL 0 Flush of the aggregate word. Shared by all the events in
	the aggregate. Useful for debug purposes.
	AGGREGATE_COUNTER U32 0 Aggregate counter section in the Event Aggregate
	structure. Shared by all the events
	*/

	typedef struct {
		uint8_t channel;
		uint64_t timestamp;
		uint64_t timestamp_ns;
		uint16_t fine_timestamp;
		uint16_t energy;
		uint16_t flags_b;
		uint8_t flags_a;
		uint16_t psd;
		uint64_t* user_info;
		uint32_t user_info_size;
		bool truncated;
		uint16_t* waveform;
		uint32_t waveform_size;
		bool board_fail;
		bool special_event;
		uint32_t event_size;
		bool flush;
		uint32_t aggregate_counter;
	} SCISDK_FE_OPENDPP_EVENT;
#endif
