; �ű��� Inno Setup �ű������ɡ�
; �����ĵ���ȡ���� INNO SETUP �ű��ļ���ϸ����!

[Setup]
AppName=����ר�� 3.5
AppCopyright=Copyright (C) 2003 - 2008.8 ������
AppVerName=����ר�� 3.5
AppPublisher=http://www.5hide.com
AppComments=����ר����һ����Ļ��˽���������
AppPublisherURL=http://www.5Hide.com
AppSupportURL=http://www.5Hide.com
AppUpdatesURL=http://www.5Hide.com
DefaultDirName={pf}\HideDragon
DefaultGroupName=����ר�� 3.5
LicenseFile=e:\My Program\T2\Product\lisence.txt
InfoBeforeFile=e:\My Program\T2\Product\readme.rtf
InfoAfterFile=e:\My Program\T2\Product\VerHistory.rtf
OutputDir=e:\My Homepage\cn
OutputBaseFilename=HideDragon35_full
Compression=lzma
SolidCompression=yes
VersionInfoVersion=3.5.0
VersionInfoDescription=����ר�� 3.5
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

; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[INI]
Filename: "{app}\HideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.5Hide.com"
Filename: "{app}\BuyHideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.hidedragon.com/cn/purchase.html"
Filename: "{app}\ManualHideDragon.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.hidedragon.com/cn/Manual_set.html"



[Icons]
Name: "{group}\����ר�� 3.5"; Filename: "{app}\HideDragon.exe";WorkingDir: "{app}";
Name: "{userdesktop}\����ר�� 3.5"; Filename: "{app}\HideDragon.exe"; Tasks: desktopicon ;WorkingDir: "{app}";
;Name: "{group}\{cm:ProgramOnTheWeb,����ר�� 3.5}"; Filename: "{app}\HideDragon.url";
Name: "{group}\���߹���"; Filename: "{app}\BuyHideDragon.url";
Name: "{group}\ʹ���ֲ�"; Filename: "{app}\ManualHideDragon.url";
Name: "{group}\ж��"; Filename: "{app}\unins000.exe";


[Run]
Filename: "{app}\HideDragon.exe"; Description: "{cm:LaunchProgram,����ר�� 3.5}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\HideDragon.url"
Type: files; Name: "{app}\AutoReserveFile.arf"
Type: files; Name: "{app}\BuyHideDragon.url"
Type: files; Name: "{app}\HabitMemFile.hmf"
Type: files; Name: "{app}\ManualHideDragon.url"
Type: files; Name: "{app}\hddb.db"

