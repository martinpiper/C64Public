@echo off

rem ..\bin\LZMPi.exe -no -yo -cu screen1.prg screen1.cmp 2
rem ..\bin\LZMPi.exe -no -yo -cu screen2.prg screen2.cmp 2
rem ..\bin\LZMPi.exe -no -yo -cu screen3.prg screen3.cmp 2
rem ..\bin\LZMPi.exe -no -yo -cu screen4.prg screen4.cmp 2

del TestIRQDisk.prg
..\acme.exe -v4 --msvc --lib ..\ TestIRQDisk.a
if not exist TestIRQDisk.prg goto error
..\bin\LZMPi.exe -c64b TestIRQDisk.prg TestIRQDisk.prg $8000 >t.txt
if not exist TestIRQDisk.prg goto error

del TestIRQDisk2.prg
..\acme.exe -v4 --msvc --lib ..\ TestIRQDisk2.a
if not exist TestIRQDisk2.prg goto error
..\bin\LZMPi.exe -c64b TestIRQDisk2.prg TestIRQDisk2.prg $c000 >t.txt
if not exist TestIRQDisk2.prg goto error

del TestIRQDisk3.prg
..\acme.exe -v4 --msvc --lib ..\ TestIRQDisk3.a
if not exist TestIRQDisk3.prg goto error

del SmallestBoot.prg
..\acme.exe -v4 --pdb SmallestBoot.pdb --msvc -f cbm -o SmallestBoot.prg SmallestBoot.a
if not exist SmallestBoot.prg goto error


goto end

:error
echo TestIRQDisk.prg not created!
exit -1
:end

..\ExternalTools\C1541\c1541.exe -interleave 15 -attach test.d64 -format test,67 -write SmallestBoot.prg boot -write TestIRQDisk.prg testirqdisk -write screen1.prg screen1 -write screen2.prg screen2 -write screen3.prg screen3 -write screen1.cmp screen1cmp -write screen2.cmp screen2cmp -write screen3.cmp screen3cmp -write screen4.cmp screen4cmp -write TESTSAVE.PRG testsave
