@echo off
echo Unit test...
for /F "eol=;usebackq tokens=* delims=," %%i in (UnitTest_Read.txt) do (
rem	@echo ** Start: %%i
	..\..\bin\TapeTool.exe r "%%i" bc60000 >Debug\t.txt

	findstr "gmail" Debug\t.txt >Debug\t2.txt
	if NOT ERRORLEVEL 1 echo **** Problem: %%i

rem	@echo ** End: %%i
)
