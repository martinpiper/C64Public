@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

rem c:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe "C:\Users\Martin Piper\Downloads\32X32-F6.png" -sample 160x96! "C:\Users\Martin Piper\Downloads\16X16-F6.png"
rem c:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe "C:\Users\Martin Piper\Downloads\tristarf.png" -sample 240x144! "C:\Users\Martin Piper\Downloads\tristarf_2.png"

echo Calculate palettes
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "Demo9/sprites1.png" --tilewh 16 16 --imagequantize 16 --nowritepass --image "Demo9/sprites2.png" --tilewh 32 32 --imagequantize 16 --nowritepass --image "Demo9/chars.png" --imagequantize 20 --tilewh 8 8 --nowritepass --image "Demo9/tiles.png" --tilewh 16 16 --outputpalettes ../tmp/Demo9PaletteData.bin --convertwritepass


echo Convert tiles
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo9PaletteData.bin --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "Demo9/tiles.png" --tilewh 16 16 --fitpalettes --outputplanes ../tmp/Demo9Tiles_plane --outputscrcol ../tmp/Demo9Tiles_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Tiles_map.bin ..\tmp\Demo9Tiles_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Tiles_plane0.bin ..\tmp\Demo9Tiles_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Tiles_plane1.bin ..\tmp\Demo9Tiles_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Tiles_plane2.bin ..\tmp\Demo9Tiles_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Tiles_plane3.bin ..\tmp\Demo9Tiles_plane3.cmp


echo Convert chars
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --splitmaps --paletteoffset 0 --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo9PaletteData.bin --chars --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "Demo9/chars.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo9Chars_plane --outputscrcol ../tmp/Demo9Chars_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars_map.bin ..\tmp\Demo9Chars_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars_map.bin2 ..\tmp\Demo9Chars_map.cmp2
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars_plane0.bin ..\tmp\Demo9Chars_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars_plane1.bin ..\tmp\Demo9Chars_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars_plane2.bin ..\tmp\Demo9Chars_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars_plane3.bin ..\tmp\Demo9Chars_plane3.cmp


echo Convert sprites
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo9PaletteData.bin --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "Demo9/sprites1.png" --tilewh 16 16 --fitpalettes --outputplanes ../tmp/Demo9Sprites1 --outputsprites ../tmp/Demo9Sprites1Sheet.txt --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites10.bin ..\tmp\Demo9Sprites10.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites11.bin ..\tmp\Demo9Sprites11.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites12.bin ..\tmp\Demo9Sprites12.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites13.bin ..\tmp\Demo9Sprites13.cmp

echo Convert scaled sprites
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo9PaletteData.bin --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "Demo9/sprites2.png" --tilewh 32 32 --fitpalettes --outputscaled ../tmp/Demo9Sprites2 --outputsprites ../tmp/Demo9Sprites2Sheet.txt --namesuffix _scaled_  --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites20.bin ..\tmp\Demo9Sprites20.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites21.bin ..\tmp\Demo9Sprites21.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites22.bin ..\tmp\Demo9Sprites22.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites23.bin ..\tmp\Demo9Sprites23.cmp

echo Convert music
cd ..\tmp
if not exist target mkdir target
java.exe -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod "..\assets\fnkyldrt.mod" "target/MusicMW2000" 1 1
rem java.exe -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --playmod "target/MusicMW2000"

popd
