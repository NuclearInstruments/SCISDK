% Load library if it isn't load
if ~libisloaded("SciSDK_DLL")
    [notfound, warnings] = loadlibrary('SciSDK_DLL.dll', 'C:\git\scisdk\headers\matlab\Matlab_SciSDK_DLL.h', 'addheader', 'C:\git\scisdk\headers\matlab\Matlab_scisdk_defines_flat.h');
end
%fprintf("PATH: " +  mfilename('fullpath'))
%addpath('..\..\..\headers\matlab\')
%fprintf("\n")

% Detach the boards that are alredy attached from the previous execution of 
% the program. Change the following lines of code to detach the boards
% that have been attached previously. Also, remember to change the name
% of the scisdk handle inside `evalin` function with the name of the
% handle that you use in your program.
if exist("sdk_handle", "var")
    old_handle = evalin("base", "sdk_handle");
    calllib('SciSDK_DLL', 'SCISDK_DetachDevice', 'board0', old_handle);
    calllib('SciSDK_DLL', 'SCISDK_FreeLib', old_handle);
    clear("buffer_ptr");
    clear("old_handle");
    clear("sdk_handle");
end
    
% Initialize library
sdk_handle = calllib("SciSDK_DLL", "SCISDK_InitLib");

% Print version of SciSDK library
ret_string = libpointer('stringPtrPtr', {''});
calllib("SciSDK_DLL", "SCISDK_GetLibraryVersion", ret_string, sdk_handle);
fprintf("SciSDK version is: " + ret_string.Value{1,1} + "\n\n");

% Attach device
res = calllib('SciSDK_DLL', 'SCISDK_AddNewDevice', 'usb:28645', 'dt1260', './DT1260RegisterFile.json', 'board0', sdk_handle);

if res == 0
    % Set oscilloscope parameters
    calllib('SciSDK_DLL', 'SCISDK_SetParameterString', 'board0:/MMCComponents/Digitizer_0.data_processing', 'decode', sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/Digitizer_0.enabledch', 1, sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/Digitizer_0.acq_len', 8000, sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_SetParameterString', 'board0:/MMCComponents/Digitizer_0.acq_mode', 'blocking', sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/Digitizer_0.timeout', 2000, sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_ExecuteCommand', 'board0:/MMCComponents/Digitizer_0.start', '', sdk_handle);

    % Allocate buffer
    oscilloscope_buffer = struct('magic', 'uint32');
    buffer_ptr = libpointer('SCISDK_DIGITIZER_DECODED_BUFFERPtrPtr');

    res = calllib('SciSDK_DLL', 'SCISDK_AllocateBuffer',  'board0:/MMCComponents/Digitizer_0', 1, buffer_ptr, sdk_handle);
    has_datatype_been_set = false;
    if res == 0
        fprintf("Digitizer buffer has been allocated successfully, program is starting to read digitizer data\n");
        while true
            res = calllib('SciSDK_DLL', 'SCISDK_ReadData',  'board0:/MMCComponents/Digitizer_0', buffer_ptr, sdk_handle);
            if res == 0
                % Set analog data type
                if ~has_datatype_been_set
                    setdatatype(buffer_ptr.Value.analog, 'int32Ptr', uint32(buffer_ptr.Value.valid_samples));
                    has_datatype_been_set = true;
                end
                % Plot data
                plot(buffer_ptr.Value.analog)
            end
            pause(0.2)
        end
    else
        fprintf('Cannot allocate oscilloscope buffer');
    end
    fprintf("\n");
        
    % Detach device
    if calllib('SciSDK_DLL', 'SCISDK_DetachDevice', 'board0', sdk_handle) == 0
        fprintf('Board successfully detached\n');
    end
else
    fprintf('Program exit due connection error\n');
end

% Free library
calllib("SciSDK_DLL", "SCISDK_FreeLib", sdk_handle);

% Free buffer memory
clear("buffer_ptr");

% Unload dll
unloadlibrary("SciSDK_DLL");