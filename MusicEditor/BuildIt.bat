@echo off
del MusicEditor.prg
..\acme.exe -v4 --msvc MusicEditor.a
if not exist MusicEditor.prg goto error
..\bin\LZMPi.exe -c64b MusicEditor.prg MusicEditor.prg $900 >t.txt
if not exist MusicEditor.prg goto error
goto end
:error
echo MusicEditor.prg not created!
exit -1
:end
