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
                ('data', ctypes.c_char_p),
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