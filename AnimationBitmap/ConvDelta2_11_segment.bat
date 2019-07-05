@echo off

set /a "FSTART=%1-1"
..\BerzerkRedux\MashSamples\Release\MashSamples.exe -fps 25.0 -sf %FSTART% -ef %2 -r 2.0f -h $4e -v -s 22050 Animation11\t.wav 5000 Data\smp_%1.raw

..\Animation\DeltaCompression\Release\DeltaCompression.exe -maxframesize 15000 -emptyframebuffer -bitmap -fragment 0 Data\frm*05d.col -fragment $400 Data\frm*05d.scr -fragment $800 Data\frm*05d.spr -fragment $c00 Data\frm*05d.sno -fragment $1c00 Data\frm*05d.chr %1 3 %2 2 $4000 $7c00 Data\frm*05d.del 32
