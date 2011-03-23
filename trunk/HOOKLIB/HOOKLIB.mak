# Microsoft Developer Studio Generated NMAKE File, Based on HOOKLIB.dsp
!IF "$(CFG)" == ""
CFG=HOOKLIB - Win32 Debug
!MESSAGE No configuration specified. Defaulting to HOOKLIB - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "HOOKLIB - Win32 Release" && "$(CFG)" != "HOOKLIB - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HOOKLIB.mak" CFG="HOOKLIB - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HOOKLIB - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "HOOKLIB - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HOOKLIB - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\HOOKLIB.dll"


CLEAN :
	-@erase "$(INTDIR)\HookLib.obj"
	-@erase "$(INTDIR)\HOOKLIB.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\HOOKLIB.dll"
	-@erase "$(OUTDIR)\HOOKLIB.exp"
	-@erase "$(OUTDIR)\HOOKLIB.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HOOKLIB_EXPORTS" /Fp"$(INTDIR)\HOOKLIB.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\HOOKLIB.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\HOOKLIB.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\HOOKLIB.pdb" /machine:I386 /out:"$(OUTDIR)\HOOKLIB.dll" /implib:"$(OUTDIR)\HOOKLIB.lib" 
LINK32_OBJS= \
	"$(INTDIR)\HookLib.obj" \
	"$(INTDIR)\HOOKLIB.res"

"$(OUTDIR)\HOOKLIB.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "HOOKLIB - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\HOOKLIB.dll" "$(OUTDIR)\HOOKLIB.bsc"


CLEAN :
	-@erase "$(INTDIR)\HookLib.obj"
	-@erase "$(INTDIR)\HOOKLIB.res"
	-@erase "$(INTDIR)\HookLib.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\HOOKLIB.bsc"
	-@erase "$(OUTDIR)\HOOKLIB.dll"
	-@erase "$(OUTDIR)\HOOKLIB.exp"
	-@erase "$(OUTDIR)\HOOKLIB.ilk"
	-@erase "$(OUTDIR)\HOOKLIB.lib"
	-@erase "$(OUTDIR)\HOOKLIB.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HOOKLIB_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\HOOKLIB.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\HOOKLIB.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\HOOKLIB.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\HookLib.sbr"

"$(OUTDIR)\HOOKLIB.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\HOOKLIB.pdb" /debug /machine:I386 /out:"$(OUTDIR)\HOOKLIB.dll" /implib:"$(OUTDIR)\HOOKLIB.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\HookLib.obj" \
	"$(INTDIR)\HOOKLIB.res"

"$(OUTDIR)\HOOKLIB.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("HOOKLIB.dep")
!INCLUDE "HOOKLIB.dep"
!ELSE 
!MESSAGE Warning: cannot find "HOOKLIB.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "HOOKLIB - Win32 Release" || "$(CFG)" == "HOOKLIB - Win32 Debug"
SOURCE=.\HookLib.cpp

!IF  "$(CFG)" == "HOOKLIB - Win32 Release"


"$(INTDIR)\HookLib.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "HOOKLIB - Win32 Debug"


"$(INTDIR)\HookLib.obj"	"$(INTDIR)\HookLib.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\HOOKLIB.RC

"$(INTDIR)\HOOKLIB.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

