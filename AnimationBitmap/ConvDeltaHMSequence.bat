@echo off

rem This compresses all the panel animations as one long sequence
rem It uses two dictionaries to optimise the sequence further

mkdir Data
del Data\*.del /q
del Data\*.tcmp /q
del Data\Dictionary*.bin /q

rem This copies over the output for the full screen bitmaps
copy /y Data\frm00002.chr Data\frm00000.chr
copy /y Data\frm00002.scr Data\frm00000.scr
copy /y Data\frm00002.col Data\frm00000.col
copy /y Data\frm00002.spr Data\frm00000.spr
copy /y Data\frm00002.sno Data\frm00000.sno
copy /y Data\frm00002.chr Data\frm00001.chr
copy /y Data\frm00002.scr Data\frm00001.scr
copy /y Data\frm00002.col Data\frm00001.col
copy /y Data\frm00002.spr Data\frm00001.spr
copy /y Data\frm00002.sno Data\frm00001.sno

copy /y Data\frm00904.chr Data\frm00902.chr
copy /y Data\frm00904.scr Data\frm00902.scr
copy /y Data\frm00904.col Data\frm00902.col
copy /y Data\frm00904.spr Data\frm00902.spr
copy /y Data\frm00904.sno Data\frm00902.sno
copy /y Data\frm00904.chr Data\frm00903.chr
copy /y Data\frm00904.scr Data\frm00903.scr
copy /y Data\frm00904.col Data\frm00903.col
copy /y Data\frm00904.spr Data\frm00903.spr
copy /y Data\frm00904.sno Data\frm00903.sno

set START=0
set STEP=1
set END=1383
rem set START=1025
rem set STEP=1
rem set END=1240

set "EXCLUDEPANEL= "
set BLOCKLEN=32
rem set EXCLUDEPANEL=-excludebitmaprect $0000 $0400 $1c00 0 10 0 12 -excludebitmaprect $0000 $0400 $1c00 30 40 0 12 -excludebitmaprect $0000 $0400 $1c00 0 40 12 25 -excludebitmaprect $4000 $4400 $5c00 0 10 0 12 -excludebitmaprect $4000 $4400 $5c00 30 40 0 12 -excludebitmaprect $4000 $4400 $5c00 0 40 12 25
rem The temporary decompression space
set EXCLUDEPANEL=-excludeposend $7400 $7c00
set BLOCKLEN=254

..\Animation\DeltaCompression\Release\DeltaCompression.exe %EXCLUDEPANEL% -savedictionary $f00 Data\dictionary1.bin -savedictionary $f00 Data\dictionary2.bin -maxframesize 6000 -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr   %START% %STEP% %END% 2 $4000 $7c00 Data\frm*05d.del %BLOCKLEN%

del Data\*.del /q
rem TODO: Why does the version with two dictionary files fail during compression and decompression sanity check?
rem ..\Animation\DeltaCompression\Release\DeltaCompression.exe %EXCLUDEPANEL% -optimisefile $d00 Data\dictionary1.bin -optimisefile $4d00 Data\dictionary2.bin -fragment f$d00 Data\Dictionary1.bin -fragment f$4d00 Data\Dictionary2.bin -maxframesize 6000 -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr   %START% %STEP% %END% 2 $4000 $7c00 Data\frm*05d.del %BLOCKLEN%
..\Animation\DeltaCompression\Release\DeltaCompression.exe %EXCLUDEPANEL% -optimisefile $d00 Data\dictionary1.bin -fragment f$d00 Data\Dictionary1.bin -maxframesize 6000 -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr   %START% %STEP% %END% 2 $4000 $7c00 Data\frm*05d.del %BLOCKLEN%

del Data\*.del /q
rem ..\Animation\DeltaCompression\Release\DeltaCompression.exe %EXCLUDEPANEL% -optimisefile $d00 Data\dictionary1.bin -optimisefile $4d00 Data\dictionary2.bin -fragment f$d00 Data\Dictionary1.bin -fragment f$4d00 Data\Dictionary2.bin -maxframesize 6000 -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr   %START% %STEP% %END% 2 $4000 $7c00 Data\frm*05d.del %BLOCKLEN%
..\Animation\DeltaCompression\Release\DeltaCompression.exe %EXCLUDEPANEL% -optimisefile $d00 Data\dictionary1.bin -fragment f$d00 Data\Dictionary1.bin -maxframesize 6000 -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr   %START% %STEP% %END% 2 $4000 $7c00 Data\frm*05d.del %BLOCKLEN%

rem call ConvDeltaWait.bat
