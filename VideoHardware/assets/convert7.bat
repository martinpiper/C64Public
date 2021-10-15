@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

echo Calculate optimal separate palettes and combine
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --resetforcergb --forcergb 63 72 204 --numbitplanes 4 --image "Demo7/foreground.png" --tilewh 16 16 --imagequantize 16 --outputpalettes ../tmp/Demo7PaletteData1.bin --convertwritepass
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --resetforcergb --numbitplanes 4 --image "Demo7/background.png" --tilewh 8 8 --imagequantize 16 --outputpalettes ../tmp/Demo7PaletteData2.bin --convertwritepass
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --usestacking --newpalettes --resetforcergb --forcergb 63 72 204 --numbitplanes 4 --image "Demo7/sprites1.png" --tilewh 16 16 --imagequantize 32 --outputpalettes ../tmp/Demo7PaletteData3.bin --convertwritepass
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --usestacking --newpalettes --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "RPG status 512.png" --tilewh 8 8 --imagequantize 16 --outputpalettes ../tmp/Demo7PaletteData4.bin --convertwritepass
copy /b /y ..\tmp\Demo7PaletteData1.bin + ..\tmp\Demo7PaletteData2.bin + ..\tmp\Demo7PaletteData3.bin + ..\tmp\Demo7PaletteData4.bin ..\tmp\Demo7PaletteData.bin

rem tiles = 5 palettes
rem chars = 3 palettes
rem sprites = 5 palettes

echo Convert tiles
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo7PaletteData1.bin --resetforcergb --forcergb 63 72 204 --numbitplanes 4 --image "Demo7/foreground.png" --tilewh 16 16 --fitpalettes --outputplanes ../tmp/Demo7Tiles_plane --outputscrcol ../tmp/Demo7Tiles_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Tiles_map.bin ..\tmp\Demo7Tiles_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Tiles_plane0.bin ..\tmp\Demo7Tiles_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Tiles_plane1.bin ..\tmp\Demo7Tiles_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Tiles_plane2.bin ..\tmp\Demo7Tiles_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Tiles_plane3.bin ..\tmp\Demo7Tiles_plane3.cmp


echo Convert chars
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 5 --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo7PaletteData2.bin --chars --resetforcergb --numbitplanes 4 --image "Demo7/background.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo7Chars_plane --outputscrcol ../tmp/Demo7Chars_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars_map.bin ..\tmp\Demo7Chars_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars_plane0.bin ..\tmp\Demo7Chars_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars_plane1.bin ..\tmp\Demo7Chars_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars_plane2.bin ..\tmp\Demo7Chars_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars_plane3.bin ..\tmp\Demo7Chars_plane3.cmp

echo Convert chars2
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 12 --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo7PaletteData4.bin --chars --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "RPG status 512.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo7Chars2_plane --outputscrcol ../tmp/Demo7Chars2_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars2_map.bin ..\tmp\Demo7Chars2_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars2_plane0.bin ..\tmp\Demo7Chars2_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars2_plane1.bin ..\tmp\Demo7Chars2_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars2_plane2.bin ..\tmp\Demo7Chars2_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars2_plane3.bin ..\tmp\Demo7Chars2_plane3.cmp


echo Convert sprites
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 7 --palettesize 16 --rgbshift 4 4 4 --usestacking --newpalettes --loadpaletteraw ../tmp/Demo7PaletteData3.bin --resetforcergb --forcergb 63 72 204 --numbitplanes 4 --image "Demo7/sprites1.png" --tilewh 16 16 --imagequantize 32 --fitpalettes --outputplanes ../tmp/Demo7Sprites_plane --outputsprites ../tmp/Demo7SpritesSheet.txt --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Sprites_plane0.bin ..\tmp\Demo7Sprites_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Sprites_plane1.bin ..\tmp\Demo7Sprites_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Sprites_plane2.bin ..\tmp\Demo7Sprites_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Sprites_plane3.bin ..\tmp\Demo7Sprites_plane3.cmp

popd
