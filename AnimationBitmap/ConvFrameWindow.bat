pushd data

..\..\bin\CharPack.exe -swizzle -window %4 %5 %6 %7 -forceordering -bitmap -m ..\%2\%1.bmp %3 0 0 win%1.chr win%1.scr win%1.col

copy /b /y %1.scr t.scr
copy /b /y %1.col t.col

rem Appends the swizzled data to one huge file
copy /b /y binaryData.bin+win%1.chr+win%1.scr tbinaryData.bin
copy /b /y tbinaryData.bin binaryData.bin

popd
