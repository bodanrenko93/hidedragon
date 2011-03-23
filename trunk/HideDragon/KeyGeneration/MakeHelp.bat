@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by KEYGENERATION.HPJ. >"hlp\KeyGeneration.hm"
echo. >>"hlp\KeyGeneration.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\KeyGeneration.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\KeyGeneration.hm"
echo. >>"hlp\KeyGeneration.hm"
echo // Prompts (IDP_*) >>"hlp\KeyGeneration.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\KeyGeneration.hm"
echo. >>"hlp\KeyGeneration.hm"
echo // Resources (IDR_*) >>"hlp\KeyGeneration.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\KeyGeneration.hm"
echo. >>"hlp\KeyGeneration.hm"
echo // Dialogs (IDD_*) >>"hlp\KeyGeneration.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\KeyGeneration.hm"
echo. >>"hlp\KeyGeneration.hm"
echo // Frame Controls (IDW_*) >>"hlp\KeyGeneration.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\KeyGeneration.hm"
REM -- Make help for Project KEYGENERATION


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\KeyGeneration.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\KeyGeneration.hlp" goto :Error
if not exist "hlp\KeyGeneration.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\KeyGeneration.hlp" Debug
if exist Debug\nul copy "hlp\KeyGeneration.cnt" Debug
if exist Release\nul copy "hlp\KeyGeneration.hlp" Release
if exist Release\nul copy "hlp\KeyGeneration.cnt" Release
echo.
goto :done

:Error
echo hlp\KeyGeneration.hpj(1) : error: Problem encountered creating help file

:done
echo.
