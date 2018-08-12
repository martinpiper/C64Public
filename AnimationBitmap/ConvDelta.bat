@echo off

mkdir Data
del Data\*.del /q
del Data\Dictionary*.bin /q

..\Animation\DeltaCompression\Release\DeltaCompression.exe -maxframesize 6000 -emptyframebuffer -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr 0 1 1 2 $4000 $7c00 Data\frm*05d.del

rem ..\Animation\DeltaCompression\Release\DeltaCompression.exe -savedictionary $f00 Data\dictionary1.bin -savedictionary $f00 Data\dictionary2.bin -maxframesize 6000 -emptyframebuffer -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr 2 1 901 2 $4000 $7c00 Data\frm*05d.del 32
rem These need "ClearAnimationMemoryBeforePanels = 1" enabled, to cause jsr ClearAnimationMemory in Timeline_AddEvent_TriggerLowerTextDisplay to be used
set "EXCLUDEPANEL= "
set BLOCKLEN=32
rem set EXCLUDEPANEL=-excludebitmaprect $0000 $0400 $1c00 0 10 0 12 -excludebitmaprect $0000 $0400 $1c00 30 40 0 12 -excludebitmaprect $0000 $0400 $1c00 0 40 12 25 -excludebitmaprect $4000 $4400 $5c00 0 10 0 12 -excludebitmaprect $4000 $4400 $5c00 30 40 0 12 -excludebitmaprect $4000 $4400 $5c00 0 40 12 25
rem set BLOCKLEN=254
..\Animation\DeltaCompression\Release\DeltaCompression.exe %EXCLUDEPANEL% -savedictionary $f00 Data\dictionary1_1.bin -savedictionary $f00 Data\dictionary1_2.bin -maxframesize 6000 -emptyframebuffer -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr   2 1 151 2 $4000 $7c00 Data\frm*05d.del %BLOCKLEN%
..\Animation\DeltaCompression\Release\DeltaCompression.exe %EXCLUDEPANEL% -savedictionary $f00 Data\dictionary2_1.bin -savedictionary $f00 Data\dictionary2_2.bin -maxframesize 6000 -emptyframebuffer -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr 152 1 301 2 $4000 $7c00 Data\frm*05d.del %BLOCKLEN%
..\Animation\DeltaCompression\Release\DeltaCompression.exe %EXCLUDEPANEL% -savedictionary $f00 Data\dictionary3_1.bin -savedictionary $f00 Data\dictionary3_2.bin -maxframesize 6000 -emptyframebuffer -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr 302 1 451 2 $4000 $7c00 Data\frm*05d.del %BLOCKLEN%
..\Animation\DeltaCompression\Release\DeltaCompression.exe %EXCLUDEPANEL% -savedictionary $f00 Data\dictionary4_1.bin -savedictionary $f00 Data\dictionary4_2.bin -maxframesize 6000 -emptyframebuffer -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr 452 1 601 2 $4000 $7c00 Data\frm*05d.del %BLOCKLEN%
..\Animation\DeltaCompression\Release\DeltaCompression.exe %EXCLUDEPANEL% -savedictionary $f00 Data\dictionary5_1.bin -savedictionary $f00 Data\dictionary5_2.bin -maxframesize 6000 -emptyframebuffer -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr 602 1 751 2 $4000 $7c00 Data\frm*05d.del %BLOCKLEN%
..\Animation\DeltaCompression\Release\DeltaCompression.exe %EXCLUDEPANEL% -savedictionary $f00 Data\dictionary6_1.bin -savedictionary $f00 Data\dictionary6_2.bin -maxframesize 6000 -emptyframebuffer -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr 752 1 901 2 $4000 $7c00 Data\frm*05d.del %BLOCKLEN%

..\Animation\DeltaCompression\Release\DeltaCompression.exe -maxframesize 6000 -emptyframebuffer -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr 902 1 903 2 $4000 $7c00 Data\frm*05d.del

rem Outro
..\Animation\DeltaCompression\Release\DeltaCompression.exe %EXCLUDEPANEL% -savedictionary $f00 Data\dictionary7_1.bin -savedictionary $f00 Data\dictionary7_2.bin -maxframesize 6000 -emptyframebuffer -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr 904 1 1383 2 $4000 $7c00 Data\frm*05d.del %BLOCKLEN%

rem call ConvDeltaWait.bat
