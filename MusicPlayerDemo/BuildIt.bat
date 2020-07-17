@echo off
del MusicPlayerDemo.prg
..\acme.exe -v4 --msvc MusicPlayerDemo.a
if not exist MusicPlayerDemo.prg goto error
..\bin\LZMPi.exe -c64b MusicPlayerDemo.prg MusicPlayerDemo.prg $1000 >t.txt
if not exist MusicPlayerDemo.prg goto error
goto end
:error
echo MusicPlayerDemo.prg not created!
exit -1
:end
