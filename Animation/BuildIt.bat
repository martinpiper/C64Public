@echo off
del Animation.prg

..\acme.exe -v3 --msvc Animation.a
if not exist Animation.prg goto error
..\bin\LZMPi.exe -c64 Animation.prg Animation.prg $400 >t.txt
if not exist Animation.prg goto error
goto end
:error
echo Animation.prg not created!
:end
