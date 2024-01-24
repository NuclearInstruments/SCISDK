# Controlla se è stato fornito almeno un argomento
if ($args.Count -lt 1) {
    Write-Host "Usage: ./script.ps1 <version>"
    exit 1
}

# Ottieni la versione dall'argomento
$version = $args[0]

# Costruisci il contenuto da scrivere nel file src/scisdk_version.h
$versionContent = @"
#ifndef __SCISDK_VERSION_H
#define __SCISDK_VERSION_H

#define SCISDK_VERSION "$version"
#endif
"@

# Scrivi il contenuto nel file src/scisdk_version.h
$versionContent | Set-Content -Path "src/scisdk_version.h" -Force

Write-Host "Veersion updated in src/scisdk_version.h a $version"


MSBuild SciSDK_VC++\SciSDK_VC++.sln -t:Rebuild -p:Configuration=Release -p:Platform=x64
MSBuild SciSDK_VC++\SciSDK_VC++.sln -t:Rebuild -p:Configuration=Debug -p:Platform=x64
MSBuild SciSDK_VC++\SciSDK_VC++.sln -t:Rebuild -p:Configuration=Release -p:Platform=x86
MSBuild SciSDK_VC++\SciSDK_VC++.sln -t:Rebuild -p:Configuration=Debug -p:Platform=x86
MSBuild wrapper\CSharp_SciSDK\CSharp_SciSDK.sln -t:Rebuild -p:Configuration=Release -p:Platform="Any CPU"
MSBuild wrapper\CSharp_SciSDK\CSharp_SciSDK.sln -t:Rebuild -p:Configuration=Debug -p:Platform="Any CPU"

New-Item -Path "." -Name "output" -ItemType "directory" -Force
Copy-Item ".\bin" -Destination ".\output\" -Recurse -Force -Exclude *.pdb,*.exp
New-Item -Path "." -Name "output\src" -ItemType "directory" -Force
Copy-Item ".\src\NIErrorCode.h" -Destination ".\output\src\" -Force
Copy-Item ".\src\SciSDK_DLL.h" -Destination ".\output\src\" -Force
Copy-Item ".\src\scisdk_defines.h" -Destination ".\output\src\" -Force
Copy-Item ".\examples" -Destination ".\output\examples\" -Force -Recurse
Copy-Item ".\wrapper" -Destination ".\output\wrapper" -Force -Recurse

Compress-Archive -Path ".\output\*" -DestinationPath ".\output\scisdk-full.zip" -Force