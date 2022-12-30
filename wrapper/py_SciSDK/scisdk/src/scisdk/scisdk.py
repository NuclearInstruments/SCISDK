import ctypes
import os
from scisdk.scisdk_defines import *


class SciSDK:
    scisdk_dll = None
    lib_ptr = None
    libname = None

    def __init__(self) -> None:
        #os.path.dirname(__file__) + "\\
        libname = ""

        if os.name == 'nt':
            self.libname = "SciSDK_DLL.dll"
        else:
            self.libname = "libscisdk.so"

        try:
            self.scisdk_dll = ctypes.CDLL(self.libname)
        except OSError as e:
            if e.errno == os.errno.ENOENT:
                print("Error: " + self.libname + " not found")
            elif e.errno == os.errno.EINVAL:
                print("Error: " + self.libname + " has the wrong architecture for the current process")
            elif e.errno == os.errno.ELIBBAD:
                print("Error:" + self.libname + " has missing dependencies")
            else:
                print("Error: unable to load libscisdk.so: " + str(e))
            



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

    def AllocateBuffer(self, path: str, size = None):
        buffer_pointer = None
        buffer_type = 0
        res, type = self.GetParameterString(path+".buffer_type")
        if(res == 0):
            if type == "SCISDK_OSCILLOSCOPE_DECODED_BUFFER":
                buffer_pointer = ctypes.POINTER(OscilloscopeDecodedBuffer)
                buffer_type = 1
            elif type == "SCISDK_OSCILLOSCOPE_RAW_BUFFER":
                buffer_pointer = ctypes.POINTER(OscilloscopeRawBuffer)
            elif type == "SCISDK_OSCILLOSCOPE_DUAL_DECODED_BUFFER":
                buffer_pointer = ctypes.POINTER(OscilloscopeDualDecodedBuffer)
                buffer_type = 1
            elif type == "SCISDK_OSCILLOSCOPE_DUAL_RAW_BUFFER":
                buffer_pointer = ctypes.POINTER(OscilloscopeDualRawBuffer)
            elif type == "SCISDK_DIGITIZER_DECODED_BUFFER":
                buffer_pointer = ctypes.POINTER(DigitizerDecodedBuffer)
                buffer_type = 1
            elif type == "SCISDK_LIST_RAW_BUFFER":
                buffer_pointer = ctypes.POINTER(ListRawBuffer)
            elif type == "SCISDK_CP_DECODED_BUFFER":
                buffer_pointer = ctypes.POINTER(CustomPacketDecodedBuffer)
                buffer_type = 1
            elif type == "SCISDK_CP_RAW_BUFFER":
                buffer_pointer = ctypes.POINTER(CustomPacketRawBuffer)
            elif type == "SCISDK_RM_RAW_BUFFER":
                buffer_pointer = ctypes.POINTER(RatemeterRawBuffer)
            elif type == "SCISDK_SPECTRUM_DECODED_BUFFER":
                buffer_pointer = ctypes.POINTER(SpectrumDecodedBuffer)
                buffer_type = 1
            elif type == "SCISDK_FFT_DECODED_BUFFER":
                buffer_pointer = ctypes.POINTER(FFTDecodedBuffer)
                buffer_type = 1
            elif type == "SCISDK_FFT_RAW_BUFFER":
                buffer_pointer = ctypes.POINTER(FFTRawBuffer)
            elif type == "SCISDK_FRAME_DECODED_BUFFER":
                buffer_pointer = ctypes.POINTER(FramePacketDecodedBuffer)
                buffer_type = 1
            elif type == "SCISDK_FRAME_RAW_BUFFER":
                buffer_pointer = ctypes.POINTER(FramePacketRawBuffer)
            elif type == "SCISDK_CITIROC_DECODED_BUFFER":
                buffer_pointer = ctypes.POINTER(CitirocDecodedBuffer)
                buffer_type = 1
            elif type == "SCISDK_CITIROC_RAW_BUFFER":
                buffer_pointer = ctypes.POINTER(CitirocRawBuffer)
            elif type == "SCISDK_PETIROC_DECODED_BUFFER":
                buffer_pointer = ctypes.POINTER(PetirocDecodedBuffer)
                buffer_type = 1
            elif type == "SCISDK_PETIROC_RAW_BUFFER":
                buffer_pointer = ctypes.POINTER(PetirocRawBuffer)      
            elif type == "SCISDK_FE_SCOPE_EVENT":
                buffer_pointer = ctypes.POINTER(FEScopePacket)
            elif type == "SCISDK_FE_OPENDPP_EVENT":
                buffer_pointer = ctypes.POINTER(FEOpenDppPacket)      
            else:
                raise Exception("Isn't a valid buffer type") 

            if size == None:
                allocate_buffer_api = self.scisdk_dll.SCISDK_AllocateBuffer
                allocate_buffer_api.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(buffer_pointer), ctypes.c_void_p]    
                allocate_buffer_api.restype = ctypes.c_int
                # convert string to bytes array
                path_b = path.encode('utf-8')
                # call C lib function
                buf_tmp = buffer_pointer()
                err = allocate_buffer_api(ctypes.c_char_p(path_b), ctypes.c_int(buffer_type), ctypes.byref(buf_tmp), self.lib_ptr)
                if err == 0:
                    buffer = buf_tmp.contents
                    return err, buffer   
                else:
                    return err, None
            else:
                allocate_buffer_api = self.scisdk_dll.SCISDK_AllocateBufferSize
                allocate_buffer_api.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(buffer_pointer), ctypes.c_void_p, ctypes.c_int]    
                allocate_buffer_api.restype = ctypes.c_int
                # convert string to bytes array
                path_b = path.encode('utf-8')
                # call C lib function
                buf_tmp = buffer_pointer()
                err = allocate_buffer_api(ctypes.c_char_p(path_b), ctypes.c_int(buffer_type), ctypes.byref(buf_tmp), self.lib_ptr, size)
                if err == 0:
                    buffer = buf_tmp.contents
                    return err, buffer
                else:
                    return err, None


    # read data
    def ReadData(self, path: str, buffer):
        read_data_api = self.scisdk_dll.SCISDK_ReadData
        if type(buffer) == OscilloscopeDecodedBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(OscilloscopeDecodedBuffer), ctypes.c_void_p]    
        elif type(buffer) == OscilloscopeRawBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(OscilloscopeRawBuffer), ctypes.c_void_p]    
        elif type(buffer) == OscilloscopeDualDecodedBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(OscilloscopeDualDecodedBuffer), ctypes.c_void_p]    
        elif type(buffer) == OscilloscopeDualRawBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(OscilloscopeDualRawBuffer), ctypes.c_void_p]    
        elif type(buffer) == DigitizerDecodedBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(DigitizerDecodedBuffer), ctypes.c_void_p]    
        elif type(buffer) == ListRawBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(ListRawBuffer), ctypes.c_void_p]    
        elif type(buffer) == CustomPacketDecodedBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(CustomPacketDecodedBuffer), ctypes.c_void_p]    
        elif type(buffer) == CustomPacketRawBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(CustomPacketRawBuffer), ctypes.c_void_p]    
        elif type(buffer) == RatemeterRawBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(RatemeterRawBuffer), ctypes.c_void_p]    
        elif type(buffer) == SpectrumDecodedBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(SpectrumDecodedBuffer), ctypes.c_void_p]    
        elif type(buffer) == FFTDecodedBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(FFTDecodedBuffer), ctypes.c_void_p]    
        elif type(buffer) == FFTRawBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(FFTRawBuffer), ctypes.c_void_p]  
        elif type(buffer) == FramePacketDecodedBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(FramePacketDecodedBuffer), ctypes.c_void_p]   
        elif type(buffer) == FramePacketRawBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(FramePacketRawBuffer), ctypes.c_void_p]      
        elif type(buffer) == CitirocDecodedBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(CitirocDecodedBuffer), ctypes.c_void_p]    
        elif type(buffer) == CitirocRawBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(CitirocRawBuffer), ctypes.c_void_p]    
        elif type(buffer) == PetirocDecodedBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(PetirocDecodedBuffer), ctypes.c_void_p]    
        elif type(buffer) == PetirocRawBuffer:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(PetirocRawBuffer), ctypes.c_void_p]    
        elif type(buffer) == FEScopePacket:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(FEScopePacket), ctypes.c_void_p]    
        elif type(buffer) == FEOpenDppPacket:
            read_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(FEOpenDppPacket), ctypes.c_void_p]    
        else:
            raise Exception(type(buffer).__name__ + " isn't a valid buffer type") 

        read_data_api = self.scisdk_dll.SCISDK_ReadData
        read_data_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        err = read_data_api(ctypes.c_char_p(path_b), ctypes.byref(buffer), self.lib_ptr)
        return err, buffer

    def FreeBuffer(self, path: str, buffer):
        free_buffer_api = self.scisdk_dll.SCISDK_FreeBuffer
        buffer_pointer = None
        buffer_type = 0
        if type(buffer) == OscilloscopeDecodedBuffer:
            buffer_pointer = ctypes.POINTER(OscilloscopeDecodedBuffer)
            buffer_type = 1
        elif type(buffer) == OscilloscopeRawBuffer:
            buffer_pointer = ctypes.POINTER(OscilloscopeRawBuffer)
        elif type(buffer) == OscilloscopeDualDecodedBuffer:
            buffer_pointer = ctypes.POINTER(OscilloscopeDualDecodedBuffer)
            buffer_type = 1
        elif type(buffer) == OscilloscopeDualRawBuffer:
            buffer_pointer = ctypes.POINTER(OscilloscopeDualRawBuffer)
        elif type(buffer) == DigitizerDecodedBuffer:
            buffer_pointer = ctypes.POINTER(DigitizerDecodedBuffer)
            buffer_type = 1
        elif type(buffer) == ListRawBuffer:
            buffer_pointer = ctypes.POINTER(ListRawBuffer)
        elif type(buffer) == CustomPacketDecodedBuffer:
            buffer_pointer = ctypes.POINTER(CustomPacketDecodedBuffer)
            buffer_type = 1
        elif type(buffer) == CustomPacketRawBuffer:
            buffer_pointer = ctypes.POINTER(CustomPacketRawBuffer)
        elif type(buffer) == RatemeterRawBuffer:
            buffer_pointer = ctypes.POINTER(RatemeterRawBuffer)
        elif type(buffer) == SpectrumDecodedBuffer:
            buffer_pointer = ctypes.POINTER(SpectrumDecodedBuffer)
            buffer_type = 1
        elif type(buffer) == FFTDecodedBuffer:
            buffer_pointer = ctypes.POINTER(FFTDecodedBuffer)
            buffer_type = 1
        elif type(buffer) == FFTRawBuffer:
            buffer_pointer = ctypes.POINTER(FFTRawBuffer)
        elif type(buffer) == FramePacketDecodedBuffer:
            buffer_pointer = ctypes.POINTER(FramePacketDecodedBuffer)
            buffer_type = 1
        elif type(buffer) == FramePacketRawBuffer:
            buffer_pointer = ctypes.POINTER(FramePacketRawBuffer)
        elif type(buffer) == CitirocDecodedBuffer:
            buffer_pointer = ctypes.POINTER(CitirocDecodedBuffer)
            buffer_type = 1
        elif type(buffer) == CitirocRawBuffer:
            buffer_pointer = ctypes.POINTER(CitirocRawBuffer)
        elif type(buffer) == PetirocDecodedBuffer:
            buffer_pointer = ctypes.POINTER(PetirocDecodedBuffer)
            buffer_type = 1
        elif type(buffer) == PetirocRawBuffer:
            buffer_pointer = ctypes.POINTER(PetirocRawBuffer)
        elif type(buffer) == FEScopePacket:
            buffer_pointer = ctypes.POINTER(FEScopePacket)
        elif type(buffer) == FEOpenDppPacket:
            buffer_pointer = ctypes.POINTER(FEOpenDppPacket)
        else:
            raise Exception(type(buffer).__name__ + " isn't a valid buffer type") 

        free_buffer_api.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(buffer_pointer), ctypes.c_void_p]    
        free_buffer_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        # call C lib function
        buf_tmp = buffer_pointer(buffer)
        err = free_buffer_api(ctypes.c_char_p(path_b), ctypes.c_int(buffer_type), ctypes.byref(buf_tmp), self.lib_ptr)
        return err

    def ExecuteCommand(self, path: str, value: str):
        execute_command_api = self.scisdk_dll.SCISDK_ExecuteCommand
        execute_command_api.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_void_p]    
        execute_command_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        value_b = value.encode('utf-8')
        # call C lib function
        err = execute_command_api(ctypes.c_char_p(path_b), ctypes.c_char_p(value_b), self.lib_ptr)
        return err

    def DecodeData(self, path: str, buffer_in: OscilloscopeRawBuffer):
        decode_data_api = self.scisdk_dll.SCISDK_DecodeData
        decode_data_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(OscilloscopeRawBuffer), ctypes.POINTER(OscilloscopeDecodedBuffer), ctypes.c_void_p]
        decode_data_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        # call C lib function
        buffer_out = OscilloscopeDecodedBuffer()
        err = decode_data_api(ctypes.c_char_p(path_b), ctypes.byref(buffer_in), ctypes.byref(buffer_out), self.lib_ptr)
        return err, buffer_out

    def SetRegister(self, path: str, value: int):
        set_register_api = self.scisdk_dll.SCISDK_SetRegister
        set_register_api.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_void_p]
        set_register_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        # call C lib function
        err = set_register_api(ctypes.c_char_p(path_b), value, self.lib_ptr)
        return err

    def GetRegister(self, path: str):
        get_register_api = self.scisdk_dll.SCISDK_GetRegister
        get_register_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_int), ctypes.c_void_p]
        get_register_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        ret_int = ctypes.c_int()
        # call C lib function
        err = get_register_api(ctypes.c_char_p(path_b), ctypes.byref(ret_int), self.lib_ptr)
        return err, ret_int.value

    def GetAllParameters(self, path: str):
        get_all_parameters_api = self.scisdk_dll.SCISDK_GetAllParameters
        get_all_parameters_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_char_p), ctypes.c_void_p]
        get_all_parameters_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        ret_char_p = ctypes.c_char_p()
        # call C lib function
        err = get_all_parameters_api(ctypes.c_char_p(path_b), ctypes.byref(ret_char_p), self.lib_ptr)
        return err, ret_char_p.value.decode('utf-8')

    def GetParameterDescription(self, path: str):
        get_parameter_description_api = self.scisdk_dll.SCISDK_GetParameterDescription
        get_parameter_description_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_char_p), ctypes.c_void_p]
        get_parameter_description_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        ret_char_p = ctypes.c_char_p()
        # call C lib function
        err = get_parameter_description_api(ctypes.c_char_p(path_b), ctypes.byref(ret_char_p), self.lib_ptr)
        return err, ret_char_p.value.decode('utf-8')

    def GetParameterListOfValues(self, path: str):
        get_parameter_list_of_values_api = self.scisdk_dll.SCISDK_GetParameterListOfValues
        get_parameter_list_of_values_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_char_p), ctypes.c_void_p]
        get_parameter_list_of_values_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        ret_char_p = ctypes.c_char_p()
        # call C lib function
        err = get_parameter_list_of_values_api(ctypes.c_char_p(path_b), ctypes.byref(ret_char_p), self.lib_ptr)
        return err, ret_char_p.value.decode('utf-8')

    def GetParameterMinimumValue(self, path: str):
        get_parameters_min_value_api = self.scisdk_dll.SCISDK_GetParameterMinimumValue
        get_parameters_min_value_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.c_void_p]
        get_parameters_min_value_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        ret_double_p = ctypes.c_double()
        # call C lib function
        err = get_parameters_min_value_api(ctypes.c_char_p(path_b), ctypes.byref(ret_double_p), self.lib_ptr)
        return err, ret_double_p.value

    def GetParameterMaximumValue(self, path: str):
        get_parameters_max_value_api = self.scisdk_dll.SCISDK_GetParameterMaximumValue
        get_parameters_max_value_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.c_void_p]
        get_parameters_max_value_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        ret_double_p = ctypes.c_double()
        # call C lib function
        err = get_parameters_max_value_api(ctypes.c_char_p(path_b), ctypes.byref(ret_double_p), self.lib_ptr)
        return err, ret_double_p.value

    def GetParametersProperties(self, path: str):
        get_parameters_properties_api = self.scisdk_dll.SCISDK_GetParametersProperties
        get_parameters_properties_api.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_char_p), ctypes.c_void_p]
        get_parameters_properties_api.restype = ctypes.c_int
        # convert string to bytes array
        path_b = path.encode('utf-8')
        ret_char_p = ctypes.c_char_p()
        # call C lib function
        err = get_parameters_properties_api(ctypes.c_char_p(path_b), ctypes.byref(ret_char_p), self.lib_ptr)
        return err, ret_char_p.value.decode('utf-8')

    def FreeLib(self):
        free_lib_api = self.scisdk_dll.SCISDK_FreeLib
        free_lib_api.argtypes = [ctypes.c_void_p]
        free_lib_api.restype = ctypes.c_int
        # call C lib function
        err = free_lib_api(self.lib_ptr)
        return err