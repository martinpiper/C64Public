@echo off
del TestIRQDisk.prg
..\acme.exe -v4 --msvc TestIRQDisk.a
if not exist TestIRQDisk.prg goto error
..\bin\LZMPi.exe -c64b TestIRQDisk.prg TestIRQDisk.prg $8000 >t.txt
if not exist TestIRQDisk.prg goto error
..\acme.exe -v4 --msvc TestIRQDisk2.a
if not exist TestIRQDisk2.prg goto error
..\bin\LZMPi.exe -c64b TestIRQDisk2.prg TestIRQDisk2.prg $c000 >t.txt
if not exist TestIRQDisk2.prg goto error
goto end
:error
echo TestIRQDisk.prg not created!
exit -1
:end
..\ExternalTools\C1541\c1541.exe -attach test.d64 -format test,67 -write TestIRQDisk.prg testirqdisk -write screen1.prg screen1 -write screen2.prg screen2 -write screen3.prg screen3 -write TESTSAVE.PRG testsave
