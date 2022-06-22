@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

echo Calculate optimal separate palettes and combine
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --minpalettesize 16 --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --resetforcergb --forcergb 83 103 141 --numbitplanes 4 --image "Demo10/background.png" --tilewh 16 16 --imagequantize 16 --outputpalettes ../tmp/Demo10PaletteData1.bin --convertwritepass
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --minpalettesize 16 --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --resetforcergb --forcergb 83 103 141 --numbitplanes 4 --image "Demo10/background2.png" --imagequantize 23 --tilewh 8 8 --imagequantize 15 --outputpalettes ../tmp/Demo10PaletteData2.bin --convertwritepass
rem SF2 Cammy https://www.spriters-resource.com/arcade/streetfighter2/sheet/117307/
set REGIONS=--regionshift --regionxy cammy_idle1 45 209 1 114 80 209 --regionxy cammy_idle2 126 209 82 114 161 209 --regionxy cammy_idle3 207 209 163 114 242 209 --regionxy cammy_idle4 288 209 244 114 323 209 --regionxy cammy_idle5 369 209 325 114 404 209 --regionxy cammy_idle6 450 209 406 114 485 209 --regionxy cammy_idle7 531 209 487 114 566 209 --regionxy cammy_idle8 612 209 568 114 647 209
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --minpalettesize 16 --palettesize 16 --rgbshift 4 4 4 --usestacking --newpalettes --resetforcergb --forcergb 248 0 248 --numbitplanes 4 --image "Demo10/117307-nomask.png" --tilewh 16 16 %REGIONS% --imagequantize 32 --outputpalettes ../tmp/Demo10PaletteData3.bin --convertwritepass
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --minpalettesize 16 --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "RPG status 1024.png" --tilewh 8 8 --imagequantize 16 --outputpalettes ../tmp/Demo10PaletteData4.bin --convertwritepass
copy /b /y ..\tmp\Demo10PaletteData4.bin + ..\tmp\Demo10PaletteData1.bin + ..\tmp\Demo10PaletteData2.bin + ..\tmp\Demo10PaletteData3.bin ..\tmp\Demo10PaletteData.bin

rem 4: chars2 = 1 palettes
rem 1: tiles = 3 palettes
rem 2: chars = 2 palettes
rem 3: sprites = 5 palettes

echo Convert chars2
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --splitmaps --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo10PaletteData.bin --chars --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "RPG status 1024.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo10Chars2_plane --outputscrcol ../tmp/Demo10Chars2_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Chars2_map.bin ..\tmp\Demo10Chars2_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Chars2_map.bin2 ..\tmp\Demo10Chars2_map.cmp2
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Chars2_plane0.bin ..\tmp\Demo10Chars2_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Chars2_plane1.bin ..\tmp\Demo10Chars2_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Chars2_plane2.bin ..\tmp\Demo10Chars2_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Chars2_plane3.bin ..\tmp\Demo10Chars2_plane3.cmp

echo Convert tiles
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo10PaletteData.bin --resetforcergb --forcergb 83 103 141 --numbitplanes 4 --image "Demo10/background.png" --tilewh 16 16 --fitpalettes --outputplanes ../tmp/Demo10Tiles_plane --outputscrcol ../tmp/Demo10Tiles_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Tiles_map.bin ..\tmp\Demo10Tiles_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Tiles_plane0.bin ..\tmp\Demo10Tiles_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Tiles_plane1.bin ..\tmp\Demo10Tiles_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Tiles_plane2.bin ..\tmp\Demo10Tiles_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Tiles_plane3.bin ..\tmp\Demo10Tiles_plane3.cmp


echo Convert chars
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --splitmaps --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo10PaletteData.bin --chars --resetforcergb --forcergb 83 103 141 --numbitplanes 4 --image "Demo10/background2.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo10Chars_plane --outputscrcol ../tmp/Demo10Chars_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Chars_map.bin ..\tmp\Demo10Chars_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Chars_map.bin2 ..\tmp\Demo10Chars_map.cmp2
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Chars_plane0.bin ..\tmp\Demo10Chars_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Chars_plane1.bin ..\tmp\Demo10Chars_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Chars_plane2.bin ..\tmp\Demo10Chars_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Chars_plane3.bin ..\tmp\Demo10Chars_plane3.cmp

echo Convert sprites
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --palettesize 16 --rgbshift 4 4 4 --usestacking --newpalettes --loadpaletteraw ../tmp/Demo10PaletteData.bin --resetforcergb --forcergb 248 0 248 --numbitplanes 4 --image "Demo10/117307-nomask.png" --tilewh 16 16 %REGIONS% --imagequantize 32 --fitpalettes --outputplanes ../tmp/Demo10Sprites_plane --outputsprites ../tmp/Demo10SpritesSheet.txt --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Sprites_plane0.bin ..\tmp\Demo10Sprites_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Sprites_plane1.bin ..\tmp\Demo10Sprites_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Sprites_plane2.bin ..\tmp\Demo10Sprites_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo10Sprites_plane3.bin ..\tmp\Demo10Sprites_plane3.cmp

popd
