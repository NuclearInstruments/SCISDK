import ctypes

# defines
# Oscilloscope decoded buffer
class OscilloscopeDecodedBufferInfo(ctypes.Structure):
    _fields_ = [('samples_analog', ctypes.c_uint32),
            ('samples_digital', ctypes.c_uint32),
            ('tracks_analog_per_channel',  ctypes.c_uint32),
            ('tracks_digital_per_channel', ctypes.c_uint32),
            ('channels', ctypes.c_uint32)]

class OscilloscopeDecodedBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
            ('analog', ctypes.POINTER(ctypes.c_int32)),
            ('digital',  ctypes.POINTER(ctypes.c_int8)),
            ('trigger_position', ctypes.c_uint32),
            ('timecode', ctypes.c_uint64),
            ('info', OscilloscopeDecodedBufferInfo)]

# Oscilloscope raw buffer
class OscilloscopeRawBufferInfo(ctypes.Structure):
    _fields = [('buffer_size', ctypes.c_uint32),
            ('samples_analog', ctypes.c_uint32),
            ('samples_digital', ctypes.c_uint32),
            ('tracks_analog_per_channel',  ctypes.c_uint32),
            ('tracks_digital_per_channel', ctypes.c_uint32),
            ('channels', ctypes.c_uint32)]

class OscilloscopeRawBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
            ('data', ctypes.POINTER(ctypes.c_uint32)),
            ('zero_position', ctypes.c_uint32),
            ('trigger_position', ctypes.c_uint32),
            ('timecode', ctypes.c_uint64),
            ('info', OscilloscopeRawBufferInfo)]

# Oscilloscope dual decoded buffer
class OscilloscopeDualDecodedBufferInfo(ctypes.Structure):
    _fields_ = [('samples_analog', ctypes.c_uint32),
            ('samples_digital', ctypes.c_uint32),
            ('tracks_analog_per_channel',  ctypes.c_uint32),
            ('tracks_digital_per_channel', ctypes.c_uint32),
            ('channels', ctypes.c_uint32)]

class OscilloscopeDualDecodedBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
            ('analog', ctypes.POINTER(ctypes.c_int32)),
            ('digital',  ctypes.POINTER(ctypes.c_int8)),
            ('trigger_position', ctypes.c_uint32),
            ('timecode', ctypes.c_uint64),
            ('info', OscilloscopeDecodedBufferInfo)]

# Oscilloscope dual raw buffer
class OscilloscopeDualRawBufferInfo(ctypes.Structure):
    _fields = [('buffer_size', ctypes.c_uint32),
            ('samples_analog', ctypes.c_uint32),
            ('samples_digital', ctypes.c_uint32),
            ('tracks_analog_per_channel',  ctypes.c_uint32),
            ('tracks_digital_per_channel', ctypes.c_uint32),
            ('channels', ctypes.c_uint32)]

class OscilloscopeDualRawBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
            ('data', ctypes.POINTER(ctypes.c_uint32)),
            ('zero_position', ctypes.c_uint32),
            ('trigger_position', ctypes.c_uint32),
            ('timecode', ctypes.c_uint64),
            ('info', OscilloscopeRawBufferInfo)]

# Digitizer decoded buffer
class DigitizerDecodedBufferInfo(ctypes.Structure):
    _fields_ = [('samples', ctypes.c_uint32),
                ('valid_samples', ctypes.c_uint32),
                ('channels', ctypes.c_uint32)]

class DigitizerDecodedBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
                ('analog', ctypes.POINTER(ctypes.c_int32)),
                ('hits', ctypes.c_uint64),
                ('timecode', ctypes.c_uint64),
                ('counter', ctypes.c_uint32),
                ('user', ctypes.c_uint32),
                ('info', DigitizerDecodedBufferInfo)]

# List raw buffer
class ListRawBufferInfo(ctypes.Structure):
    _fields_ = [('buffer_size', ctypes.c_uint32),
                ('samples', ctypes.c_uint32),
                ('valid_samples', ctypes.c_uint32),
                ('channels', ctypes.c_uint32)]

class ListRawBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
                ('data', ctypes.POINTER(ctypes.c_char)),
                ('info', ListRawBufferInfo)]

# Custom packet packet
class CustomPacketPacket(ctypes.Structure):
    _fields_ = [('row', ctypes.POINTER(ctypes.c_uint32)),
                ('n', ctypes.c_uint32)]

# Custom packet decoded buffer
class CustomPacketDecodedBufferInfo(ctypes.Structure):
    _fields_ = [('buffer_size', ctypes.c_uint32),
                ('packet_size', ctypes.c_uint32),
                ('valid_data', ctypes.c_uint32)]

class CustomPacketDecodedBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
                ('data', ctypes.POINTER(CustomPacketPacket)),
                ('info', CustomPacketDecodedBufferInfo)]

# Custom packet raw buffer
class CustomPacketRawBufferInfo(ctypes.Structure):
    _fields_ = [('buffer_size', ctypes.c_uint32),
                ('valid_data', ctypes.c_uint32)]

class CustomPacketRawBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
                ('data', ctypes.POINTER(ctypes.c_uint32)),
                ('info', CustomPacketRawBufferInfo)]

# Ratemeter raw buffer
class RatemeterRawBufferInfo(ctypes.Structure):
    _fields_ = [('buffer_size', ctypes.c_uint32),
                ('valid_data', ctypes.c_uint32),
                ('nchannels', ctypes.c_uint32)]

class RatemeterRawBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
                ('data', ctypes.POINTER(ctypes.c_double)),
                ('info', RatemeterRawBufferInfo)]

# Spectrum decoded buffer
class Histogram2DDecodedBufferInfo(ctypes.Structure):
    _fields_ = [('buffer_size', ctypes.c_uint32),
                ('total_bins', ctypes.c_uint32),
                ('valid_bins', ctypes.c_uint32),
                ('binsX', ctypes.c_uint32),
                ('binsY', ctypes.c_uint32)
                ]

class Histogram2DDecodedBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
                ('data', ctypes.POINTER(ctypes.c_uint32)),
                ('timecode', ctypes.c_uint64),
                ('inttime', ctypes.c_uint32),
                ('info', Histogram2DDecodedBufferInfo),]
    
# Spectrum decoded buffer
class SpectrumDecodedBufferInfo(ctypes.Structure):
    _fields_ = [('buffer_size', ctypes.c_uint32),
                ('total_bins', ctypes.c_uint32),
                ('valid_bins', ctypes.c_uint32)]

class SpectrumDecodedBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
                ('data', ctypes.POINTER(ctypes.c_uint32)),
                ('timecode', ctypes.c_uint64),
                ('inttime', ctypes.c_uint32),
                ('info', SpectrumDecodedBufferInfo),]

# FFT decoded buffer
class FFTDecodedBufferInfo(ctypes.Structure):
    _fields_ = [('samples', ctypes.c_uint32),
                ('channels', ctypes.c_uint32)]

class FFTDecodedBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
                ('double', ctypes.POINTER(ctypes.c_double)),
                ('ph', ctypes.POINTER(ctypes.c_double)),
                ('timecode', ctypes.c_uint64),
                ('info', FFTDecodedBufferInfo)]

# FFT raw buffer
class FFTRawBufferInfo(ctypes.Structure):
    _fields_ = [('buffer_size', ctypes.c_uint32),
                ('samples', ctypes.c_uint32),
                ('channels', ctypes.c_uint32)]

class FFTRawBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
                ('data', ctypes.POINTER(ctypes.c_uint32)),
                ('timecode', ctypes.c_uint64),
                ('info', FFTRawBufferInfo)]


# Frame packet decoded info
class CustomPacketDecodedBufferInfo(ctypes.Structure):
    _fields_ = [('timestamp', ctypes.c_uint64),
                ('trigger_count', ctypes.c_uint64),
                ('event_count', ctypes.c_uint64),
                ('hits', ctypes.c_uint64)]

# Frame packet packet
class FramePacket(ctypes.Structure):
    _fields_ = [('pixel', ctypes.POINTER(ctypes.c_uint32)),
                ('n', ctypes.c_uint32),
                ('info', CustomPacketDecodedBufferInfo)
                ]

# Custom packet decoded buffer
class FramePacketDecodedBufferInfo(ctypes.Structure):
    _fields_ = [('buffer_size', ctypes.c_uint32),
                ('valid_data', ctypes.c_uint32)]

class FramePacketDecodedBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
                ('data', ctypes.POINTER(FramePacket)),
                ('info', FramePacketDecodedBufferInfo)]

# Custom packet raw buffer
class FramePacketRawBufferInfo(ctypes.Structure):
    _fields_ = [('buffer_size', ctypes.c_uint32),
                ('packet_size', ctypes.c_uint32),
                ('valid_data', ctypes.c_uint32)]

class FramePacketRawBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
                ('data', ctypes.POINTER(ctypes.c_uint32)),
                ('info', FramePacketRawBufferInfo)]


