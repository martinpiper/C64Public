rem ..\acme.exe --lib  ../ -v4 --msvc TapeStreamSamples.a
..\acme.exe --lib  ../ -v4 --msvc TapeStreamSamplesIRQ.a
..\bin\LZMPi.exe -c64bmu TapeStreamSamples.prg TapeStreamSamples.prg $400

..\acme.exe --lib  ../ -v4 --msvc TapeStreamSamplesConfig.a TapeTool\Loaders.a

rem This must match kStartCycleOffset and kPerVolumeCycles used in the code
..\bin\TapeTool.exe wn "vice.tap" q c:\temp\t.wav 22050 $4e 965536 48 16 985248 c

..\bin\TapeTool.exe n 1 wn "viceautorun.tap" m "Loaders.map" ocb1 Loaders.bin otl$c0 otft "TapeStreamSamples.prg" .RealCodeStart .RealCodeEnd q c:\temp\t.wav 22050 $4e 965536 48 16 985248 c

C:\Downloads\WinVICE-3.1-x86-r34062\WinVICE-3.1-x86-r34062\x64sc.exe -remotemonitor -dstapewobble 0 -dsspeedtuning 0 -1 vice.tap TapeStreamSamples.prg
rem C:\Downloads\WinVICE-3.1-x86-r34062\WinVICE-3.1-x86-r34062\x64sc.exe -remotemonitor -dstapewobble 0 -dsspeedtuning 0 viceautorun.tap

