@echo off
del MusicConvert.prg
..\acme.exe -v3 --msvc MusicConvert.a
if not exist MusicConvert.prg goto error
..\bin\LZMPi.exe -c64b MusicConvert.prg MusicConvert.prg $800 >t.txt
if not exist MusicConvert.prg goto error
goto end
:error
echo MusicConvert.prg not created!
:end
