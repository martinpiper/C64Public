@echo off
del DemoArmalyte.prg DemoCitadel.prg DemoSnare.prg DemoWarbles.prg

echo ;This file is automatically generated by BuildIt.bat >FingerPrint.a
echo !scr "%TIME% %DATE% %COMPUTERNAME% %USERNAME%" >>FingerPrint.a


Release\CompactorTool.exe -i bin\armalyte2.mus.prg -o t.a
IF EXIST Demo.prg del Demo.prg
..\acme.exe -v4 --msvc Demo.a
if not exist Demo.prg goto error
..\bin\LZMPi.exe -c64b Demo.prg DemoArmalyte.prg $c00 >t.txt


Release\CompactorTool.exe -i bin\citadel2.mus.prg -o t.a
IF EXIST Demo.prg del Demo.prg
..\acme.exe -v4 --msvc Demo.a
if not exist Demo.prg goto error
..\bin\LZMPi.exe -c64b Demo.prg DemoCitadel.prg $c00 >t.txt


Release\CompactorTool.exe -i bin\snare2.mus.prg -o t.a
IF EXIST Demo.prg del Demo.prg
..\acme.exe -v4 --msvc Demo.a
if not exist Demo.prg goto error
..\bin\LZMPi.exe -c64b Demo.prg DemoSnare.prg $c00 >t.txt

Release\CompactorTool.exe -i bin\warbles2.mus.prg -o t.a
IF EXIST Demo.prg del Demo.prg
..\acme.exe -v4 --msvc Demo.a
if not exist Demo.prg goto error
..\bin\LZMPi.exe -c64b Demo.prg DemoWarbles.prg $c00 >t.txt

del Demo.prg

goto end
:error
echo Demos not created!
exit /B -1
:end
