@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

rem c:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe "C:\Users\Martin Piper\Downloads\32X32-F6.png" -sample 160x96! "C:\Users\Martin Piper\Downloads\16X16-F6.png"
rem c:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe "C:\Users\Martin Piper\Downloads\tristarf.png" -sample 240x144! "C:\Users\Martin Piper\Downloads\tristarf_2.png"

echo Concat tiles
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "Demo9/tiles.png" "Demo9/RenderFloor/out/floor.png" "../tmp/Demo9Tiles.png"


echo Calculate palettes
rem These palettes are all squished together into one with no separation
rem java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "Demo9/chars.png" --imagequantize 20 --tilewh 8 8 --nowritepass --image "Demo9/sprites1.png" --tilewh 16 16 --imagequantize 16 --nowritepass --image "Demo9/sprites2.png" --tilewh 32 32 --imagequantize 16 --nowritepass --image "../tmp/Demo9Tiles.png" --tilewh 16 16 --nowritepass --rgbshift 5 6 5 --resetforcergb --forcergb 63 72 204 --image "Demo9\foreground2.png" --imagequantize 16 --tilewh 8 8 --outputpalettes ../tmp/Demo9PaletteData.bin --convertwritepass
rem Individual counts show that converting as one large palette is more optimal
rem chars = 5
rem sprites = 1
rem sprites2 = 7
rem tiles = 3
rem chars2 = 1

rem Just for chars palette, test conversions...
rem 5 palettes - As above
rem java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "Demo9/chars.png" --imagequantize 20 --tilewh 8 8 --nowritepass --rgbshift 5 6 5 --outputpalettes ../tmp/Demo9PaletteData.bin --convertwritepass
rem 6 palettes? - No --forcergb 255 0 255
rem java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --resetforcergb --numbitplanes 4 --image "Demo9/chars.png" --imagequantize 20 --tilewh 8 8 --nowritepass --rgbshift 5 6 5 --outputpalettes ../tmp/Demo9PaletteData.bin --convertwritepass
rem 13 palettes - Using --rgbshift 5 6 5. Also results in loss of the bright blue highlights
rem java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --resetforcergb --numbitplanes 4 --image "Demo9/chars.png" --imagequantize 20 --tilewh 8 8 --nowritepass --rgbshift 5 6 5 --outputpalettes ../tmp/Demo9PaletteData.bin --convertwritepass
rem Still results in the loss of highlights, although the colour graduations looks very subtle.
rem java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --resetforcergb --numbitplanes 4 --image "Demo9/chars.png" --imagequantize 30 --tilewh 8 8 --imagequantize 16 --nowritepass --rgbshift 5 6 5 --palettequantize 16 --outputpalettes ../tmp/Demo9PaletteData.bin --convertwritepass

rem Removes transparency for background characters
rem java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --resetforcergb --numbitplanes 4 --image "Demo9/chars.png" --imagequantize 20 --tilewh 8 8 --nowritepass --palettequantize 5 --resetforcergb --forcergb 255 0 255 --image "Demo9/sprites1.png" --tilewh 16 16 --imagequantize 16 --nowritepass --image "Demo9/sprites2.png" --tilewh 32 32 --imagequantize 16 --nowritepass --image "../tmp/Demo9Tiles.png" --tilewh 16 16 --nowritepass --rgbshift 5 6 5 --resetforcergb --forcergb 63 72 204 --image "Demo9\foreground2.png" --imagequantize 16 --tilewh 8 8 --nowritepass --outputpalettes ../tmp/Demo9PaletteData.bin --convertwritepass
rem Also uses RGB 565 for all of the assests except the background chars
rem java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --resetforcergb --numbitplanes 4 --image "Demo9/chars.png" --imagequantize 20 --tilewh 8 8 --imagequantize 16 --nowritepass --palettequantize 5 --rgbshift 5 6 5 --resetforcergb --forcergb 255 0 255 --image "Demo9/sprites1.png" --tilewh 16 16 --imagequantize 16 --nowritepass --image "Demo9/sprites2.png" --tilewh 32 32 --imagequantize 16 --nowritepass --image "../tmp/Demo9Tiles.png" --tilewh 16 16 --nowritepass --resetforcergb --forcergb 63 72 204 --image "Demo9\foreground2.png" --imagequantize 16 --tilewh 8 8 --nowritepass --rgbshift 5 6 5 --palettequantize 16 --outputpalettes ../tmp/Demo9PaletteData.bin --convertwritepass
rem This increases the chars import to use RGB565 and increases the number of palettes for the background, since it has more subtle shades, but it potentially reduces the palettes to the rest of the assets. It also takes a long time to reduce the palettes.
rem However comparing the debug output png files (_pre.png and .png) during the conversion stage shows no pixel colours are dropped in the sprites and tiles. Only the background chars which actually shows improved colour matching.
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --resetforcergb --numbitplanes 4 --image "Demo9/chars.png" --imagequantize 30 --tilewh 8 8 --imagequantize 16 --nowritepass --palettequantize 10 --rgbshift 5 6 5 --resetforcergb --forcergb 255 0 255 --image "Demo9/sprites1.png" --tilewh 16 16 --imagequantize 16 --nowritepass --image "Demo9/sprites2.png" --tilewh 32 32 --imagequantize 16 --nowritepass --image "../tmp/Demo9Tiles.png" --tilewh 16 16 --nowritepass --resetforcergb --forcergb 63 72 204 --image "Demo9\foreground2.png" --imagequantize 16 --tilewh 8 8 --nowritepass --rgbshift 5 6 5 --palettequantize 16 --outputpalettes ../tmp/Demo9PaletteData.bin --convertwritepass

