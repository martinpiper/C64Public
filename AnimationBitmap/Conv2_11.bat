@echo off

mkdir Data
del Data\* /q

setlocal EnableDelayedExpansion

rem for /l %%x in (1,1,4064) do (
for /l %%x in (1,1,992) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame2.bat frm!formattedValue:~-5! Animation11 0 11 12
)
