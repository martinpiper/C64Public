@echo off
del BASICStarter.prg

..\acme.exe -v4 --msvc BASICStarter.a
if not exist BASICStarter.prg goto error
..\bin\LZMPi.exe -c64 BASICStarter.prg BASICStarter.prg $400 >t.txt
if not exist BASICStarter.prg goto error
goto end
:error
echo BASICStarter.prg not created!
exit -1
:end
