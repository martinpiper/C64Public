@echo off

mkdir Data
del Data\*.del /q
del Data\*.tcmp /q
del Data\Dictionary*.bin /q

set START=0
set STEP=1
rem set END=99
rem set END=149
set END=289

rem ..\Animation\DeltaCompression\Release\DeltaCompression.exe -savedictionary $1000 Data\dictionary1.bin -maxframesize 16000 -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $1c00 Data\frm*05d.chr 0 1 289 2 $4000 $7c00 Data\frm*05d.del 32
..\Animation\DeltaCompression\Release\DeltaCompression.exe -savedictionary $f00 Data\dictionary1.bin -savedictionary $f00 Data\dictionary2.bin -maxframesize 7000 -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr %START% %STEP% %END% 2 $4000 $7c00 Data\frm*05d.del 32


del Data\*.del /q
..\Animation\DeltaCompression\Release\DeltaCompression.exe -optimisefile $d00 Data\dictionary1.bin -optimisefile $4d00 Data\dictionary2.bin -fragment f$d00 Data\Dictionary1.bin -fragment f$4d00 Data\Dictionary2.bin -maxframesize 7000 -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr %START% %STEP% %END% 2 $4000 $7c00 Data\frm*05d.del 32

del Data\*.del /q
..\Animation\DeltaCompression\Release\DeltaCompression.exe -optimisefile $d00 Data\dictionary1.bin -optimisefile $4d00 Data\dictionary2.bin -fragment f$d00 Data\Dictionary1.bin -fragment f$4d00 Data\Dictionary2.bin -maxframesize 7000 -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr %START% %STEP% %END% 2 $4000 $7c00 Data\frm*05d.del 32
