import ctypes
import os
from scisdk.scisdk_defines import *

class SciSDK:
    scisdk_dll = None
    lib_ptr = None

    def __init__(self) -> None:
        self.scisdk_dll = ctypes.CDLL(os.path.dirname(__file__) + "\\SciSDK_DLL.dll")
        init_lib_api = self.scisdk_dll.SCISDK_InitLib
        init_lib_api.restype = ctypes.c_void_p
        self.lib_ptr = init_lib_api()

    def AddNewDevice(self, device_path: str, device_model: str, json_file_path: str, name: str) -> int:
        add_new_device_api = self.scisdk_dll.SCISDK_AddNewDevice
        add_new_device_api.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_void_p]
        add_new_device_api.restype = ctypes.c_int
        # convert strings to bytes array
        b_device_path = device_path.encode('utf-8')
        b_device_model = device_model.encode('utf-8')
        b_json_file_path = json_file_path.encode('utf-8')
        b_name = name.encode('utf-8')
        # call C lib function
        err = add_new_device_api(b_device_path, b_device_model, b_json_file_path, b_name, self.lib_ptr)
        return err
        
    def DetachDevice(self, name: str) -> int:
        detach_device_api = self.scisdk_dll.SCISDK_DetachDevice
        detach_device_api.argtypes = [ctypes.c_char_p, ctypes.c_void_p]
        detach_device_api.restype = ctypes.c_int
        # convert string to bytes array
        b_name = name.encode('utf-8')
        # call C lib function
        err = detach_device_api(b_name, self.lib_ptr)
        return err

    def GetComponentList(self, board_name: str, node_type: str, return_json: bool):
        get_components_list_api = self.scisdk_dll.SCISDK_GetComponentList
        get_components_list_api.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_char_p), ctypes.c_bool, ctypes.c_void_p]
        get_components_list_api.restype = ctypes.c_int
        # convert string to bytes array
        board_name_b = board_name.encode('utf-8')
        node_type_b = node_type.encode('utf-8')
        ret_string_char_ptr = ctypes.c_char_p()
        # call C lib function
        err = get_components_list_api(ctypes.c_char_p(board_name_b), ctypes.c_char_p(node_type_b), ctypes.byref(ret_string_char_ptr), ctypes.c_bool(return_json), self.lib_ptr)
        return err, ret_string_char_ptr.value.decode('utf-8')

    def s_error(self, value: int) -> int:
        s_error_api = self.scisdk_dll.SCISDK_s_error
        s_error_api.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_char_p), ctypes.c_void_p]
        s_error_api.restype = ctypes.c_int
        # convert string to bytes array
        ret_string_char_ptr = ctypes.c_char_p()
        err = s_error_api(ctypes.c_int(value), ctypes.byref(ret_string_char_ptr),self.lib_ptr)
        return err, ret_string_char_ptr.value.decode('utf-8')

    def SetParameterInteger(self, path: str, val: int) -> int:
        set_parameter_api = self.scisdk_dll.SCISDK_SetParameterInteger
        set_parameter_api.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_void_p]
        set_parameter_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        # call C lib function
        err = set_parameter_api(ctypes.c_char_p(path_b), val, self.lib_ptr)
        return err

    def GetParameterInteger(self, path: str):
        get_parameter_api = self.scisdk_dll.SCISDK_GetParameterInteger
        get_parameter_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_int), ctypes.c_void_p]
        get_parameter_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        ret_int = ctypes.c_int()
        # call C lib function
        err = get_parameter_api(ctypes.c_char_p(path_b), ctypes.byref(ret_int), self.lib_ptr)
        return err, ret_int.value

    def SetParameterString(self, path: str, value: str):
        set_parameter_api = self.scisdk_dll.SCISDK_SetParameterString
        set_parameter_api.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_void_p]
        set_parameter_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        value_b = value.encode('utf-8')
        # call C lib function
        err = set_parameter_api(ctypes.c_char_p(path_b), ctypes.c_char_p(value_b), self.lib_ptr)
        return err

    def GetParameterString(self, path: str):
        get_parameter_api = self.scisdk_dll.SCISDK_GetParameterString
        get_parameter_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_char_p), ctypes.c_void_p]
        get_parameter_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        value_b = ctypes.c_char_p()
        # call C lib function
        err = get_parameter_api(ctypes.c_char_p(path_b), ctypes.byref(value_b), self.lib_ptr)
        return err, value_b.value.decode('utf-8')

    def SetParameterDouble(self, path: str, value: float):
        set_parameter_api = self.scisdk_dll.SCISDK_SetParameterDouble
        set_parameter_api.argtypes = [ctypes.c_char_p, ctypes.c_double, ctypes.c_void_p]
        set_parameter_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        # call C lib function
        err = set_parameter_api(ctypes.c_char_p(path_b), ctypes.c_double(value), self.lib_ptr)
        return err

    def GetParameterDouble(self, path:str):
        get_parameter_api = self.scisdk_dll.SCISDK_SetParameterDouble
        get_parameter_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.c_void_p]
        get_parameter_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        value = ctypes.c_double()
        # call C lib function
        err = get_parameter_api(ctypes.c_char_p(path_b), ctypes.POINTER(value), self.lib_ptr)
        return err, value.value

    # int SCISDK_AllocateBuffer(char* Path, int buffer_type, void **buffer, void *handle);
    def AllocateBuffer(self, path: str, buffer_type: int, buffer: OscilloscopeDecodedBuffer):
        allocate_buffer_api = self.scisdk_dll.SCISDK_AllocateBuffer
        allocate_buffer_api.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(OscilloscopeDecodedBuffer), ctypes.c_void_p]    
        allocate_buffer_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        # buf_ptr = OscilloscopeDecodedBuffer()
        # call C lib function
        print(buffer.info)
        err = allocate_buffer_api(ctypes.c_char_p(path_b), ctypes.c_int(buffer_type), ctypes.byref(buffer), self.lib_ptr)
        print(buffer.magic)
        return err