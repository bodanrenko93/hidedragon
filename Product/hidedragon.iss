; 脚本用 Inno Setup 脚本向导生成。
; 查阅文档获取创建 INNO SETUP 脚本文件详细资料!

[Setup]
AppName=隐身专家 3.5
AppCopyright=Copyright (C) 2003 - 2008.8 董大鹏
AppVerName=隐身专家 3.5
AppPublisher=http://www.5hide.com
AppComments=隐身专家是一款屏幕隐私保护软件。
AppPublisherURL=http://www.5Hide.com
AppSupportURL=http://www.5Hide.com
AppUpdatesURL=http://www.5Hide.com
DefaultDirName={pf}\HideDragon
DefaultGroupName=隐身专家 3.5
LicenseFile=e:\My Program\T2\Product\lisence.txt
InfoBeforeFile=e:\My Program\T2\Product\readme.rtf
InfoAfterFile=e:\My Program\T2\Product\VerHistory.rtf
OutputDir=e:\My Homepage\cn
OutputBaseFilename=HideDragon35_full
Compression=lzma
SolidCompression=yes
VersionInfoVersion=3.5.0
VersionInfoDescription=隐身专家 3.5
VersionInfoTextVersion=3.5
VersionInfoCompany=http://www.5Hide.com
AppVersion=3.5
DefaultUserInfoName=http://www.5Hide.com
SetupIconFile=E:\My Program\T2\HideDragon\icon1.ico




[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "e:\My Program\T2\Product\HideDragon.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\Product\HDUpdate.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\Product\HOOKLIB.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\Product\unrar.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\Product\VerHistory.rtf"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\Product\Misc.hdo"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\Product\lisence.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\Product\readme.rtf"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\Product\stlport_vc646.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\Product\sqlite3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\Product\HideProc.sys"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\Product\hddb.db"; DestDir: "{app}"; Flags: ignoreversion
Source: "e:\My Program\T2\Product\Language\*.*"; DestDir: "{app}\Language"; Flags: ignoreversion
Source: "e:\My Program\T2\Product\Icon\*.*"; DestDir: "{app}\Icon"; Flags: ignoreversion

; 注意: 不要在任何共享系统文件中使用“Flags: ignoreversion”

[INI]
Filename: "{app}\HideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.5Hide.com"
Filename: "{app}\BuyHideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.hidedragon.com/cn/purchase.html"
Filename: "{app}\ManualHideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.hidedragon.com/cn/Manual_set.html"



[Icons]
Name: "{group}\隐身专家 3.5"; Filename: "{app}\HideDragon.exe";WorkingDir: "{app}";
Name: "{userdesktop}\隐身专家 3.5"; Filename: "{app}\HideDragon.exe"; Tasks: desktopicon ;WorkingDir: "{app}";
;Name: "{group}\{cm:ProgramOnTheWeb,隐身专家 3.5}"; Filename: "{app}\HideDragon.url";
Name: "{group}\在线购买"; Filename: "{app}\BuyHideDragon.url";
Name: "{group}\使用手册"; Filename: "{app}\ManualHideDragon.url";
Name: "{group}\卸载"; Filename: "{app}\unins000.exe";


[Run]
Filename: "{app}\HideDragon.exe"; Description: "{cm:LaunchProgram,隐身专家 3.5}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\HideDragon.url"
Type: files; Name: "{app}\AutoReserveFile.arf"
Type: files; Name: "{app}\BuyHideDragon.url"
Type: files; Name: "{app}\HabitMemFile.hmf"
Type: files; Name: "{app}\ManualHideDragon.url"
Type: files; Name: "{app}\hddb.db"

