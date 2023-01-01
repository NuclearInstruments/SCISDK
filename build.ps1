MSBuild SciSDK_VC++\SciSDK_VC++.sln -t:Rebuild -p:Configuration=Release -p:Platform=x64
MSBuild SciSDK_VC++\SciSDK_VC++.sln -t:Rebuild -p:Configuration=Debug -p:Platform=x64
MSBuild SciSDK_VC++\SciSDK_VC++.sln -t:Rebuild -p:Configuration=Release -p:Platform=x86
MSBuild SciSDK_VC++\SciSDK_VC++.sln -t:Rebuild -p:Configuration=Debug -p:Platform=x86
New-Item -Path "." -Name "output" -ItemType "directory" -Force