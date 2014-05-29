@echo off
del BlankProjectComp.prg

..\acme.exe -v3 --msvc BlankProjectComp.a
if not exist BlankProjectComp.prg goto error
..\bin\LZMPi.exe -c64 BlankProjectComp.prg BlankProjectComp.prg $400 >t.txt
if not exist BlankProjectComp.prg goto error
goto end
:error
echo BlankProjectComp.prg not created!
exit -1
:end
