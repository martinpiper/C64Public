pushd data

rem RawFrames3 is hires single colour
rem RawFrames5 uses colour data as well so it isn't forced to use multi-colour

rem ..\..\bin\CharPack.exe -m ..\RawFrames\%1.bmp 0 12 15 %1.chr %1.scr %1.col
rem ..\..\bin\CharPack.exe -m ..\RawFrames2\%1.bmp 0 12 15 %1.chr %1.scr %1.col
rem ..\..\bin\CharPack.exe -r ..\RawFrames3\%1.bmp 0 12 15 %1.chr %1.scr %1.col
rem ..\..\bin\CharPack.exe -m ..\RawFrames4\%1.bmp 0 12 14 %1.chr %1.scr %1.col
rem ..\..\bin\CharPack.exe -c64remap 7 15 -c64remap 3 11 -c64remap 9 11 -c64remap 15 12 ..\RawFrames5\%1.bmp 1 11 12 %1.chr %1.scr %1.col
rem ..\..\bin\CharPack.exe -c64remap 9 11 -c64remap 15 12 -m ..\RawFrames5\%1.bmp 1 11 12 %1.chr %1.scr %1.col
rem ..\..\bin\CharPack.exe -r ..\RawFrames6\%1.bmp 0 12 14 %1.chr %1.scr %1.col
rem ..\..\bin\CharPack.exe -r ..\RawFrames7\%1.bmp 1 12 14 %1.chr %1.scr %1.col

..\..\bin\CharPack.exe -r C:\Work\C64\AnimationBitmap\Animation10\%1.bmp 0 1 0 %1.chr %1.scr %1.col

rem Just use the character screen, no colour
rem copy /y %1.scr /b + ..\24Bytes.bin /b + %1.chr /b %1.sch
rem When using RawFrames5 include the colour screen before the character screen
rem copy /y %1.col /b + ..\24Bytes.bin /b + %1.scr /b + ..\24Bytes.bin /b + %1.chr /b %1.sch

rem ..\..\Compression\Release\Compression.exe -cr %1.sch %1.rle
rem del %1.chr %1.scr %1.col %1.sch
rem del %1.chr %1.scr %1.col

popd
