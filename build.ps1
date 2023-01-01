MSBuild SciSDK_VC++\SciSDK_VC++.sln -t:Rebuild -p:Configuration=Release -p:Platform=x64
MSBuild SciSDK_VC++\SciSDK_VC++.sln -t:Rebuild -p:Configuration=Debug -p:Platform=x64
MSBuild SciSDK_VC++\SciSDK_VC++.sln -t:Rebuild -p:Configuration=Release -p:Platform=x86
MSBuild SciSDK_VC++\SciSDK_VC++.sln -t:Rebuild -p:Configuration=Debug -p:Platform=x86
New-Item -Path "." -Name "output" -ItemType "directory" -Force
Copy-Item ".\bin" -Destination ".\output\" -Recurse -Force -Exclude *.pdb,*.exe,*.exp
New-Item -Path "." -Name "output\src" -ItemType "directory" -Force
Copy-Item ".\src\NIErrorCode.h" -Destination ".\output\src\" -Force
Copy-Item ".\src\SciSDK_DLL.h" -Destination ".\output\src\" -Force
Copy-Item ".\src\scisdk_defines.h" -Destination ".\output\src\" -Force
Copy-Item ".\examples" -Destination ".\output\examples\" -Force -Recurse

Compress-Archive -Path ".\output\*" -DestinationPath ".\output\scisdk-full.zip" -Force