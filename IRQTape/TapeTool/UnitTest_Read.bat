@echo off
for /F "eol=;usebackq tokens=* delims=," %%i in (UnitTest_Read.txt) do (
	@echo ** Start: %%i
	..\..\bin\TapeTool.exe r "%%i" bc2000
	@echo ** End: %%i
)
