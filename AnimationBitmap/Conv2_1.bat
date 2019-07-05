@echo off

mkdir Data
del Data\* /q

setlocal EnableDelayedExpansion

for /l %%x in (0,1,99) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame2.bat frm!formattedValue:~-5! Animation1 0 11 12
)
