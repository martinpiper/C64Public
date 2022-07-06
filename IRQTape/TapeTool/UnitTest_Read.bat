@echo off
echo Unit test...
for /F "eol=;usebackq tokens=1* delims=," %%i in (UnitTest_Read.txt) do (
rem echo ** Start: %%j
rem echo ** Looking for: %%i
	..\..\bin\TapeTool.exe r "%%j" bc60000 >Debug\t.txt

	findstr /l "%%i" Debug\t.txt >Debug\t2.txt
	if ERRORLEVEL 1 echo **** Problem with expected: %%j && rem type Debug\t.txt

	findstr /l "gmail" Debug\t.txt >Debug\t2.txt
	if NOT ERRORLEVEL 1 echo **** Problem found gmail: %%j

rem	pause
rem	cls

rem echo ** End: %%j
)
