@echo off
del SMBTest.prg

..\acme.exe -v3 --msvc SMBTest.a
if not exist SMBTest.prg goto error
..\bin\LZMPi.exe -c64 SMBTest.prg SMBTest.prg $900 >t.txt
if not exist SMBTest.prg goto error
goto end
:error
echo SMBTest.prg not created!
exit -1
:end
