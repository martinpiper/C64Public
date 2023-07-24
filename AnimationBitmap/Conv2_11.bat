rem @echo off

mkdir Data
del Data\* /q

setlocal EnableDelayedExpansion
rem -l 0 turns off runlength encoding
rem Note: "Calculated rate:" for 1615
rem But every ~1.5 lines now
..\BerzerkRedux\MashSamples\Release\MashSamples.exe -fps 25.0 -r 4.0f -h $4e -v -l 0 -s 22050 Animation11\t.wav 5750 Data\smp.raw

fsutil file createnew data\binaryData.bin 0

rem for /l %%x in (1,1,4064) do (
for /l %%x in (1,1,992) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame2.bat frm!formattedValue:~-5! Animation11 0 11 12
rem	call ConvFrameWindow.bat frm!formattedValue:~-5! Animation11 0 0 0 22 16
)
