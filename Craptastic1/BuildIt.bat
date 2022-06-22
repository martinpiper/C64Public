@echo off
del Craptastic1.prg

echo Exporting music...
"%cd%\..\MusicStudio2\MusicStudio\Release\MusicStudio.exe" /hide /openFile "%cd%\music.msmus" /pressEnvelopeForceUsedFX 09 /pressEnvelopeForceUsedFX 0A /pressEnvelopeForceUsedFX 0B /exportToC64 $4000 y "%cd%\music.prg" /exit
rem "%cd%\..\MusicStudio2\MusicStudio\Release\MusicStudio.exe" /hide /openFile "%cd%\music.msmus" /exportToC64 $4000 n "%cd%\music.prg" /exit

echo Building code...
..\acme.exe --lib ..\ -v4 --msvc Craptastic1.a
if not exist Craptastic1.prg goto error
..\bin\LZMPi.exe -c64m Craptastic1.prg Craptastic1.prg $900
if not exist Craptastic1.prg goto error

echo Making the tape...
mkdir tmp
cd tmp

del /q Loaders.bin
..\..\acme.exe --lib  ../ --lib ../../  --lib ../../IRQTape/ -v4 --msvc ..\_config.a  ..\..\IRQTape\TapeTool\Loaders.a
..\..\bin\TapeTool.exe wn "..\test.tap" m "Loaders.map" ocb1 Loaders.bin otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd otl$c0 otfbr "..\Craptastic1.prg" 0 otl$c0 c

cd ..



goto end
:error
echo Craptastic1.prg not created!
exit /B -1
:end
