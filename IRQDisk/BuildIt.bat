@echo off
del TestIRQDisk.prg
..\acme.exe -v4 --msvc TestIRQDisk.a
if not exist TestIRQDisk.prg goto error
..\bin\LZMPi.exe -c64b TestIRQDisk.prg TestIRQDisk.prg $8000 >t.txt
if not exist TestIRQDisk.prg goto error

del TestIRQDisk2.prg
..\acme.exe -v4 --msvc TestIRQDisk2.a
if not exist TestIRQDisk2.prg goto error
..\bin\LZMPi.exe -c64b TestIRQDisk2.prg TestIRQDisk2.prg $c000 >t.txt
if not exist TestIRQDisk2.prg goto error

del TestIRQDisk3.prg
..\acme.exe -v4 --msvc TestIRQDisk3.a
if not exist TestIRQDisk3.prg goto error

goto end

:error
echo TestIRQDisk.prg not created!
exit -1
:end
..\ExternalTools\C1541\c1541.exe -interleave 15 -attach test.d64 -format test,67 -write TestIRQDisk.prg testirqdisk -write screen1.prg screen1 -write screen2.prg screen2 -write screen3.prg screen3 -write TESTSAVE.PRG testsave
