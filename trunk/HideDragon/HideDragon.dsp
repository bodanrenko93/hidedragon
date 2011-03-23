# Microsoft Developer Studio Project File - Name="HideDragon" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=HideDragon - Win32 Debug_NAD
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HideDragon.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HideDragon.mak" CFG="HideDragon - Win32 Debug_NAD"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HideDragon - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "HideDragon - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "HideDragon - Win32 BCDebug" (based on "Win32 (x86) Application")
!MESSAGE "HideDragon - Win32 Debug_NP" (based on "Win32 (x86) Application")
!MESSAGE "HideDragon - Win32 Release_NP" (based on "Win32 (x86) Application")
!MESSAGE "HideDragon - Win32 Debug_NAD" (based on "Win32 (x86) Application")
!MESSAGE "HideDragon - Win32 Release_NAD" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HideDragon - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /D "_HIDEPROCESS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\Output\Release\hooklib.lib comctl32.lib shlwapi.lib ws2_32.lib version.lib wininet.lib shell32.lib shlwapi.lib unrar.lib sqlite3\sqlite3.lib /nologo /subsystem:windows /machine:I386 /out:"..\Output\Release\HideDragon.exe"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copy to Product directory
PostBuild_Cmds=copy ..\Output\Release\HideDragon.exe   "..\Product\hidedragon.exe"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "HideDragon - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /D "_HIDEPROCESS" /D "_NOADS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../Output/Debug/hooklib.lib comctl32.lib shlwapi.lib ws2_32.lib version.lib wininet.lib winmm.lib unrar.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sqlite3\sqlite3.lib /nologo /subsystem:windows /map:"HideDragon.map" /debug /machine:I386 /out:"../Output/debug/HideDragon.exe" /pdbtype:sept /mapinfo:lines
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "HideDragon - Win32 BCDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "HideDragon___Win32_BCDebug"
# PROP BASE Intermediate_Dir "HideDragon___Win32_BCDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "HideDragon___Win32_BCDebug"
# PROP Intermediate_Dir "HideDragon___Win32_BCDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 hooklib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib shlwapi.lib ws2_32.lib version.lib wininet.lib shell32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 hooklib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib shlwapi.lib ws2_32.lib version.lib wininet.lib shell32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "HideDragon - Win32 Debug_NP"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "HideDragon___Win32_Debug_NP"
# PROP BASE Intermediate_Dir "HideDragon___Win32_Debug_NP"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "HideDragon___Win32_Debug_NP"
# PROP Intermediate_Dir "HideDragon___Win32_Debug_NP"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /D "_HIDEPROCESS" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../Output/Debug/hooklib.lib comctl32.lib shlwapi.lib ws2_32.lib version.lib wininet.lib winmm.lib unrar.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sqlite3\sqlite3.lib /nologo /subsystem:windows /map:"HideDragon.map" /debug /machine:I386 /out:"../Output/debug/HideDragon.exe" /pdbtype:sept /mapinfo:lines
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ../Output/Debug/hooklib.lib comctl32.lib shlwapi.lib ws2_32.lib version.lib wininet.lib winmm.lib unrar.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sqlite3\sqlite3.lib Rasapi32.lib /nologo /subsystem:windows /map:"HideDragon.map" /debug /machine:I386 /out:"../Output/debug/Hide_Show.exe" /pdbtype:sept /mapinfo:lines
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "HideDragon - Win32 Release_NP"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "HideDragon___Win32_Release_NP"
# PROP BASE Intermediate_Dir "HideDragon___Win32_Release_NP"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "HideDragon___Win32_Release_NP"
# PROP Intermediate_Dir "HideDragon___Win32_Release_NP"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /D "_HIDEPROCESS" /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /FR /YX /FD /c
# SUBTRACT CPP /Z<none>
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\Output\Release\hooklib.lib comctl32.lib shlwapi.lib ws2_32.lib version.lib wininet.lib shell32.lib shlwapi.lib unrar.lib sqlite3\sqlite3.lib /nologo /subsystem:windows /machine:I386 /out:"..\Output\Release\HideDragon.exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\Output\Release\hooklib.lib comctl32.lib shlwapi.lib ws2_32.lib version.lib wininet.lib shell32.lib shlwapi.lib unrar.lib sqlite3\sqlite3.lib Rasapi32.lib /nologo /subsystem:windows /machine:I386 /out:"..\Output\Release\Hide_Show.exe"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copy to Product_NP directory
PostBuild_Cmds=copy ..\Output\Release\Hide_Show.exe   "..\Product_NP\Hide_Show.exe"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "HideDragon - Win32 Debug_NAD"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "HideDragon___Win32_Debug_NAD"
# PROP BASE Intermediate_Dir "HideDragon___Win32_Debug_NAD"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "HideDragon___Win32_Debug_NAD"
# PROP Intermediate_Dir "HideDragon___Win32_Debug_NAD"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /D "_NOADS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../Output/Debug/hooklib.lib comctl32.lib shlwapi.lib ws2_32.lib version.lib wininet.lib winmm.lib unrar.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sqlite3\sqlite3.lib /nologo /subsystem:windows /map:"HideDragon.map" /debug /machine:I386 /out:"../Output/debug/HideDragon.exe" /pdbtype:sept /mapinfo:lines
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ../Output/Debug/hooklib.lib comctl32.lib shlwapi.lib ws2_32.lib version.lib wininet.lib winmm.lib unrar.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sqlite3\sqlite3.lib Rasapi32.lib /nologo /subsystem:windows /map:"HideDragon.map" /debug /machine:I386 /out:"../Output/debug/HideDragon.exe" /pdbtype:sept /mapinfo:lines
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "HideDragon - Win32 Release_NAD"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "HideDragon___Win32_Release_NAD"
# PROP BASE Intermediate_Dir "HideDragon___Win32_Release_NAD"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "HideDragon___Win32_Release_NAD"
# PROP Intermediate_Dir "HideDragon___Win32_Release_NAD"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /D "_NOADS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\Output\Release\hooklib.lib comctl32.lib shlwapi.lib ws2_32.lib version.lib wininet.lib shell32.lib shlwapi.lib unrar.lib sqlite3\sqlite3.lib /nologo /subsystem:windows /machine:I386 /out:"..\Output\Release\HideDragon.exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\Output\Release\hooklib.lib comctl32.lib shlwapi.lib ws2_32.lib version.lib wininet.lib shell32.lib shlwapi.lib unrar.lib sqlite3\sqlite3.lib Rasapi32.lib /nologo /subsystem:windows /machine:I386 /out:"..\Output\Release\HideDragon.exe"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copy to Product_NAD directory
PostBuild_Cmds=copy ..\Output\Release\HideDragon.exe   "..\Product_NAD\hidedragon.exe"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "HideDragon - Win32 Release"
# Name "HideDragon - Win32 Debug"
# Name "HideDragon - Win32 BCDebug"
# Name "HideDragon - Win32 Debug_NP"
# Name "HideDragon - Win32 Release_NP"
# Name "HideDragon - Win32 Debug_NAD"
# Name "HideDragon - Win32 Release_NAD"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ClearDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Sqlite3\CppSQLite3.cpp
# End Source File
# Begin Source File

