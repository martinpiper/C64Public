@echo off

mkdir Data
del Data\* /q

setlocal EnableDelayedExpansion

for /l %%x in (0,1,138) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame2.bat frm!formattedValue:~-5! Animation3 0 11 12
)

for /l %%x in (139,1,289) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame2.bat frm!formattedValue:~-5! Animation3 3 11 12
)
