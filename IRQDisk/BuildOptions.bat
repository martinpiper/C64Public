@echo off
SETLOCAL ENABLEEXTENSIONS

IF "%~1"=="" (
	ECHO Usage: $c000="Start address" 1="Use two-bit ATN, otherwise the one-bit protocol is used" 1="Screen is disabled during load, otherwise the screen can be enabled" 1="Enable pulsing LED code" 15="LED Pulse speed" 5="Drive retries" 1="Enable drive code init" 1="Enable load code" 1="Enable compressed load" 1="Enable save code"
	ECHO 0=Disable the feature.
	ECHO Example to include drive code, with one-bit protocol, with normal and compressed load, but not the save code: BuildOptions.bat $c000 0 0 1 15 5 1 1 1 0
	GOTO end
)

REM First output the options for this build

echo IRQDisk_MachineTarget = IRQDisk_MachineTypeC64 > _buildOptions.a
echo RNXPCompressionDecompress_WriteBytesmAddr = ZPStartBasicLo >> _buildOptions.a

IF "%~3"=="1" (
	ECHO IRQDisk_NoScreenNoIRQ = 1 >> _buildOptions.a
)
IF "%~4"=="0" (
	ECHO IRQDisk_NoCode_Pulse = 1 >> _buildOptions.a
)
ECHO IRQDisk_LEDFlash = %~5 >> _buildOptions.a
ECHO IRQDisk_Retries = %~6 >> _buildOptions.a
IF "%~7"=="0" (
	ECHO IRQDisk_NoDriveCode = 1 >> _buildOptions.a
)
IF "%~8"=="0" (
	ECHO IRQDisk_NoCode_Load = 1 >> _buildOptions.a
)
IF "%~9"=="0" (
	ECHO IRQDisk_NoCode_LoadCompressed = 1 >> _buildOptions.a
)
IF "%~10"=="0" (
	ECHO IRQDisk_NoCode_Save = 1 >> _buildOptions.a
)
IF "%~2"=="1" (
	ECHO IRQDisk_UseATN = 1 >> _buildOptions.a
)

REM Now create the jump table with the enabled options
echo *=%~1 >> _buildOptions.a
IF "%~7"=="1" (
	ECHO jmp IRQDisk_Init >> _buildOptions.a
)
IF "%~8"=="1" (
	ECHO jmp IRQDisk_Load >> _buildOptions.a
)
IF "%~9"=="1" (
	ECHO jmp IRQDisk_LoadCompressed >> _buildOptions.a
)
IF "%~10"=="1" (
	ECHO jmp IRQDisk_WriteSetName >> _buildOptions.a
	ECHO jmp IRQDisk_WriteData >> _buildOptions.a
)
IF "%~2"=="1" (
IF "%~3"=="0" (
	ECHO jmp IRQDisk_MarkUnsafeRaster >> _buildOptions.a
)
)

ECHO jmp IRQDisk_computerSend >> _buildOptions.a
ECHO jmp IRQDisk_computerRecvBits >> _buildOptions.a

ECHO These are the build options, note the jump table order...
TYPE _buildOptions.a

..\acme.exe -v4 --pdb _buildOptions.pdb --msvc --lib ..\ -f cbm -o _buildOptions.prg stdlib/stdlib.a stdlib/LongBranches.a _buildOptions.a IRQDiskVars.a IRQDiskC64.a IRQDisk.a stdlib1541.a IRQDiskDriveCode.a

:end

