
; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "SciSDK"
//#define MyAppVersion "2022.12.0.1"
#define MyAppPublisher "Nuclear Instruments"
#define MyAppURL "https://github.com/NuclearInstruments/SCISDK"


[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{990DB01D-A138-4A12-BFA1-1DB681063D42}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName=C:\OpenHardware\{#MyAppName}
ChangesAssociations=yes
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputDir=.\output
OutputBaseFilename=SciSDKSetup
Compression=lzma
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin 
DisableWelcomePage=no
DisableDirPage=no
UsePreviousAppDir=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"



[Files]
Source: "scisdk\bin\x64\Release\SciSDK_DLL.dll"; DestDir: "{app}\bin\x64\Release\"; Flags: ignoreversion recursesubdirs
Source: "scisdk\bin\Win32\Release\SciSDK_DLL.dll"; DestDir: "{app}\bin\Win32\Release\"; Flags: ignoreversion recursesubdirs
Source: "scisdk\bin\x64\Release\SciSDK_DLL.lib"; DestDir: "{app}\bin\x64\Release\"; Flags: ignoreversion recursesubdirs
Source: "scisdk\bin\Win32\Release\SciSDK_DLL.lib"; DestDir: "{app}\bin\Win32\Release\"; Flags: ignoreversion recursesubdirs
Source: "scisdk\bin\x64\Debug\SciSDK_DLL.dll"; DestDir: "{app}\bin\x64\Debug\"; Flags: ignoreversion recursesubdirs
Source: "scisdk\bin\Win32\Debug\SciSDK_DLL.dll"; DestDir: "{app}\bin\Win32\Debug\"; Flags: ignoreversion recursesubdirs
Source: "scisdk\bin\x64\Debug\SciSDK_DLL.lib"; DestDir: "{app}\bin\x64\Debug\"; Flags: ignoreversion recursesubdirs
Source: "scisdk\bin\Win32\Debug\SciSDK_DLL.lib"; DestDir: "{app}\bin\Win32\Debug\"; Flags: ignoreversion recursesubdirs
Source: "dt1260\x64\Release\SCIDK_Lib.dll"; DestDir: "{app}\bin\x64\Release\"; Flags: ignoreversion recursesubdirs
Source: "dt1260\Win32\Release\SCIDK_Lib.dll"; DestDir: "{app}\bin\Win32\Release\"; Flags: ignoreversion recursesubdirs
Source: "dt1260\x64\Debug\SCIDK_Lib.dll"; DestDir: "{app}\bin\x64\Debug\"; Flags: ignoreversion recursesubdirs
Source: "dt1260\Win32\Debug\SCIDK_Lib.dll"; DestDir: "{app}\bin\Win32\Debug\"; Flags: ignoreversion recursesubdirs
Source: "x5560\x64\R5560_SDKLib.dll"; DestDir: "{app}\bin\x64\Release\"; Flags: ignoreversion recursesubdirs
Source: "x5560\x64\libsodium.dll"; DestDir: "{app}\bin\x64\Release\"; Flags: ignoreversion recursesubdirs
Source: "x5560\x64\libzmq-v140-mt-4_3_4.dll"; DestDir: "{app}\bin\x64\Release\"; Flags: ignoreversion recursesubdirs
Source: "x5560\x86\R5560_SDKLib.dll"; DestDir: "{app}\bin\Win32\Release\"; Flags: ignoreversion recursesubdirs
Source: "x5560\x86\libsodium.dll"; DestDir: "{app}\bin\Win32\Release\"; Flags: ignoreversion recursesubdirs
Source: "x5560\x86\libzmq-v140-mt-4_3_4.dll"; DestDir: "{app}\bin\Win32\Release\"; Flags: ignoreversion recursesubdirs
Source: "x5560\x64d\R5560_SDKLib.dll"; DestDir: "{app}\bin\x64\Debug\"; Flags: ignoreversion recursesubdirs
Source: "x5560\x64d\libsodium.dll"; DestDir: "{app}\bin\x64\Debug\"; Flags: ignoreversion recursesubdirs
Source: "x5560\x64d\libzmq-v140-mt-4_3_4.dll"; DestDir: "{app}\bin\x64\Debug\"; Flags: ignoreversion recursesubdirs
Source: "x5560\x86d\R5560_SDKLib.dll"; DestDir: "{app}\bin\Win32\Debug\"; Flags: ignoreversion recursesubdirs
Source: "x5560\x86d\libsodium.dll"; DestDir: "{app}\bin\Win32\Debug\"; Flags: ignoreversion recursesubdirs
Source: "x5560\x86d\libzmq-v140-mt-4_3_4.dll"; DestDir: "{app}\bin\Win32\Debug\"; Flags: ignoreversion recursesubdirs
Source: "CAEN Dig2\x86_64\CAEN_Dig2Lib.dll"; DestDir: "{app}\bin\x64\Release\"; Flags: ignoreversion recursesubdirs
Source: "CAEN FELib\x86_64\CAEN_FELib.dll"; DestDir: "{app}\bin\x64\Release\"; Flags: ignoreversion recursesubdirs
Source: "CAEN Dig2\x86\CAEN_Dig2Lib.dll"; DestDir: "{app}\bin\Win32\Release\"; Flags: ignoreversion recursesubdirs
Source: "CAEN FELib\x86\CAEN_FELib.dll"; DestDir: "{app}\bin\Win32\Release\"; Flags: ignoreversion recursesubdirs
Source: "CAEN Dig2\x86_64\CAEN_Dig2Lib.dll"; DestDir: "{app}\bin\x64\Debug\"; Flags: ignoreversion recursesubdirs
Source: "CAEN FELib\x86_64\CAEN_FELib.dll"; DestDir: "{app}\bin\x64\Debug\"; Flags: ignoreversion recursesubdirs
Source: "CAEN Dig2\x86\CAEN_Dig2Lib.dll"; DestDir: "{app}\bin\Win32\Debug\"; Flags:  recursesubdirs
Source: "CAEN FELib\x86\CAEN_FELib.dll"; DestDir: "{app}\bin\Win32\Debug\"; Flags: ignoreversion recursesubdirs
Source: "scisdk\src\NIErrorCode.h"; DestDir: "{app}\src\"; Flags: ignoreversion recursesubdirs
Source: "scisdk\src\scisdk_defines.h"; DestDir: "{app}\src\"; Flags: ignoreversion recursesubdirs
Source: "scisdk\src\SciSDK_DLL.h"; DestDir: "{app}\src\"; Flags: ignoreversion recursesubdirs
Source: "scisdk\scisdk_docs\*"; DestDir: "{app}\docs\"; Flags: ignoreversion recursesubdirs
Source: "scisdk\examples\*"; DestDir: "{app}\examples\"; Flags: ignoreversion recursesubdirs

Source: "scisdk\bin\x64\Release\SciSDK_DLL.dll"; DestDir: "{sys}"; Flags: 64bit 
Source: "dt1260\x64\Release\SCIDK_Lib.dll"; DestDir: "{sys}"; Flags: 64bit 
Source: "x5560\x64\R5560_SDKLib.dll"; DestDir: "{sys}"; Flags: 64bit 
Source: "x5560\x64\libsodium.dll"; DestDir: "{sys}"; Flags: 64bit 
Source: "x5560\x64\libzmq-v140-mt-4_3_4.dll"; DestDir: "{sys}"; Flags: 64bit 
Source: "CAEN Dig2\x86_64\CAEN_Dig2Lib.dll"; DestDir: "{sys}"; Flags: 64bit 
Source: "CAEN FELib\x86_64\CAEN_FELib.dll"; DestDir: "{sys}"; Flags: 64bit 

Source: "scisdk\bin\Win32\Release\SciSDK_DLL.dll"; DestDir: "{sys}"; Flags: 32bit 
Source: "dt1260\Win32\Release\SCIDK_Lib.dll"; DestDir: "{sys}"; Flags: 32bit 
Source: "x5560\x86\R5560_SDKLib.dll"; DestDir: "{sys}"; Flags: 32bit 
Source: "x5560\x86\libsodium.dll"; DestDir: "{sys}"; Flags: 32bit 
Source: "x5560\x86\libzmq-v140-mt-4_3_4.dll"; DestDir: "{sys}"; Flags: 32bit 
Source: "CAEN Dig2\x86\CAEN_Dig2Lib.dll"; DestDir: "{sys}"; Flags: 32bit 
Source: "CAEN FELib\x86\CAEN_FELib.dll"; DestDir: "{sys}"; Flags: 32bit 
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

Source: ".\Redist\VC_redist.x64.exe"; DestDir: {tmp}; Flags: deleteafterinstall
Source: ".\Redist\VC_redist.x86.exe"; DestDir: {tmp}; Flags: deleteafterinstall
Source: ".\Drivers\d2xx_setup.exe"; DestDir: {tmp}; Flags: deleteafterinstall
Source: ".\Drivers\d3xx_setup.exe"; DestDir: {tmp}; Flags: deleteafterinstall

[Icons]
Name: "{group}\SciSDK User Guide"; Filename: "{app}\docs\index.html"; WorkingDir: "{app}\docs\"
Name: "{group}\SciSDK Examples"; Filename: "{app}\examples"; WorkingDir: "{app}"; Flags: foldershortcut

[Run]
Filename: "{tmp}\VC_redist.x86.exe"; Parameters: "/q /norestart"; \
    Check: VC2022RedistNeedsInstall('x86'); \
    Flags: waituntilterminated; \
    StatusMsg: "Installing VC++ 2022 (x86) redistributables..."
Filename: "{tmp}\VC_redist.x64.exe"; Parameters: "/q /norestart"; \
    Check: VC2022RedistNeedsInstall('x64'); \
    Flags: waituntilterminated; \
    StatusMsg: "Installing VC++ 2022 (x64) redistributables..."

Filename: "{tmp}\d2xx_setup.exe"; Description: "Install DT1260 Drivers (FTDI D2XX)"; \
  Flags: postinstall runascurrentuser      
Filename: "{tmp}\d3xx_setup.exe"; Description: "Install DT5550X Drivers (FTDI D3XX)"; \
  Flags: postinstall runascurrentuser

[Code]
function VC2022RedistNeedsInstall(Arch: string): Boolean;
var 
  Version: String;
begin
  if RegQueryStringValue(
       HKEY_LOCAL_MACHINE,
       'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\' + Arch,
       'Version', Version) then
  begin
    Log('VC Redist Version check : found ' + Version);
    Result := (CompareStr(Version, 'v14.34.31931.00')<0);
  end;
end;