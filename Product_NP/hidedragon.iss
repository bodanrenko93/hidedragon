; �ű��� Inno Setup �ű������ɡ�
; �����ĵ���ȡ���� INNO SETUP �ű��ļ���ϸ����!

[Setup]
AppName=����ר�� 3.51
AppCopyright=Copyright (C) 2003 - 2009.7 ������
AppVerName=����ר�� 3.51
AppPublisher=http://www.5hide.com
AppComments=����ר����һ����Ļ��˽���������
AppPublisherURL=http://www.5Hide.com
AppSupportURL=http://www.5Hide.com
AppUpdatesURL=http://www.5Hide.com
DefaultDirName={pf}\HideDragon
DefaultGroupName=����ר�� 3.51
LicenseFile=d:\My Program\T2\Product_NP\lisence.txt
InfoBeforeFile=d:\My Program\T2\Product_NP\readme.rtf
InfoAfterFile=d:\My Program\T2\Product_NP\VerHistory.rtf
OutputDir=d:\My Homepage\cn
OutputBaseFilename=Hide_Show351
Compression=lzma
SolidCompression=yes
VersionInfoVersion=3.51
VersionInfoDescription=����ר�� 3.51
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





; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[INI]
Filename: "{app}\HideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.5Hide.com"
;Filename: "{app}\BuyHideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.hidedragon.com/cn/purchase.html"
Filename: "{app}\ManualHideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.hidedragon.com/cn/Manual_set.html"



[Icons]
Name: "{group}\����ר�� 3.51"; Filename: "{app}\Hide_Show.exe";WorkingDir: "{app}";
Name: "{userdesktop}\����ר�� 3.51"; Filename: "{app}\Hide_Show.exe"; Tasks: desktopicon ;WorkingDir: "{app}";
;Name: "{group}\{cm:ProgramOnTheWeb,����ר�� 3.51}"; Filename: "{app}\HideDragon.url";
;Name: "{group}\���߹���"; Filename: "{app}\BuyHideDragon.url";
Name: "{group}\ʹ���ֲ�"; Filename: "{app}\ManualHideDragon.url";
Name: "{group}\ж��"; Filename: "{app}\unins000.exe";


[Run]
Filename: "{app}\Hide_Show.exe"; Description: "{cm:LaunchProgram,����ר�� 3.51}"; Flags: nowait postinstall skipifsilent

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
