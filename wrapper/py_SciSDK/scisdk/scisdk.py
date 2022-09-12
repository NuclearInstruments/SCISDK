import ctypes
import os

class SciSDK:
    scisdk_dll = None
    lib_ptr = None

    def __init__(self) -> None:
        self.scisdk_dll = ctypes.WinDLL(os.path.dirname(__file__) + "\\SciSDK_DLL.dll")
        self.lib_ptr = self.scisdk_dll.SCISDK_InitLib()

    def AddNewDevice(device_path: str, device_model: str, json_file_path: str, name: str, self):
        add_new_device_api_proto = ctypes.WINFUNCTYPE (
            ctypes.c_int,
            ctypes.c_char_p,
            ctypes.c_char_p,
            ctypes.c_char_p,
            ctypes.c_char_p,
            ctypes.c_void_p) 
        add_new_device_api_params = (1, "device_path", 0), (1, "device_model", 0), (1, "json_file_path",0), (1, "name",0),(1, "handle",0),

        add_new_device_api = add_new_device_api_proto (("SCISDK_AddNewDevice", self.scisdk_dll), add_new_device_api_params)

        p1 = ctypes.c_char_p (device_path)
        p2 = ctypes.c_char_p (device_model)
        p3 = ctypes.c_char_p (json_file_path)
        p4 = ctypes.c_char_p (name)
        add_new_device_api (p1, p2, p3, p4, self.lib_ptr)
    
    # defines
    # Oscilloscope decoded buffer
    class OscilloscopeDecodedBuffer(ctypes.Structure):
        _fields_ = [('magic', ctypes.c_uint32),
                ('analog', ctypes.POINTER(ctypes.c_int32)),
                ('digital',  ctypes.POINTER(ctypes.c_int8)),
                ('trigger_position', ctypes.c_uint32),
                ('timecode', ctypes.c_uint64)]

    class OscilloscopeDecodedBufferInfo(ctypes.Structure):
        _fields_ = [('samples_analog', ctypes.c_uint32),
                ('samples_digital', ctypes.c_uint32),
                ('tracks_analog_per_channel',  ctypes.c_uint32),
                ('tracks_digital_per_channel', ctypes.c_uint32),
                ('channels', ctypes.c_uint32)]