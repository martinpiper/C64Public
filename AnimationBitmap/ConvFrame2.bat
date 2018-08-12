pushd data

rem ..\..\bin\CharPack.exe -addscreeninfo -bitmap -m ..\%2\%1.bmp %3 0 0 %1.chr %1.scr %1.col
..\..\bin\CharPack.exe -forceordering -addscreeninfo -addsheetinfo -sheetthresholdbase 0 -sheettargetnumspritesmax 8 -sheetframes %1.spr -1 -sheetinfo %1.sno -1 48 -sheetcolours %3 %4 %5 -bitmap -m ..\%2\%1.bmp %3 0 0 %1.chr %1.scr %1.col

copy /y %1.scr t.scr
copy /y %1.col t.col

popd