# Citiroc Pixel
class CitirocPixel(ctypes.Structure):
    _fields_ = [('hg', ctypes.c_uint16),
                ('lg', ctypes.c_uint16),
                ('hit', ctypes.c_uint8)
                ]

# Citiroc packet info
class CitirocPacketInfo(ctypes.Structure):
    _fields_ = [('asic', ctypes.c_uint8),
                ('timestamp_from_t0', ctypes.c_uint64),
                ('timestamp_from_run', ctypes.c_uint64),
                ('event_id', ctypes.c_uint32),
                ('trigger_count', ctypes.c_uint32),
                ('validation_counter', ctypes.c_uint32),
                ('flags', ctypes.c_uint32)
                ]
# Citiroc packet
class CitirocPacket(ctypes.Structure):
    _fields_ = [('pixel', ctypes.POINTER(CitirocPixel)),
                ('n', ctypes.c_uint32),
                ('info', CitirocPacketInfo)
                ]

# Citiroc decoded buffer info
class CitirocDecodedBufferInfo(ctypes.Structure):
    _fields_ = [('buffer_size', ctypes.c_uint32),
                ('valid_data', ctypes.c_uint32)]

# Citiroc decoded buffer
class CitirocDecodedBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
                ('data', ctypes.POINTER(CitirocPacket)),
                ('info', CitirocDecodedBufferInfo)]

# Citiroc raw buffer info
class CitirocRawBufferInfo(ctypes.Structure):
    _fields_ = [('buffer_size', ctypes.c_uint32),
                ('packet_size', ctypes.c_uint32),
                ('valid_data', ctypes.c_uint32)]

# Citiroc raw buffer
class CitirocRawBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
                ('data', ctypes.POINTER(ctypes.c_uint32)),
                ('info', CitirocRawBufferInfo)]

# Petiroc Pixel
class PetirocPixel(ctypes.Structure):   
    _fields_ = [('charge', ctypes.c_uint16),
                ('fine', ctypes.c_uint16),
                ('coarse', ctypes.c_uint16),
                ('hit', ctypes.c_uint8)
                ]

# Petiroc packet info
class PetirocPacketInfo(ctypes.Structure):
    _fields_ = [('asic', ctypes.c_uint8),
                ('timestamp_from_t0', ctypes.c_uint64),
                ('timestamp_from_run', ctypes.c_uint64),
                ('event_id', ctypes.c_uint32),
                ('trigger_count', ctypes.c_uint32),
                ('validation_counter', ctypes.c_uint32),
                ('flags', ctypes.c_uint32)
                ]

# Petiroc packet
class PetirocPacket(ctypes.Structure):
    _fields_ = [('pixel', ctypes.POINTER(PetirocPixel)),
                ('n', ctypes.c_uint32),
                ('info', PetirocPacketInfo)
                ]

# Petiroc decoded buffer info
class PetirocDecodedBufferInfo(ctypes.Structure):
    _fields_ = [('buffer_size', ctypes.c_uint32),
                ('valid_data', ctypes.c_uint32)]

# Petiroc decoded buffer
class PetirocDecodedBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
                ('data', ctypes.POINTER(PetirocPacket)),
                ('info', PetirocDecodedBufferInfo)]

# Petiroc raw buffer info
class PetirocRawBufferInfo(ctypes.Structure):
    _fields_ = [('buffer_size', ctypes.c_uint32),
                ('packet_size', ctypes.c_uint32),
                ('valid_data', ctypes.c_uint32)]

# Petiroc raw buffer
class PetirocRawBuffer(ctypes.Structure):
    _fields_ = [('magic', ctypes.c_uint32),
                ('data', ctypes.POINTER(ctypes.c_uint32)),
                ('info', PetirocRawBufferInfo)]

# FE-Scope packet
# FE-Scope packet
class FEScopePacket(ctypes.Structure):
    _fields_ = [('timestamp', ctypes.c_uint64),
                ('trigger_id', ctypes.c_size_t),
                ('event_size', ctypes.c_size_t),
                ('waveform', ctypes.POINTER(ctypes.POINTER(ctypes.c_uint16))),
                ('n_samples', ctypes.POINTER(ctypes.c_size_t)),
                ('n_allocated_samples', ctypes.POINTER(ctypes.c_size_t)),
                ('n_channels', ctypes.c_uint32)]



