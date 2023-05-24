% Load library if it isn't load
if ~libisloaded("SciSDK_DLL")
    [notfound, warnings] = loadlibrary('SciSDK_DLL.dll', 'C:\git\scisdk\headers\matlab\Matlab_SciSDK_DLL.h', 'addheader', 'C:\git\scisdk\headers\matlab\Matlab_scisdk_defines_flat.h');
end

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

% print version of SciSDK
ret_string = libpointer('stringPtrPtr', {''});
calllib("SciSDK_DLL", "SCISDK_GetLibraryVersion", ret_string, sdk_handle);
fprintf("SciSDK version is: " + ret_string.Value{1,1} + "\n\n")

% attach device
res = calllib('SciSDK_DLL', 'SCISDK_AddNewDevice', 'usb:28645', 'dt1260', './DT1260RegisterFile.json', 'board0', sdk_handle);

if res == 0
    % Set register A value
    calllib('SciSDK_DLL', 'SCISDK_SetRegister', 'board0:/Registers/A', 100, sdk_handle);
    % Set register B value
    calllib('SciSDK_DLL', 'SCISDK_SetRegister', 'board0:/Registers/B', 151, sdk_handle);
    
    % Get register C value and print in console
    register_value = libpointer("uint32Ptr", 1);
    calllib('SciSDK_DLL', 'SCISDK_GetRegister', 'board0:/Registers/C', register_value, sdk_handle);
    fprintf(strcat('Register C value is:',  string(register_value.Value), '\n'));
    fprintf('\n');

    % Write and readback channel registers (from 0 to 5)
    for i = 0:+1:5
        calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger',  convertStringsToChars(strcat('board0:/MMCComponents/CREG_0.CH', string(i))),i+10, sdk_handle);
    end
   
    for i = 0:+1:5
        register_value = libpointer("int32Ptr", 1);
        res = calllib('SciSDK_DLL', 'SCISDK_GetParameterInteger',  convertStringsToChars(strcat('board0:/MMCComponents/CREG_1.CH', string(i))),register_value, sdk_handle);
        fprintf('Register CREG_0.CH' + string(i) + " value is " + register_value.Value)
        fprintf('\n')
    end
    fprintf('\n');

    % Set register file REG_0...3 and readback 5..6th registers
    calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/REGFILE_0.REG_0', 10, sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/REGFILE_0.REG_1', 20, sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/REGFILE_0.REG_2', 50, sdk_handle);
    calllib('SciSDK_DLL', 'SCISDK_SetParameterInteger', 'board0:/MMCComponents/REGFILE_0.REG_3', 5, sdk_handle);

    for i = 0:+1:3
        register_value = libpointer("int32Ptr", 1);
        res = calllib('SciSDK_DLL', 'SCISDK_GetParameterInteger',  convertStringsToChars(strcat('board0:/MMCComponents/REGFILE_0.REG_', string(i+4))),register_value, sdk_handle);
        if res == 0
            fprintf('Register REGFILE_' + string(i) + " value is " + register_value.Value);
            fprintf('\n');
        end
    end
    fprintf('\n');

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