echo Convert tiles
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo9PaletteData.bin --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "../tmp/Demo9Tiles.png" --tilewh 16 16 --fitpalettes --outputplanes ../tmp/Demo9Tiles_plane --outputscrcol ../tmp/Demo9Tiles_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Tiles_map.bin ..\tmp\Demo9Tiles_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Tiles_plane0.bin ..\tmp\Demo9Tiles_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Tiles_plane1.bin ..\tmp\Demo9Tiles_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Tiles_plane2.bin ..\tmp\Demo9Tiles_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Tiles_plane3.bin ..\tmp\Demo9Tiles_plane3.cmp


echo Convert chars - As background with no transparency
rem java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --splitmaps --paletteoffset 0 --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo9PaletteData.bin --chars --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "Demo9/chars.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo9Chars_plane --outputscrcol ../tmp/Demo9Chars_map.bin --convertwritepass
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --splitmaps --paletteoffset 0 --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo9PaletteData.bin --chars --resetforcergb --numbitplanes 4 --image "Demo9/chars.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo9Chars_plane --outputscrcol ../tmp/Demo9Chars_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars_map.bin ..\tmp\Demo9Chars_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars_map.bin2 ..\tmp\Demo9Chars_map.cmp2
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars_plane0.bin ..\tmp\Demo9Chars_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars_plane1.bin ..\tmp\Demo9Chars_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars_plane2.bin ..\tmp\Demo9Chars_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars_plane3.bin ..\tmp\Demo9Chars_plane3.cmp

echo Convert chars2
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --splitmaps --paletteoffset 0 --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo9PaletteData.bin --chars --resetforcergb --forcergb 63 72 204 --numbitplanes 4 --image "Demo9/foreground2.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo9Chars2_plane --outputscrcol ../tmp/Demo9Chars2_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars2_map.bin ..\tmp\Demo9Chars2_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars2_map.bin2 ..\tmp\Demo9Chars2_map.cmp2
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars2_plane0.bin ..\tmp\Demo9Chars2_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars2_plane1.bin ..\tmp\Demo9Chars2_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars2_plane2.bin ..\tmp\Demo9Chars2_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Chars2_plane3.bin ..\tmp\Demo9Chars2_plane3.cmp


echo Convert sprites
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo9PaletteData.bin --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "Demo9/sprites1.png" --tilewh 16 16 --fitpalettes --outputplanes ../tmp/Demo9Sprites1 --outputsprites ../tmp/Demo9Sprites1Sheet.txt --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites10.bin ..\tmp\Demo9Sprites10.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites11.bin ..\tmp\Demo9Sprites11.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites12.bin ..\tmp\Demo9Sprites12.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites13.bin ..\tmp\Demo9Sprites13.cmp

echo Convert scaled sprites
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo9PaletteData.bin --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "Demo9/sprites2.png" --tilewh 32 32 --fitpalettes --outputscaled ../tmp/Demo9Sprites2 --outputsprites ../tmp/Demo9Sprites2Sheet.txt --namesuffix _scaled_  --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites20.bin ..\tmp\Demo9Sprites20.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites21.bin ..\tmp\Demo9Sprites21.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites22.bin ..\tmp\Demo9Sprites22.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo9Sprites23.bin ..\tmp\Demo9Sprites23.cmp

echo Convert music
cd ..\tmp
if not exist target mkdir target
java.exe -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod "..\assets\fnkyldrt.mod" "target/MusicMW2000" 1 1
rem java.exe -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod "C:\Users\Martin Piper\Downloads\lotus2-title.mod" "target/MusicMW2000" 65535 66858
rem java.exe -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --playmod "target/MusicMW2000"
..\..\bin\LZMPi.exe -cut target\MusicMW2000Samples.bin target\MusicMW2000Samples.bin1 0 32768
..\..\bin\LZMPi.exe -cut target\MusicMW2000Samples.bin target\MusicMW2000Samples.bin2 32768
..\..\bin\LZMPi.exe -cr target\MusicMW2000Samples.bin1 target\MusicMW2000Samples.cmp1
..\..\bin\LZMPi.exe -cr target\MusicMW2000Samples.bin2 target\MusicMW2000Samples.cmp2


java.exe -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod "..\assets\Demo9\spacehar.mod" "target/MusicMW2000_game" 1 1
rem java.exe -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --playmod "target/MusicMW2000_game"

popd
