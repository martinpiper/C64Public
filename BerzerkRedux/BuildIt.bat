@echo off
del BerzerkRedux.prg

..\acme.exe --lib ..\ -v3 --msvc BerzerkRedux.a
if not exist BerzerkRedux.prg goto error
..\bin\LZMPi.exe -c64 BerzerkRedux.prg BerzerkRedux.prg $400 >t.txt
if not exist BerzerkRedux.prg goto error
goto end
:error
echo BerzerkRedux.prg not created!
exit -1
:end
