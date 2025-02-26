
python -m nirelease --pull -u DT1260/rev-A/release/sw-lib/DT1260SDK/ -o tmp
Expand-Archive -LiteralPath ./tmp/DT1260SDK/windows_x86_x64.zip -DestinationPath ./tmp/DT1260SDK/ -Force
Move-Item -Path ./tmp/DT1260SDK/windows/ -Destination ./dt1260 -force
python -m nirelease --pull -u X5560/rev-B/release/sw-lib/X5560SDK/ -o tmp
Expand-Archive -LiteralPath ./tmp/X5560SDK/windows_x86_x64.zip -DestinationPath ./tmp/X5560SDK/ -Force
Move-Item -Path ./tmp/X5560SDK/windows/ -Destination ./x5560 -force
python -m nirelease --pull -u DT5771/rev-A/release/sw-lib/dt5771sdk-windows/ -o tmp
Expand-Archive -LiteralPath ./tmp/dt5771sdk-windows/windows_x86_x64.zip -DestinationPath ./tmp/dt5771sdk-windows/ -Force
Move-Item -Path ./tmp/dt5771sdk-windows/windows/ -Destination ./dt5771 -force
python -m nirelease --pull -u SciSDK/rev-A/sub/sw-docs/SciSDKDocs/ -o tmp
Expand-Archive -LiteralPath ./tmp/SciSDKDocs/scisdk_docs.zip -DestinationPath ./scisdk/ -Force
New-Item -Path "." -Name "redist" -ItemType "directory" -Force
Invoke-WebRequest http://installers.lanni/windows_sw/vc_redist/VC_redist.x64.exe -OutFile redist/VC_redist.x64.exe 
Invoke-WebRequest http://installers.lanni/windows_sw/vc_redist/VC_redist.x86.exe -OutFile redist/VC_redist.x86.exe	
python -m update_caen.py 
#Invoke-WebRequest http://installers.lanni/windows_sw/caen_felib/felib_2022_12_1.zip -OutFile tmp/felib_2022_12_1.zip 
#Expand-Archive -LiteralPath ./tmp/felib_2022_12_1.zip -DestinationPath ./ -Force
New-Item -Path "." -Name "Drivers" -ItemType "directory" -Force
Invoke-WebRequest http://installers.lanni/windows_sw/ftdi_driver/d2xx_setup.exe -OutFile Drivers/d2xx_setup.exe
Invoke-WebRequest http://installers.lanni/windows_sw/ftdi_driver/d3xx_setup.exe -OutFile Drivers/d3xx_setup.exe
python -m nirelease --pull -u SciSDK/rev-A/release/sw-windows-lib/scisdk-full-win-x86_x64/ -o tmp
Expand-Archive -LiteralPath ./tmp/scisdk-full-win-x86_x64/scisdk-win-full.zip -DestinationPath ./scisdk/ -Force

$param1=$args[0]

iscc /Qp /DMyAppVersion=$param1 scisdk.iss
