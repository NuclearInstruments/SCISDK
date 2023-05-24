% Load library if it isn't load
if ~libisloaded("SciSDK_DLL")
    [notfound, warnings] = loadlibrary('SciSDK_DLL.dll', "../../../../headers/matlab/Matlab_SciSDK_DLL.h", 'addheader', "Matlab_scisdk_defines_flat.h");
end

% Detach the boards that are alredy attached from the previous execution of 
% the program. Change the following lines of code to detach the boards
% that have been attached previously. Also, remember to change the name
% of the scisdk handle inside `evalin` function with the name of the
% handle that you use in your program.
if exist("sdk_handle", "var")
    old_handle = evalin("base", "sdk_handle");
    calllib('SciSDK_DLL', 'SCISDK_DetachDevice', 'board0', old_handle);
    clearvars
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
    calllib('SciSDK_DLL', 'SCISDK_SetParameterString', 'board0:/MMCComponents/List_0.thread', 'false', sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/List_0.timeout', 500, sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_SetParameterString', 'board0:/MMCComponents/List_0.acq_mode', 'blocking', sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_ExecuteCommand', 'board0:/MMCComponents/List_0.stop', '', sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_ExecuteCommand', 'board0:/MMCComponents/List_0.start', '', sdk_handle);

    buffer_ptr = libpointer('SCISDK_LIST_RAW_BUFFERPtrPtr');
    res = calllib('SciSDK_DLL', 'SCISDK_AllocateBufferSize',  'board0:/MMCComponents/List_0', 0, buffer_ptr, sdk_handle, 1024);

    if res ==0
        % buffer has been allocated so read data
        run = true;
        res = calllib('SciSDK_DLL', 'SCISDK_ReadData', 'board0:/MMCComponents/List_0',buffer_ptr,sdk_handle);
        if res == 0
            setdatatype(buffer_ptr.Value.data, 'int8Ptr',  uint32(buffer_ptr.Value.valid_samples));

            for i = 0:+1:(uint32(buffer_ptr.Value.valid_samples)/8)-1
                chunk = buffer_ptr.Value.data((i*8)+1:(i+1)*8);
                ch1 = typecast(chunk(1:2), 'uint16');
                ch0 = typecast(chunk(3:4), 'uint16');
                counter = typecast(chunk(5:8), 'uint32');
                fprintf(string(ch1) + ", " + string(ch0) + ", " + string(counter) + "\n");
            end
        else
            % print error message
            calllib("SciSDK_DLL", "SCISDK_s_error", res, ret_string, sdk_handle);
            fprintf("Cannot read data: " + ret_string.Value{1,1} + "\n");
        end
    else
        fprintf("Cannot allocate list buffer");
    end
   
    % Detach device
    if calllib('SciSDK_DLL', 'SCISDK_DetachDevice', 'board0', sdk_handle) == 0
        fprintf('Board successfully detached\n');
    end
else
    fprintf('Program exit due connection error\n');
end

% Free up vars' memory
clearvars

% Unload dll
unloadlibrary("SciSDK_DLL");