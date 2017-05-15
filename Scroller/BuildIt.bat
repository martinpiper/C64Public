@echo off
del Scroller.prg
..\acme.exe -v3 --msvc ScrollEntry.a
if not exist Scroller.prg goto error
copy /y Scroller.prg ScrollerOrig.prg
..\bin\LZMPi.exe -c64b ScrollerOrig.prg Scroller.prg 1024 >t.txt
if not exist Scroller.prg goto error
goto end
:error
echo Scroller.prg not created!
exit -1
:end
