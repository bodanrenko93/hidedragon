; 脚本用 Inno Setup 脚本向导生成。
; 查阅文档获取创建 INNO SETUP 脚本文件详细资料!

[Setup]
AppName=隐身专家 3.51
AppCopyright=Copyright (C) 2003 - 2009.7 董大鹏
AppVerName=隐身专家 3.51
AppPublisher=http://www.5hide.com
AppComments=隐身专家是一款屏幕隐私保护软件。
AppPublisherURL=http://www.5Hide.com
AppSupportURL=http://www.5Hide.com
AppUpdatesURL=http://www.5Hide.com
DefaultDirName={pf}\HideDragon
DefaultGroupName=隐身专家 3.51
LicenseFile=d:\My Program\T2\Product_NP\lisence.txt
InfoBeforeFile=d:\My Program\T2\Product_NP\readme.rtf
InfoAfterFile=d:\My Program\T2\Product_NP\VerHistory.rtf
OutputDir=d:\My Homepage\cn
OutputBaseFilename=Hide_Show351
Compression=lzma
SolidCompression=yes
VersionInfoVersion=3.51
VersionInfoDescription=隐身专家 3.51
VersionInfoTextVersion=3.51
VersionInfoCompany=http://www.5Hide.com
AppVersion=3.51
DefaultUserInfoName=http://www.5Hide.com
SetupIconFile=d:\My Program\T2\HideDragon\icon1.ico




[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "d:\My Program\T2\Product_NP\Hide_Show.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NP\HDUpdate.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NP\HOOKLIB.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NP\unrar.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NP\VerHistory.rtf"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NP\Misc35.hdo"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NP\lisence.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NP\readme.rtf"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NP\stlport_vc646.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NP\sqlite3.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "d:\My Program\T2\Product_NP\HideProc.sys"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NP\hddb.db"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NP\Language\*.*"; DestDir: "{app}\Language"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NP\Icon\*.*"; DestDir: "{app}\Icon"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NP\gif\*.*"; DestDir: "{app}\gif"; Flags: ignoreversion
Source: "d:\My Program\T2\Product_NP\giflink.db"; DestDir: "{app}"; Flags: ignoreversion





; 注意: 不要在任何共享系统文件中使用“Flags: ignoreversion”

[INI]
Filename: "{app}\HideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.5Hide.com"
;Filename: "{app}\BuyHideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.hidedragon.com/cn/purchase.html"
Filename: "{app}\ManualHideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.hidedragon.com/cn/Manual_set.html"



[Icons]
Name: "{group}\隐身专家 3.51"; Filename: "{app}\Hide_Show.exe";WorkingDir: "{app}";
Name: "{userdesktop}\隐身专家 3.51"; Filename: "{app}\Hide_Show.exe"; Tasks: desktopicon ;WorkingDir: "{app}";
;Name: "{group}\{cm:ProgramOnTheWeb,隐身专家 3.51}"; Filename: "{app}\HideDragon.url";
;Name: "{group}\在线购买"; Filename: "{app}\BuyHideDragon.url";
Name: "{group}\使用手册"; Filename: "{app}\ManualHideDragon.url";
Name: "{group}\卸载"; Filename: "{app}\unins000.exe";


[Run]
Filename: "{app}\Hide_Show.exe"; Description: "{cm:LaunchProgram,隐身专家 3.51}"; Flags: nowait postinstall skipifsilent

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
