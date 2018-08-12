rem Takes parameters: <filename> <directory> <background colour> <sprite multicolour 1> <sprite multicolour 2> <sprite threshold base>

pushd data

rem ..\..\bin\CharPack.exe -addscreeninfo -addsheetinfo -sheetthresholdbase %6 -sheetframes %1.spr -1 -sheetinfo %1.sno -1 48 -sheetcolours %3 %4 %5 -bitmap -m ..\%2\%1.bmp %3 0 0 %1.chr %1.scr %1.col
..\..\bin\CharPack.exe -forceordering -spriteminy 0 -spritemaxy 75 -spriteminx 80 -spritemaxx 216 -addscreeninfo -addsheetinfo -sheetthresholdbase %6 -sheettargetnumspritesmax 8 -sheetframes %1.spr -1 -sheetinfo %1.sno -1 48 -sheetcolours %3 %4 %5 -bitmap -m ..\%2\%1.bmp %3 0 0 %1.chr %1.scr %1.col

rem -forceordering -reducebitmapchange t.scr t.col

copy /y %1.scr t.scr
copy /y %1.col t.col


popd
