#ifndef HEADER_H_SCISDK_DEFINES
#define HEADER_H_SCISDK_DEFINES
#include <stdint.h>
#include <stdlib.h>

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

	/**
	 * @brief Digitizer decoded data type
	 * @details This type is used to store decoded waveform from the digitizer component.
	 * The reference document for the raw data format is the  [Digitizer  driver](digitizer.md)
	 */
	typedef struct {
		uint32_t magic;							/**< Magic number to identify the data type*/
		int32_t *analog;						/**< Pointer to analog data allocated by the AllocateBuffer function*/
		uint64_t hits;							/**< Channel that generate a trigger (hits)*/
		uint64_t timecode;						/**< Timecode of the first sample. Taken by timecome input in SciCompiler firmware*/
		uint32_t counter;						/**< Counter progressive value*/
		uint32_t user;							/**< User value field from SciCompiler IP block*/	
		struct {
			uint32_t samples;					/**< Number of samples allocated in analog buffer*/
			uint32_t valid_samples;				/**< Number of valid samples in analog buffer*/
			uint32_t channels;					/**< Number of channels of the digitizer*/
			uint32_t enabled_channels;			/**< Number of enabled channels*/
		} info;
	}SCISDK_DIGITIZER_DECODED_BUFFER;
	
	/**
	 * @brief Digitizer raw data type
	 * @details This type is used to store raw from the digitizer component.
	 * The reference document for the raw data format is the  [Digitizer  driver](digitizer.md)
	 */
	typedef struct {
		uint32_t magic;							/**< Magic number to identify the data type*/
		int32_t *data;							/**< Pointer to data allocated by the AllocateBuffer function*/
		struct {
			uint32_t buffer_size;				/**< Size in DWORD of the data buffer*/
			uint32_t samples;					/**< Number of samples allocated in the data buffer*/
			uint32_t valid_samples;				/**< Number of valid samples allocated in the data buffer*/
			uint32_t channels;					/**< Number of channels of the digitizer*/
		} info;
	}SCISDK_DIGITIZER_RAW_BUFFER;


	/**
	 * @brief List raw data type
	 * @details This type is used to store raw data from list.
	 * The reference document for the raw data format is the  [List  driver](list.md)
	 */
	typedef struct {
		uint32_t magic;					/**< Magic number to identify the data type*/
		char *data;						/**< Pointer to data allocated by the AllocateBuffer function*/
		struct {
			uint32_t buffer_size;		/**< Size of the buffer in bytes*/
			uint32_t samples;			/**< Number of samples in the buffer expressed in FPGA Word*/	
			uint32_t valid_samples;		/**< Number of valid samples in the buffer expressed in FPGA Word*/
			uint32_t channels;			/**< Number of channels, always 1*/
		} info;
	}SCISDK_LIST_RAW_BUFFER;

	/**
	 * @brief Custom Packet body decoded data
	 * @details Packet structure for custom packet body is defined by SciCompiler Custom Packet Editor
	 * The user can add a custom number of rows. Every rows is 32 bit wide.
	 * Inside a row the user can add a custom number of fields. Every field can have a with between 1 and 32 bit.
	 * It is possible to create filed larger than 32 bit by using multiple rows.
	 * The inputs to custom packet can be larger from 1 to 1024 bits.
	 * On FPGA side will appear as a single input, but in the custom packet, if larger than 32 bit
	 * will use multiple rows.
	 * It is possible to cast rows to a custom structure to recostruct the original FPGA data layout.
	 */
	typedef struct {
		uint32_t *row;			/**< Pointer to the row data, this express every raw in the event created by SciCompiler*/
		uint32_t n;				/**< Number of row in the event*/
	}SCISDK_CP_PACKET;

	/**
	 * @brief Custom Packet Decoded data structure
	 * @details This type is used to store data for custom packet in Decoded mode
	 * The reference document for the data format is the  [Custom Packet Driver](custom-packet.md)
	 */
	typedef struct {
		uint32_t magic;					/**< Magic number to identify the data type*/
		SCISDK_CP_PACKET *data;			/**< Pointer to data allocated by the AllocateBuffer function*/
		struct {
			uint32_t buffer_size;		/**< Size of the buffer in number of SCISDK_CP_PACKET allocated*/
			uint32_t packet_size;		/**<  Number of 32-bit rows in the data packet*/
			uint32_t valid_data;		/**< Number of valid data in the buffer expressed in number of SCISDK_CP_PACKET*/
		} info;
	}SCISDK_CP_DECODED_BUFFER;


	/**
	 * @brief Custom Packet Raw data structure
	 * @details This type is used to store data for custom packet in raw mode
	 * The reference document for the data format is the  [Custom Packet Driver](custom-packet.md)
	 * @note When used in RAW mode the custom packet behave like a list with 32 bit word size and the meaning of packet is lost
	 */
	typedef struct {
		uint32_t magic;				/**< Magic number to identify the data type*/
		uint32_t *data;				/**< Pointer to data allocated by the AllocateBuffer function*/
		struct {
			uint32_t buffer_size;	/**< Size of the buffer in in DWORD*/
			uint32_t valid_data;	/**< Number of valid data in the buffer in DWORD*/
		} info;
	}SCISDK_CP_RAW_BUFFER;

	/**
	 * @brief Rate Meter data structure
	 * @details This type is used to store data from rate meter component
	 * The reference document for the data format is the  [Rate Meter](rate-meter.md)
	 */
	typedef struct {
		uint32_t magic;					/**< Magic number to identify the data type*/
		double *data;					/**< Pointer to data containings the counts in the integration time allocated by the AllocateBuffer function*/
		struct {
			uint32_t buffer_size;		/**< Size of the buffer in samples*/
			uint32_t valid_data;		/**< Number of valid samples in the buffer*/
			uint32_t nchannels;			/**< Number of channels*/
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


	/**
	 * @brief FFT monitor decoded data structure
	 * @details This type is used to store FFT moditor decoded result
	 * The reference document for the data format is the  [FFT](fft.md)
	 */
	typedef struct {
		uint32_t magic;					/**< Magic number to identify the data type*/
		double *mag;					/**< Magnitude array llocated by the AllocateBuffer function*/
		double *ph;						/**< Phase array llocated by the AllocateBuffer function*/
		uint64_t timecode;				/**< Timestamp of the acquisition*/
		struct {
			uint32_t samples;			/**< the number of samples per channel*/
			uint32_t channels;			/**< the number of channel*/
		} info;
	}SCISDK_FFT_DECODED_BUFFER;

	/**
	 * @brief FFT monitor raw data structure
	 * @details This type is used to store FFT monitor raw result
	 * The reference document for the data format is the  [FFT](fft.md)
	 */
	typedef struct {
		uint32_t magic;					/**< Magic number to identify the data type*/
		uint32_t *data;					/**< Real/Imaginary part  array llocated by the AllocateBuffer function
											The first DWORD is the Real, the second is the Immaginary part
										*/					
		uint64_t timecode;				/**< Timestamp of the acquisition*/
		struct {
			uint32_t buffer_size;		/**< Size of the data buffer in DWORD*/
			uint32_t samples;			/**< Number of samples (Re+Im) in data per channel*/
			uint32_t channels;			/**< Number of channel*/
		} info;
	}SCISDK_FFT_RAW_BUFFER;

	typedef struct {
		bool armed;
		bool ready;
		bool running;
	}SCISDK_FFT_STATUS;


	/**
	 * @brief Single decoded event by the frame transfer driver
	 * @details This type is used to store a single decoded event by the frame transfer driver,
	 * multiple event are stored in a SCISDK_FRAME_DECODED_BUFFER
	 * The reference document for the data format is the  [Frame](frame.md)
	 */
	typedef struct {
		uint32_t *pixel;					/**< Pointer to the pixel array*/
		uint32_t n;							/**< Number of pixel in the array*/
		struct {
			uint64_t timestamp;				/**< Timestamp of the event*/
			uint64_t trigger_count;			/**< Progressive trigger counter calculated in FPGA*/
			uint64_t event_count;			/**< Progressive event counter calculated in FPGA*/
			uint64_t hits;					/**< hits vector, each 1 indicate that the respective Pixel generate a trigger for the event*/
		} info;
	}SCISDK_FRAME_PACKET;

	/**
	 * @brief Decoded events list created by the frame transfer driver
	 * @details This type is used to store a list of SCISDK_FRAME_PACKET
	 * The reference document for the data format is the  [Frame](frame.md)
	 */
	typedef struct {
		uint32_t magic;						/**< Magic number to identify the data type*/
		SCISDK_FRAME_PACKET *data;			/**< Pointer to the array of SCISDK_FRAME_PACKET*/
		struct {
			uint32_t buffer_size;			/**< Size of the data buffer in events*/
			uint32_t valid_data;			/**< Number of valid events in the buffer*/
		} info;
	}SCISDK_FRAME_DECODED_BUFFER;

	/**
	 * @brief Frame raw data structure
	 * @details This type is used to store frame raw result
	 * The reference document for the data format is the [Frame](frame.md)
	 */
	typedef struct {
		uint32_t magic;				/**< Magic number to identify the data type*/
		uint32_t *data;				/**< Pointer to data allocated by the AllocateBuffer function*/
		struct {
			uint32_t buffer_size;	/**< Size of the data buffer in DWORD*/
			uint32_t packet_size;	/**< Size of the packet in DWORD, calculated from JSON file*/
			uint32_t valid_data;	/**< Number of valid data in the buffer in DWORD*/
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


	/**
	 * @brief OpenScope structure
	 * @details This type is used to store scope (waveform) events from CAEN OpenScope result
	 * The reference document for the data format is the [OpenScope](openscope.md)
	 */
	typedef struct  {
		uint64_t timestamp;					/**<  A 64-bit unsigned integer representing the timestamp of the event in nanoseconds. */
		uint32_t trigger_id;				/**< Trigger ID (32 bits) */
		size_t event_size;					/**< Event size in bytes */
		uint16_t** waveform;				/**< A pointer to an array of pointers to 16-bit unsigned integers, 
												where each element in the array represents a channel of the oscilloscope and each 
												element in the array of pointers represents a sample in that channel. */
		size_t* n_samples;					/**< the number of samples in each channel of the oscilloscope. */
		size_t* n_allocated_samples;		/**< the number of samples that have been allocated for each channel of the oscilloscope. */
		unsigned n_channels;				/**< the number of channels in the oscilloscope.*/ 
	} SCISDK_FE_SCOPE_EVENT;




	/**
	 * @brief OpenDPP structure
	 * @details This type is used to store decoded events from CAEN OpenDPP result
	 * The reference document for the data format is the [OpenDPP](opendpp.md)
	 */
	typedef struct {
		uint8_t channel;				/**< Channel (7 bits) */
		uint64_t timestamp;				/**< Timestamp (48 bits) */
		uint64_t timestamp_ns;			/**< Timestamp in nanoseconds (51 bits) */
		uint16_t fine_timestamp;		/**< Fine timestamp (10 bits) */
		uint16_t energy;				/**< Energy (16 bits) */
		uint16_t flags_b;				/**< Event low priority flags (12 bits). See Low Priority. */
		uint8_t flags_a;				/**< Event high priority flags (8 bits). See High Priority. */
		uint16_t psd;					/**< Pulse Shape Discrimination (16 bits) */
		uint64_t* user_info;			/**< User Info word */
		uint32_t user_info_size;		/**< Number of User Info word */
		bool truncated;					/**< 0 Set if the event is truncated*/
		uint16_t* waveform;				/**< Waveform samples */
		uint32_t waveform_size;			/**< Number of waveform samples. */
		bool board_fail;				/**< Set if the logical AND between of the ErrorFlag and ErrorFlagDataMask is not 0. Present in all the events */
		bool special_event;				/**< Set if Special event */
		uint32_t event_size;			/**< Total event raw size, useful for statistics and to discriminate between compressed events (if == 8, see EnDataReduction) and standard events (if > 8). */
		bool flush;						/**< Flush of the aggregate word. Shared by all the events in the aggregate. Useful for debug purposes. */
		uint32_t aggregate_counter;		/**< Aggregate counter section in the Event Aggregate structure. Shared by all the events */
	} SCISDK_FE_OPENDPP_EVENT;
#endif
