@echo off

mkdir Data
del Data\* /q

setlocal EnableDelayedExpansion

..\bin\CharPack.exe Animation8\ThalamusLogo.bmp 15 11 12 Data\ThalamusLogo.chr Data\ThalamusLogo.scr Data\ThalamusLogo.col


rem Hunter's moon logo. No sprite overlay allowed.
for /l %%x in (0,1,1) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame.bat frm!formattedValue:~-5! Animation8 6 11 12 64
)

del /q Data\t.scr
del /q Data\t.col

rem Scene 1
for /l %%x in (2,1,151) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame.bat frm!formattedValue:~-5! Animation8 0 11 15 2
)

rem Scene 2
for /l %%x in (152,1,301) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame.bat frm!formattedValue:~-5! Animation8 0 07 15 2
)

rem Scene 3
for /l %%x in (302,1,451) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame.bat frm!formattedValue:~-5! Animation8 0 11 12 2
)

rem Scene 4
for /l %%x in (452,1,601) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame.bat frm!formattedValue:~-5! Animation8 0 11 12 2
)

rem Scene 5
for /l %%x in (602,1,751) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame.bat frm!formattedValue:~-5! Animation8 0 11 12 2
)

rem Scene 6
for /l %%x in (752,1,901) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame.bat frm!formattedValue:~-5! Animation8 0 11 12 2
)

del /q Data\t.scr
del /q Data\t.col
rem Loading screen. No sprite overlay allowed.
for /l %%x in (902,1,903) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame.bat frm!formattedValue:~-5! Animation8 0 11 12 64
)


rem Outro scene
del /q Data\t.scr
del /q Data\t.col

rem Scene 1
for /l %%x in (904,1,1383) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame.bat frm!formattedValue:~-5! Animation8 0 11 15 2
)