# FE_OpenDpp packet
class FEOpenDppPacket(ctypes.Structure):
    _fields_ = [('channel', ctypes.c_uint8),
                ('timestamp', ctypes.c_uint64),
                ('timestamp_ns', ctypes.c_uint64),
                ('fine_timestamp', ctypes.c_uint16),
                ('energy', ctypes.c_uint16),
                ('flags_b', ctypes.c_uint16),
                ('flags_a', ctypes.c_uint8),
                ('psd', ctypes.c_uint16),
                ('user_info', ctypes.POINTER(ctypes.c_uint64)),
                ('user_info_size', ctypes.c_uint32),
                ('truncated', ctypes.c_bool),
                ('waveform', ctypes.POINTER(ctypes.c_uint16)),
                ('waveform_size', ctypes.c_uint32),
                ('board_fail', ctypes.c_bool),
                ('special_event', ctypes.c_bool),
                ('event_size', ctypes.c_uint32),
                ('flush', ctypes.c_bool),
                ('aggregate_counter', ctypes.c_uint32),
                ]



class EmulatorEnergySpectrumInfo(ctypes.Structure):
    _fields_ = [
        ('allocated_bins', ctypes.c_uint32),
        ('valid_bins', ctypes.c_uint32)
    ]

# Detector emulator energy spectrum
class EmulatorEnergySpectrum(ctypes.Structure):
    _fields_ = [
        ('magic', ctypes.c_uint32),               
        ('data', ctypes.POINTER(ctypes.c_uint32)),
        ('info', EmulatorEnergySpectrumInfo)
    ]

    def __init__(self, num_bins=16384):
        super().__init__()

        # Initializer 'magic' e 'info'
        self.magic = 12347
        self.info.allocated_bins = num_bins  # allocated bins
        self.info.valid_bins = 0  # valid bins

        # allocate array of num_bins
        self.c_array = (ctypes.c_uint32 * num_bins)()
        self.data = ctypes.cast(self.c_array, ctypes.POINTER(ctypes.c_uint32))

class NIErrors:
    NI_OK = 0x00000000
    NI_ERROR_GENERIC = 0x00000001
    NI_ERROR_INTERFACE = 0x00000002
    NI_ERROR_FPGA = 0x00000003
    NI_ERROR_TRANSFER_MAX_LENGTH = 0x00000004
    NI_ERROR_NOTCONNECTED = 0x00000005
    NI_NO_DATA_AVAILABLE = 0x00000006
    NI_TOO_MANY_DEVICES_CONNECTED = 0x00000007
    NI_INVALID_HANDLE = 0x00000008
    NI_INVALID_KEY = 0x00000009
    NI_INVALID_PARAMETER = 0x0000000A
    NI_PARAMETER_OUT_OF_RANGE = 0x0000000B
    NI_INCOMPLETE_READ = 0x0000000C
    NI_INVALID_COMMAND = 0x0000000D
    NI_PARAMETER_CAN_NOT_BE_CANGHED_IN_RUN = 0x0000000E
    NI_PARAMETER_CAN_NOT_BE_SET_WITH_THIS_CONFIG = 0x0000000F
    NI_PARAMETER_CAN_NOT_BE_SET = 0x00000010
    NI_PARAMETER_MIN_MAX_NOT_SET = 0x00000011
    NI_UNABLE_TO_LOAD_EXTERNAL_LIBRARY = 0x00000012
    NI_INVALID_PARAMETER_PATH = 0x00000013
    NI_INVALID_PARAMETER_ROOT = 0x00000014
    NI_NOT_SUPPORTED = 0x00000015
    NI_INVALID_ACQ_MODE = 0x00000016
    NI_INVALID_METHOD = 0x00000017
    NI_ALREADY_CONNECTED = 0x00000100
    NI_ALLOC_FAILED = 0x00000200
    NI_MEMORY_NOT_ALLOCATED = 0x00000201
    NI_INVALID_BUFFER_TYPE = 0x00000202
    NI_INVALID_BUFFER_SIZE = 0x00000203
    NI_INCOMPATIBLE_BUFFER = 0x00000204
    NI_INVALID_BUFFER = 0x00000205
    NI_NOT_ARMED = 0x00000206
    NI_TIMEOUT = 0x00000300
    NI_INVALID_CFG_JSON = 0x10000000
    NI_CFG_JSON_NOT_FOUND = 0x10000001
    NI_DEVICE_NAME_ALREADY_EXISTS = 0x10000002
    NI_INVALID_PATH = 0x10001000
    NI_NOT_FOUND = 0x10001FFE
    NI_INVALID_TYPE = 0x10001FFF
    NI_ALREADY_RUNNING = 0x10003000
    NI_NOT_RUNNING = 0x10003001
    NI_FEELIB_INTERNAL_ERROR = 0x20000000

    NI_SPECIFIC_ERROR = 0xFFFFFFFD
    NI_LIBRARY_ERROR = 0xFFFFFFFE
