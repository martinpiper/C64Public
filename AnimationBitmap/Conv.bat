@echo off

del Data\* /q

setlocal EnableDelayedExpansion

rem for /l %%x in (0,10,8260) do (
rem for /l %%x in (0,1,99) do (
rem for /l %%x in (0,1,729) do (
rem for /l %%x in (0,2,300) do (
for /l %%x in (0,2,290) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame.bat frm!formattedValue:~-5!
)

rem ..\Animation\DeltaCompression\Release\DeltaCompression.exe -bitmap Data\frm*05d.sch 0 0 10 8260 2 $4000 $7c00 Data\frm*05d.del
rem ..\Animation\DeltaCompression\Release\DeltaCompression.exe -bitmap Data\frm*05d.sch 0 0 1 99 2 $4000 $7c00 Data\frm*05d.del
rem ..\Animation\DeltaCompression\Release\DeltaCompression.exe -bitmap Data\frm*05d.sch 0 0 1 729 2 $4000 $7c00 Data\frm*05d.del
rem ..\Animation\DeltaCompression\Release\DeltaCompression.exe -bitmap Data\frm*05d.sch 0 0 2 300 2 $4000 $7c00 Data\frm*05d.del
..\Animation\DeltaCompression\Release\DeltaCompression.exe -bitmap Data\frm*05d.sch 0 0 2 290 2 $4000 $7c00 Data\frm*05d.del
