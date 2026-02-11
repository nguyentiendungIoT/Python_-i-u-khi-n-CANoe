@ECHO OFF

REM NB: 'ECHO(' produces a blank line

SET SILKIT_DIR=%CANoe_InstallDir64%

SET SCRIPT_DPATH_WITH_BACKSLASH=%~dp0
SET SCRIPT_DPATH=%SCRIPT_DPATH_WITH_BACKSLASH:~0,-1%

SET STARTED_SIL_KIT_REGISTRY=0
tasklist /fi "ImageName eq sil-kit-registry.exe" /fo csv /nh 2>NUL | find "sil-kit-registry.exe" >NUL 2>NUL
IF "%ERRORLEVEL%"=="0" GOTO FoundSilKitRegistry
	ECHO Starting SIL Kit Registry ...
	
	START "sil-kit-registry" "%SILKIT_DIR%\sil-kit-registry.exe"
	SET STARTED_SIL_KIT_REGISTRY=1
	
	GOTO HandledSilKitRegistry
:FoundSilKitRegistry
	ECHO SIL Kit Registry is already running ...
	
	GOTO HandledSilKitRegistry
:HandledSilKitRegistry

ECHO(
ECHO Starting SIL Kit System Controller ...

START "sil-kit-system-controller" "%SILKIT_DIR%\sil-kit-system-controller.exe" CANoe Engine Lights

ECHO(
ECHO Starting SUTs ...

START "sil-kit-participant-engine" "%SCRIPT_DPATH%\..\..\SilKit\bin\SUT_Engine.exe" Engine
START "sil-kit-participant-lights" "%SCRIPT_DPATH%\..\..\SilKit\bin\SUT_Lights.exe" Lights

ECHO(
ECHO ======================================================================
ECHO(
ECHO           Press ENTER to stop all processes
ECHO(
ECHO ======================================================================
ECHO(

PAUSE

ECHO(
ECHO Stopping SUTs ...
ECHO(

taskkill /F /IM SUT_Engine.exe
taskkill /F /IM SUT_Lights.exe

ECHO(
ECHO Stopping SIL Kit System Controller ...
ECHO(

taskkill /F /IM sil-kit-system-controller.exe

IF "%STARTED_SIL_KIT_REGISTRY%"=="0" GOTO KeepSilKitRegistry
	ECHO(
	ECHO Stopping SIL Kit Registry ...
	ECHO(
	
	taskkill /F /IM sil-kit-registry.exe
	
	ECHO(
:KeepSilKitRegistry
