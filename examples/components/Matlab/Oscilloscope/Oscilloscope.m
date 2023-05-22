% load library
%if ~libisloaded('SciSDK_DLL')
    loadlibrary('SciSDK_DLL.dll', 'C:\git\scisdk\src\SciSDK_DLL.h', 'addheader', 'C:\git\scisdk\src\scisdk_defines.h');
%end

% initialize library
sdk_handle = calllib("SciSDK_DLL", "SCISDK_InitLib");
calllib("SciSDK_DLL", "SCISDK_InitLib");

% print version of SciSDK
ret_string = libpointer('stringPtrPtr', {''});
calllib("SciSDK_DLL", "SCISDK_GetLibraryVersion", ret_string, sdk_handle);
fprintf("SciSDK version is: " + ret_string.Value{1,1} + "\n\n")

% attach device
res = calllib('SciSDK_DLL', 'SCISDK_AddNewDevice', 'usb:28645', 'dt1260', './DT1260RegisterFile.json', 'board0', sdk_handle);

if res == 0
    % set oscilloscope parameters
    res = calllib('SciSDK_DLL', 'SCISDK_SetParameterString', 'board0:/MMCComponents/Oscilloscope_0.data_processing', 'decode', sdk_handle);
    res = calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/Oscilloscope_0.trigger_level', 1500, sdk_handle);
    res = calllib('SciSDK_DLL', 'SCISDK_SetParameterString', 'board0:/MMCComponents/Oscilloscope_0.trigger_mode', 'self', sdk_handle);
    res = calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/Oscilloscope_0.trigger_channel', 0, sdk_handle);
    res = calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/Oscilloscope_0.pretrigger', 150, sdk_handle);
    res = calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/Oscilloscope_0.decimator', 0, sdk_handle);
    res = calllib('SciSDK_DLL', 'SCISDK_SetParameterString', 'board0:/MMCComponents/Oscilloscope_0.acq_mode', 'blocking', sdk_handle);

    % allocate buffer
    oscilloscope_buffer = struct('magic', 'uint32');
    buffer_ptr = libpointer('voidPtr', 0);
    
    % libstruct('SCISDK_SPECTRUM_STATUS')
    % libstruct('SCISDK_OSCILLOSCOPE_DECODED_BUFFER')
    
    res = calllib('SciSDK_DLL', 'SCISDK_AllocateBuffer',  'board0:/MMCComponents/Oscilloscope_0', 1, buffer_ptr, sdk_handle);
    buffer_ptr.Value
    if res == 0
        res = calllib('SciSDK_DLL', 'SCISDK_ReadData',  'board0:/MMCComponents/Oscilloscope_0', buffer_ptr, sdk_handle);
        if res == 0
            buffer_ptr.Value;
            fprintf('OK');
        else
            fprintf('Cannot read oscilloscope data ' + string(res));
        end
    else
        fprintf('Cannot allocate oscilloscope buffer');
    end
    fprintf("\n");
    
    % Detach device
    if calllib('SciSDK_DLL','SCISDK_DetachDevice', 'board0', sdk_handle) == 0
        fprintf('Board successfully detached\n');
    end
else
    fprintf('Program exit due connection error');
end

% free library
calllib("SciSDK_DLL", "SCISDK_FreeLib", sdk_handle);
unloadlibrary("SciSDK_DLL");