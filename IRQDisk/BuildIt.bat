@echo off
del TestIRQDisk2.prg
..\acme.exe -v3 --msvc TestIRQDisk2.a
if not exist TestIRQDisk.prg goto error
..\bin\LZMPi.exe -c64 TestIRQDisk.prg TestIRQDisk.prg $c000 >t.txt
if not exist TestIRQDisk.prg goto error
goto end
:error
echo TestIRQDisk.prg not created!
:end
