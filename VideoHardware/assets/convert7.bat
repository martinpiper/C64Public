@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

echo Calculate optimal separate palettes and combine
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --resetforcergb --forcergb 63 72 204 --numbitplanes 4 --image "Demo7/foreground.png" --tilewh 16 16 --imagequantize 16 --outputpalettes ../tmp/Demo7PaletteData1.bin --convertwritepass
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --resetforcergb --numbitplanes 4 --image "Demo7/background.png" --tilewh 8 8 --imagequantize 16 --outputpalettes ../tmp/Demo7PaletteData2.bin --convertwritepass
set REGIONS=--regionshift --region redframe1 29 39 -1 -11 32 32 --region redframe2 70 40 -1 -11 32 32 --region redframe3 110 41 -1 -11 32 32 --region redframe4 152 40 -1 -11 32 32 --region redframe5 192 40 -1 -11 32 32 --region redframe6 229 40 -1 -11 32 32 --region bullet1 274 39 -8 -8 16 16 --region walker1 29 97 0 -31 32 32 --region walker2 70 97 0 -31 32 32 --region walker3 109 96 0 -31 32 32 --region rotate1 33 129 -8 -16 32 32 --region rotate2 74 127 -8 -16 32 32 --region rotate3 114 123 -8 -8 32 32 --region rotate4 159 122 -16 -8 32 32 --region boss1 77 749 -47 -32 96 80 --region boss2 200 745 -47 -32 96 80 --region boss3 312 745 -47 -32 96 80 --region boss4 423 745 -47 -32 96 80
rem set REGIONS=--regionshift --region redframe1 29 39 -1 -11 32 32 --region bullet1 274 39 -8 -8 16 16 --region walker1 29 97 0 -31 32 32 --region rotate1 33 129 -8 -16 32 32 --region boss1 77 749 -47 -32 96 80
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --usestacking --newpalettes --resetforcergb --forcergb 63 72 204 --numbitplanes 4 --image "Demo7/73181.png" --tilewh 16 16 %REGIONS% --imagequantize 32 --outputpalettes ../tmp/Demo7PaletteData3.bin --convertwritepass
rem java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --usestacking --newpalettes --resetforcergb --forcergb 248 0 248 --numbitplanes 4 --image "C:\Users\Martin Piper\Downloads\117307-nomask.png" --tilewh 16 16 %REGIONS% --imagequantize 32 --outputpalettes ../tmp/Demo7PaletteData3.bin --convertwritepass
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "RPG status 1024.png" --tilewh 8 8 --imagequantize 16 --outputpalettes ../tmp/Demo7PaletteData4.bin --convertwritepass
copy /b /y ..\tmp\Demo7PaletteData1.bin + ..\tmp\Demo7PaletteData2.bin + ..\tmp\Demo7PaletteData3.bin + ..\tmp\Demo7PaletteData4.bin ..\tmp\Demo7PaletteData.bin

rem 1: tiles = 5 palettes
rem 2: chars = 2 palettes
rem 3: sprites = 5 palettes
rem 4: chars2 = 1 palettes

echo Convert tiles
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo7PaletteData1.bin --resetforcergb --forcergb 63 72 204 --numbitplanes 4 --image "Demo7/foreground.png" --tilewh 16 16 --fitpalettes --outputplanes ../tmp/Demo7Tiles_plane --outputscrcol ../tmp/Demo7Tiles_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Tiles_map.bin ..\tmp\Demo7Tiles_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Tiles_plane0.bin ..\tmp\Demo7Tiles_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Tiles_plane1.bin ..\tmp\Demo7Tiles_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Tiles_plane2.bin ..\tmp\Demo7Tiles_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Tiles_plane3.bin ..\tmp\Demo7Tiles_plane3.cmp


echo Convert chars
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 5 --splitmaps --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo7PaletteData2.bin --chars --resetforcergb --numbitplanes 4 --image "Demo7/background.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo7Chars_plane --outputscrcol ../tmp/Demo7Chars_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars_map.bin ..\tmp\Demo7Chars_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars_map.bin2 ..\tmp\Demo7Chars_map.cmp2
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars_plane0.bin ..\tmp\Demo7Chars_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars_plane1.bin ..\tmp\Demo7Chars_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars_plane2.bin ..\tmp\Demo7Chars_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars_plane3.bin ..\tmp\Demo7Chars_plane3.cmp

echo Convert sprites
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 7 --palettesize 16 --rgbshift 4 4 4 --usestacking --newpalettes --loadpaletteraw ../tmp/Demo7PaletteData3.bin --resetforcergb --forcergb 63 72 204 --numbitplanes 4 --image "Demo7/73181.png" --tilewh 16 16 %REGIONS% --imagequantize 32 --fitpalettes --outputplanes ../tmp/Demo7Sprites_plane --outputsprites ../tmp/Demo7SpritesSheet.txt --convertwritepass
rem java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 7 --palettesize 16 --rgbshift 4 4 4 --usestacking --newpalettes --loadpaletteraw ../tmp/Demo7PaletteData3.bin --resetforcergb --forcergb 248 0 248 --numbitplanes 4 --image "C:\Users\Martin Piper\Downloads\117307-nomask.png" --tilewh 16 16 %REGIONS% --imagequantize 32 --fitpalettes --outputplanes ../tmp/Demo7Sprites_plane --outputsprites ../tmp/Demo7SpritesSheet.txt --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Sprites_plane0.bin ..\tmp\Demo7Sprites_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Sprites_plane1.bin ..\tmp\Demo7Sprites_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Sprites_plane2.bin ..\tmp\Demo7Sprites_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Sprites_plane3.bin ..\tmp\Demo7Sprites_plane3.cmp

echo Convert chars2
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 12 --splitmaps --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo7PaletteData4.bin --chars --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "RPG status 1024.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo7Chars2_plane --outputscrcol ../tmp/Demo7Chars2_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars2_map.bin ..\tmp\Demo7Chars2_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars2_map.bin2 ..\tmp\Demo7Chars2_map.cmp2
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars2_plane0.bin ..\tmp\Demo7Chars2_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars2_plane1.bin ..\tmp\Demo7Chars2_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars2_plane2.bin ..\tmp\Demo7Chars2_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo7Chars2_plane3.bin ..\tmp\Demo7Chars2_plane3.cmp


popd
