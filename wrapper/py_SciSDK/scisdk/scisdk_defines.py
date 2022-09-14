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