SOURCE=.\CWebPage.c
# End Source File
# Begin Source File

SOURCE=.\DBClass.cpp
# End Source File
# Begin Source File

SOURCE=.\hd.cpp
# End Source File
# Begin Source File

SOURCE=.\hd2.cpp
# End Source File
# Begin Source File

SOURCE=.\hd3.cpp
# End Source File
# Begin Source File

SOURCE=.\HDResource.rc
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\PictureEx.cpp
# End Source File
# Begin Source File

SOURCE=.\UseProxyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WebDB.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonClass\WINButton.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ClearDlg.h
# End Source File
# Begin Source File

SOURCE=.\Sqlite3\CppSQLite3.h
# End Source File
# Begin Source File

SOURCE=.\CWebPage.h
# End Source File
# Begin Source File

SOURCE=.\DBClass.h
# End Source File
# Begin Source File

SOURCE=.\hd.h
# End Source File
# Begin Source File

SOURCE=.\HDStruct.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\PictureEx.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Update.h
# End Source File
# Begin Source File

SOURCE=.\UseProxyDlg.h
# End Source File
# Begin Source File

SOURCE=.\WebDB.h
# End Source File
# Begin Source File

SOURCE=.\ButtonClass\WINButton.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\App.ico
# End Source File
# Begin Source File

SOURCE=.\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\Bmp\back.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\bottom.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonicon\cleannow_clean.ico
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\Bmp\Disable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonicon\hideipnow_hideip.ico
# End Source File
# Begin Source File

SOURCE=.\res\buttonicon\hidethisicon_iconlist.ico
# End Source File
# Begin Source File

SOURCE=.\res\buttonicon\hidethiswindow_allwnd.ico
# End Source File
# Begin Source File

SOURCE=.\Bmp\hove.bmp
# End Source File
# Begin Source File

SOURCE=.\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\id_dn_ic.ico
# End Source File
# Begin Source File

SOURCE=.\id_up_ic.ico
# End Source File
# Begin Source File

SOURCE=.\res\buttonicon\info_bkwizard.ico
# End Source File
# Begin Source File

SOURCE=.\res\buttonicon\info_hideip.ico
# End Source File
# Begin Source File

SOURCE=.\res\buttonicon\info_option.ico
# End Source File
# Begin Source File

SOURCE=.\Left_Ext.bmp
# End Source File
# Begin Source File

SOURCE=.\manifest.bin
# End Source File
# Begin Source File

SOURCE=.\MiniButton.bmp
# End Source File
# Begin Source File

SOURCE=.\MiniPress.bmp
# End Source File
# Begin Source File

SOURCE=.\Bmp\press.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonicon\refresh_allwnd.ico
# End Source File
# Begin Source File

SOURCE=.\res\buttonicon\restoreip_hideip.ico
# End Source File
# Begin Source File

SOURCE=.\Right_Ext.bmp
# End Source File
# Begin Source File

SOURCE=.\rtfs1.bin
# End Source File
# Begin Source File

SOURCE=.\res\buttonicon\searchip_hideip.ico
# End Source File
# Begin Source File

SOURCE=.\res\buttonicon\showicon_bklist.ico
# End Source File
# Begin Source File

SOURCE=.\Side_left.bmp
# End Source File
# Begin Source File

SOURCE=.\Side_right.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tabbmp\tab_adsunion.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tabbmp\tab_allwnd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tabbmp\tab_autohide.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tabbmp\tab_bklist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tabbmp\tab_clean.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tabbmp\tab_hideip.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tabbmp\tab_trayicon.bmp
# End Source File
# Begin Source File

SOURCE=.\TitleBar.bmp
# End Source File
# Begin Source File

SOURCE=".\BUTTONBMP\toolbar _ burn.bmp"
# End Source File
# Begin Source File

SOURCE=".\BUTTONICON\Toolbar _ Burn16.bmp"
# End Source File
# Begin Source File

SOURCE=.\W95MBX04.ICO
# End Source File
# End Group
# Begin Source File

SOURCE=.\hidedragon.exe.manifest
# End Source File
# End Target
# End Project
