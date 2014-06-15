@echo off
del HelpViewer.prg
..\bin\CharPack.exe -f TimesNewRoman10.bmp 1 TimesNewRoman10.cft
..\acme.exe -v3 --msvc HelpViewer.a
if not exist HelpViewer.prg goto error
..\bin\LZMPi.exe -c64b HelpViewer.prg HelpViewer.prg $400 >t.txt
if not exist HelpViewer.prg goto error
goto end
:error
echo HelpViewer.prg not created!
exit -1
:end
