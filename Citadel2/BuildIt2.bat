@echo off
del SolveRunner.prg
..\acme.exe -v4 --msvc --lib ../Scroller/ SolveRunner.a
if not exist SolveRunner.prg goto error
..\bin\LZMPi.exe -c64b SolveRunner.prg SolveRunner.prg $800 >t.txt
if not exist SolveRunner.prg goto error
goto end
:error
echo SolveRunner.prg not created!
exit /B -1
:end
