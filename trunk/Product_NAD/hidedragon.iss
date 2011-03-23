; 脚本用 Inno Setup 脚本向导生成。
; 查阅文档获取创建 INNO SETUP 脚本文件详细资料!

[Setup]
AppName=隐身专家 3.6
AppCopyright=Copyright (C) 2003 - 2011.3 董大鹏
AppVerName=隐身专家 3.6
AppPublisher=http://www.store3000.com/hidedragon
AppComments=隐身专家是一款屏幕隐私保护软件。
AppPublisherURL=http://www.store3000.com/hidedragon
AppSupportURL=http://www.store3000.com/hidedragon
AppUpdatesURL=http://www.store3000.com/hidedragon
DefaultDirName={pf}\HideDragon
DefaultGroupName=隐身专家 3.6
LicenseFile=d:\My Program\T2\Product_NAD\lisence.txt
InfoBeforeFile=d:\My Program\T2\Product_NAD\readme.rtf
InfoAfterFile=d:\My Program\T2\Product_NAD\VerHistory.rtf
OutputDir=d:\My Program\T2\Product_NAD\
OutputBaseFilename=HideDragon36
Compression=lzma
SolidCompression=yes
VersionInfoVersion=3.6
VersionInfoDescription=隐身专家 3.6
VersionInfoTextVersion=3.6
VersionInfoCompany=http://www.store3000.com/hidedragon
AppVersion=3.6
DefaultUserInfoName=http://www.store3000.com/hidedragon
SetupIconFile=d:\My Program\T2\HideDragon\icon1.ico




[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "d:\My Program\T2\Product_NAD\HideDragon.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NAD\HDUpdate.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NAD\HOOKLIB.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NAD\unrar.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NAD\VerHistory.rtf"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NAD\Misc.hdo"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NAD\lisence.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NAD\readme.rtf"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NAD\stlport_vc646.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NAD\sqlite3.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "d:\My Program\T2\Product_NAD\HideProc.sys"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NAD\hddb.db"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NAD\Language\*.*"; DestDir: "{app}\Language"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NAD\Icon\*.*"; DestDir: "{app}\Icon"; Flags: ignoreversion

; 注意: 不要在任何共享系统文件中使用“Flags: ignoreversion”

[INI]
Filename: "{app}\HideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.store3000.com/hidedragon"
Filename: "{app}\BuyHideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.store3000.com/hidedragon"
Filename: "{app}\ManualHideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.store3000.com/hidedragon"



[Icons]
Name: "{group}\隐身专家 3.6"; Filename: "{app}\HideDragon.exe";WorkingDir: "{app}";
Name: "{userdesktop}\隐身专家 3.6"; Filename: "{app}\HideDragon.exe"; Tasks: desktopicon ;WorkingDir: "{app}";
;Name: "{group}\{cm:ProgramOnTheWeb,隐身专家 3.6}"; Filename: "{app}\HideDragon.url";
;Name: "{group}\在线购买"; Filename: "{app}\BuyHideDragon.url";
Name: "{group}\使用手册"; Filename: "{app}\ManualHideDragon.url";
Name: "{group}\卸载"; Filename: "{app}\unins000.exe";


[Run]
Filename: "{app}\HideDragon.exe"; Description: "{cm:LaunchProgram,隐身专家 3.6}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\HideDragon.url"
Type: files; Name: "{app}\AutoReserveFile.arf"
Type: files; Name: "{app}\BuyHideDragon.url"
Type: files; Name: "{app}\HabitMemFile.hmf"
Type: files; Name: "{app}\ManualHideDragon.url"
Type: files; Name: "{app}\hddb.db"
Type: files; Name: "{app}\hd_syslog.arf"
Type: files; Name: "{app}\Downloads\webstatus.db"
Type: files; Name: "{app}\Downloads\wndads.db"
Type: dirifempty; Name: "{app}\Downloads"
Type: dirifempty; Name: "{app}"
