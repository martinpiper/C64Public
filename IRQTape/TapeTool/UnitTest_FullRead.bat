@echo off
echo Problems are: ...
for /R "E:\Ultimate tape archive\" %%i IN (*.tap) DO (
rem @echo ** Start: %%i
	..\..\bin\TapeTool.exe r "%%i" bc60000 >Debug\t1.txt

	findstr "gmail" Debug\t1.txt >Debug\t12.txt
	if NOT ERRORLEVEL 1 echo %%i

rem @echo ** End: %%i
)
