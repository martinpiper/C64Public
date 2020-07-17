@echo off
del ChristmasDemo.prg

..\acme.exe -v4 --msvc ChristmasDemo.a
if not exist ChristmasDemo.prg goto error
..\bin\LZMPi.exe -c64b ChristmasDemo.prg ChristmasDemo.prg $400 >t.txt
if not exist ChristmasDemo.prg goto error
goto end
:error
echo ChristmasDemo.prg not created!
exit -1
:end
