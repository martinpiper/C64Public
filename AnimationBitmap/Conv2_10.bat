@echo off

mkdir Data
del Data\* /q

setlocal EnableDelayedExpansion

for /l %%x in (0,1,7777) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame2h.bat frm!formattedValue:~-5! Animation10 0 1
)
