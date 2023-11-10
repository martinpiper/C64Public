..\acme.exe --lib  ../ -v4 --msvc TapeStreamSamples.a
..\bin\LZMPi.exe -c64bmu TapeStreamSamples.prg TapeStreamSamples.prg $400
..\bin\TapeTool.exe wn "vice.tap" q c:\temp\t.wav 22050 $4e 965536 64 16 985248 c
C:\Downloads\WinVICE-3.1-x86-r34062\WinVICE-3.1-x86-r34062\x64sc.exe -remotemonitor -dstapewobble 0 -dsspeedtuning 0 -1 vice.tap TapeStreamSamples.prg
