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
    calllib('SciSDK_DLL', 'SCISDK_SetParameterString', 'board0:/MMCComponents/Oscilloscope_0.data_processing', 'decode', sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/Oscilloscope_0.trigger_level', 1500, sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_SetParameterString', 'board0:/MMCComponents/Oscilloscope_0.trigger_mode', 'self', sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/Oscilloscope_0.trigger_channel', 0, sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/Oscilloscope_0.pretrigger', 150, sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/Oscilloscope_0.decimator', 0, sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_SetParameterString', 'board0:/MMCComponents/Oscilloscope_0.acq_mode', 'blocking', sdk_handle);

    % Allocate buffer
    buffer_ptr = libpointer('SCISDK_OSCILLOSCOPE_DECODED_BUFFERPtrPtr');
    res = calllib('SciSDK_DLL', 'SCISDK_AllocateBuffer',  'board0:/MMCComponents/Oscilloscope_0', 1, buffer_ptr, sdk_handle);

    if res == 0
        fprintf("Buffer's magic number is " + string(uint32(buffer_ptr.Value.magic)));
        res = calllib('SciSDK_DLL', 'SCISDK_ReadData',  'board0:/MMCComponents/Oscilloscope_0', buffer_ptr, sdk_handle);
        if res == 0
            % Set analog data type
            setdatatype(buffer_ptr.Value.analog, 'int32Ptr', uint32(buffer_ptr.Value.samples_analog) * uint32(buffer_ptr.Value.channels));
            % Write analog data to file
            writetable(array2table(buffer_ptr.Value.analog),'analog.xlsx');
            
            % Set digital data type
            setdatatype(buffer_ptr.Value.digital, 'uint8Ptr', uint32(buffer_ptr.Value.samples_digital) * uint32(buffer_ptr.Value.channels));
            % Write digital data to file
            writetable(array2table(buffer_ptr.Value.digital),'digital.xlsx');
            
            subplot(2,1,1);
            plot(buffer_ptr.Value.analog(1:buffer_ptr.Value.samples_digital));
            title("Channel 0");
            
            subplot(2,1,2);
            plot(buffer_ptr.Value.analog(buffer_ptr.Value.samples_digital+1:buffer_ptr.Value.samples_digital*2));
            title("Channel 1");

            % Print success message
            fprintf("");
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
    fprintf('Program exit due connection error\n');
end

% Free up vars' memory
clearvars

% Unload dll
unloadlibrary("SciSDK_DLL");