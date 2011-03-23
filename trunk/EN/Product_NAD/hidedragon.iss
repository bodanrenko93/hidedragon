; 脚本用 Inno Setup 脚本向导生成。
; 查阅文档获取创建 INNO SETUP 脚本文件详细资料!

[Setup]
AppName=HideDragon 3.2
AppCopyright=Copyright (C) 2003 - 2008.8 HideDragon
AppVerName=HideDragon 3.2
AppPublisher=http://www.HideDragon.com
AppComments=HideDragon is a protect computer screen privacy software.
AppPublisherURL=http://www.HideDragon.com
AppSupportURL=http://www.HideDragon.com
AppUpdatesURL=http://www.HideDragon.com
DefaultDirName={pf}\HideDragon
DefaultGroupName=HideDragon 3.2
LicenseFile=d:\My Program\T2\EN\Product_NP\liCenSe.txt
InfoBeforeFile=d:\My Program\T2\EN\Product_NP\readme.rtf
InfoAfterFile=d:\My Program\T2\EN\Product_NP\VerHistory.rtf
OutputDir=d:\My Homepage\en
OutputBaseFilename=HideDragon32
Compression=lzma
SolidCompression=yes
VersionInfoVersion=3.2
VersionInfoDescription=HideDragon 3.2
VersionInfoTextVersion=3.2
VersionInfoCompany=http://www.HideDragon.com
AppVersion=3.2
DefaultUserInfoName=http://www.HideDragon.com
SetupIconFile=d:\My Program\T2\en\HideDragon\icon1.ico




[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "d:\My Program\T2\En\Product_NP\HideDragon.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\En\Product_NP\HDUpdate.exe"; DestDir: "{app}"; Flags: ignoreversion
;Source: "d:\My Program\T2\En\Product_NP\HDUpdate.exe.manifest"; DestDir: "{app}"; Flags: ignoreversion
;Source: "d:\My Program\T2\En\Product_NP\hidedragon.exe.manifest"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\En\Product_NP\HOOKLIB.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\En\Product_NP\unrar.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\En\Product_NP\VerHistory.rtf"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\En\Product_NP\Misc.hdo"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\En\Product_NP\license.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\En\Product_NP\readme.rtf"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\En\Product_NP\stlport_vc646.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\En\Product_NP\sqlite3.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "d:\My Program\T2\En\Product_NP\HideProc.sys"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\En\Product_NP\hddb.db"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\En\Product_NP\Language\*.*"; DestDir: "{app}\Language"; Flags: ignoreversion
Source: "d:\My Program\T2\En\Product_NP\Icon\*.*"; DestDir: "{app}\Icon"; Flags: ignoreversion

; 注意: 不要在任何共享系统文件中使用“Flags: ignoreversion”

[INI]
Filename: "{app}\HideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.HideDragon.com"
Filename: "{app}\BuyHideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.HideDragon.com/en/purchase.html"
Filename: "{app}\ManualHideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.hidedragon.com/en/Manual_set.html"

[Icons]
Name: "{group}\HideDragon 3.2"; Filename: "{app}\HideDragon.exe";WorkingDir: "{app}";
Name: "{userdesktop}\HideDragon 3.2"; Filename: "{app}\HideDragon.exe"; Tasks: desktopicon ;WorkingDir: "{app}";
;Name: "{group}\{cm:ProgramOnTheWeb,HideDragon 3.2}"; Filename: "{app}\HideDragon.url";
Name: "{group}\Buy"; Filename: "{app}\BuyHideDragon.url";
Name: "{group}\HideDragon Manual"; Filename: "{app}\ManualHideDragon.url";
Name: "{group}\Uninstall"; Filename: "{app}\unins000.exe";

[Run]
Filename: "{app}\HideDragon.exe"; Description: "{cm:LaunchProgram,HideDragon 3.2}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\HideDragon.url"
Type: files; Name: "{app}\AutoReserveFile.arf"
Type: files; Name: "{app}\BuyHideDragon.url"
Type: files; Name: "{app}\HabitMemFile.hmf"
Type: files; Name: "{app}\ManualHideDragon.url"
Type: files; Name: "{app}\hddb.db"
Type: files; Name: "{app}\Downloads\webstatus.db"
Type: files; Name: "{app}\Downloads\wndads.db"
Type: dirifempty; Name: "{app}\Downloads"
Type: dirifempty; Name: "{app}"
