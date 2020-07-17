@echo off
del SubGameStub.prg
..\acme.exe -v4 --msvc --lib ../Scroller/ asm/SubGameStub.a
if not exist SubGameStub.prg goto error
..\bin\LZMPi.exe -c64b SubGameStub.prg SubGameStub.prg $800 >t.txt
if not exist SubGameStub.prg goto error
goto end
:error
echo SubGameStub.prg not created!
exit /B -1
:end
