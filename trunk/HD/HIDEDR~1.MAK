# Microsoft Developer Studio Generated NMAKE File, Based on HideDragon.dsp
!IF "$(CFG)" == ""
CFG=HideDragon - Win32 Debug
!MESSAGE No configuration specified. Defaulting to HideDragon - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "HideDragon - Win32 Release" && "$(CFG)" != "HideDragon - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HideDragon.mak" CFG="HideDragon - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HideDragon - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "HideDragon - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "HideDragon - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\HideDragon.exe"


CLEAN :
	-@erase "$(INTDIR)\hd.obj"
	-@erase "$(INTDIR)\HDResource.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\HideDragon.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\HideDragon.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\HDResource.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\HideDragon.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\HideDragon.pdb" /machine:I386 /out:"$(OUTDIR)\HideDragon.exe" 
LINK32_OBJS= \
	"$(INTDIR)\hd.obj" \
	"$(INTDIR)\HDResource.res"

"$(OUTDIR)\HideDragon.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "HideDragon - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\HideDragon.exe" "$(OUTDIR)\HideDragon.bsc"


CLEAN :
	-@erase "$(INTDIR)\hd.obj"
	-@erase "$(INTDIR)\hd.sbr"
	-@erase "$(INTDIR)\HDResource.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\HideDragon.bsc"
	-@erase "$(OUTDIR)\HideDragon.exe"
	-@erase "$(OUTDIR)\HideDragon.ilk"
	-@erase "$(OUTDIR)\HideDragon.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\HideDragon.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\HDResource.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\HideDragon.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\hd.sbr"

"$(OUTDIR)\HideDragon.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=hooklib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\HideDragon.pdb" /debug /machine:I386 /out:"$(OUTDIR)\HideDragon.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\hd.obj" \
	"$(INTDIR)\HDResource.res"

"$(OUTDIR)\HideDragon.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("HideDragon.dep")
!INCLUDE "HideDragon.dep"
!ELSE 
!MESSAGE Warning: cannot find "HideDragon.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "HideDragon - Win32 Release" || "$(CFG)" == "HideDragon - Win32 Debug"
SOURCE=.\hd.cpp

!IF  "$(CFG)" == "HideDragon - Win32 Release"


"$(INTDIR)\hd.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "HideDragon - Win32 Debug"


"$(INTDIR)\hd.obj"	"$(INTDIR)\hd.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\HDResource.rc

"$(INTDIR)\HDResource.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

