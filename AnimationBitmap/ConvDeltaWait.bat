@echo off

SETLOCAL EnableExtensions
set EXE=DeltaCompression.exe
:LOOPSTART
FOR /F %%x IN ('tasklist /NH /FI "IMAGENAME eq %EXE%"') DO IF %%x == %EXE% goto FOUND
goto FIN
:FOUND
TIMEOUT /T 10
goto LOOPSTART
:FIN
