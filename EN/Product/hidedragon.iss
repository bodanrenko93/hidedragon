; 脚本用 Inno Setup 脚本向导生成。
; 查阅文档获取创建 INNO SETUP 脚本文件详细资料!

[Setup]
AppName=HideDragon 3.1.2
AppCopyright=Copyright (C) 2003 - 2008.5 HideDragon
AppVerName=HideDragon 3.1.2
AppPublisher=http://www.HideDragon.com
AppComments=HideDragon is a protect computer screen privacy software.
AppPublisherURL=http://www.HideDragon.com
AppSupportURL=http://www.HideDragon.com
AppUpdatesURL=http://www.HideDragon.com
DefaultDirName={pf}\HideDragon
DefaultGroupName=HideDragon 3.1
LicenseFile=e:\My Program\T2\EN\Product\liCenSe.txt
InfoBeforeFile=e:\My Program\T2\EN\Product\readme.rtf
InfoAfterFile=e:\My Program\T2\EN\Product\VerHistory.rtf
OutputDir=e:\My Homepage\en
OutputBaseFilename=HideDragon31_brothersoft
Compression=lzma
SolidCompression=yes
VersionInfoVersion=3.1.2.0
VersionInfoDescription=HideDragon 3.1.2
VersionInfoTextVersion=3.1
VersionInfoCompany=http://www.HideDragon.com
AppVersion=3.1.2
DefaultUserInfoName=http://www.HideDragon.com
SetupIconFile=E:\My Program\T2\HideDragon\icon1.ico




[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "e:\My Program\T2\En\Product\HideDragon.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\En\Product\HDUpdate.exe"; DestDir: "{app}"; Flags: ignoreversion
;Source: "e:\My Program\T2\En\Product\HDUpdate.exe.manifest"; DestDir: "{app}"; Flags: ignoreversion
;Source: "e:\My Program\T2\En\Product\hidedragon.exe.manifest"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\En\Product\HOOKLIB.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\En\Product\unrar.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\En\Product\VerHistory.rtf"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\En\Product\Misc.hdo"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\En\Product\license.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\En\Product\readme.rtf"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\En\Product\stlport_vc646.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\En\Product\sqlite3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\En\Product\HideProc.sys"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\En\Product\hddb.db"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\En\Product\Language\*.*"; DestDir: "{app}\Language"; Flags: ignoreversion
Source: "e:\My Program\T2\En\Product\Icon\*.*"; DestDir: "{app}\Icon"; Flags: ignoreversion

; 注意: 不要在任何共享系统文件中使用“Flags: ignoreversion”

[INI]
Filename: "{app}\HideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.HideDragon.com"
Filename: "{app}\BuyHideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.HideDragon.com/en/purchase.htm"
Filename: "{app}\ManualHideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.hidedragon.com/en/Manual_set.html"

[Icons]
Name: "{group}\HideDragon 3.1"; Filename: "{app}\HideDragon.exe";WorkingDir: "{app}";
Name: "{userdesktop}\HideDragon 3.1"; Filename: "{app}\HideDragon.exe"; Tasks: desktopicon ;WorkingDir: "{app}";
;Name: "{group}\{cm:ProgramOnTheWeb,HideDragon 3.1}"; Filename: "{app}\HideDragon.url";
Name: "{group}\Buy"; Filename: "{app}\BuyHideDragon.url";
Name: "{group}\HideDragon Manual"; Filename: "{app}\ManualHideDragon.url";
Name: "{group}\Uninstall"; Filename: "{app}\unins000.exe";

[Run]
Filename: "{app}\HideDragon.exe"; Description: "{cm:LaunchProgram,HideDragon 3.1}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\HideDragon.url"
Type: files; Name: "{app}\AutoReserveFile.arf"
Type: files; Name: "{app}\BuyHideDragon.url"
Type: files; Name: "{app}\HabitMemFile.hmf"
Type: files; Name: "{app}\ManualHideDragon.url"
Type: files; Name: "{app}\hddb.db"
