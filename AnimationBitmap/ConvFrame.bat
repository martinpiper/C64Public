pushd data

rem ..\..\bin\CharPack.exe -bitmap -m ..\Animation1\%1.bmp 0 0 0 %1.chr %1.scr %1.col
rem ..\..\bin\CharPack.exe -bitmap -m ..\Animation2\%1.bmp 1 0 0 %1.chr %1.scr %1.col
..\..\bin\CharPack.exe -bitmap -m ..\Animation3\%1.bmp 0 0 0 %1.chr %1.scr %1.col

copy /y %1.col /b + ..\24Bytes.bin /b + %1.scr /b + ..\24Bytes.bin /b + ..\1024Bytes.bin /b + ..\1024Bytes.bin /b + ..\1024Bytes.bin /b + ..\1024Bytes.bin /b + ..\1024Bytes.bin /b + %1.chr /b %1.sch

popd
