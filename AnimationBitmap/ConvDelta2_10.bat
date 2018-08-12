@echo off

mkdir Data
del Data\*.del /q
del Data\Dictionary*.bin /q

set START=1
set STEP=6
set END=7777

rem ..\Animation\DeltaCompression\Release\DeltaCompression.exe -savedictionary $1000 Data\dictionary1.bin -maxframesize 16000 -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $1c00 Data\frm*05d.chr 0 1 289 2 $4000 $7c00 Data\frm*05d.del 32
..\Animation\DeltaCompression\Release\DeltaCompression.exe -savedictionary $f00 Data\dictionary1.bin -savedictionary $f00 Data\dictionary2.bin -maxframesize 16000 -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $1c00 Data\frm*05d.chr %START% %STEP% %END% 2 $4000 $7c00 Data\frm*05d.del 254


del Data\*.del /q
..\Animation\DeltaCompression\Release\DeltaCompression.exe -optimisefile $d00 Data\dictionary1.bin -optimisefile $4d00 Data\dictionary2.bin -fragment f$d00 Data\Dictionary1.bin -fragment f$4d00 Data\Dictionary2.bin -maxframesize 16000 -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $1c00 Data\frm*05d.chr %START% %STEP% %END% 2 $4000 $7c00 Data\frm*05d.del 254
del Data\*.del /q
..\Animation\DeltaCompression\Release\DeltaCompression.exe -optimisefile $d00 Data\dictionary1.bin -optimisefile $4d00 Data\dictionary2.bin -fragment f$d00 Data\Dictionary1.bin -fragment f$4d00 Data\Dictionary2.bin -maxframesize 16000 -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $1c00 Data\frm*05d.chr %START% %STEP% %END% 2 $4000 $7c00 Data\frm*05d.del 254
