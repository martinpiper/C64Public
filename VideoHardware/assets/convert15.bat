@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

echo Calculate palettes
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --numbitplanes 4 --rgbshift 5 6 5 --nostacking --newpalettes --resetforcergb --forcergb 63 72 204 --image "Demo9\foreground2.png" --tilewh 8 8 --imagequantize 16 --nowritepass --resetforcergb --forcergb 255 0 255 --image "ShadowBeast/game foreground.png" --tilewh 8 8 --imagequantize 16 --outputpalettes ../tmp/Demo15PaletteData.bin --convertwritepass


rem java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --splitmaps --paletteoffset 0 --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo15PaletteData.bin --chars --resetforcergb --forcergb 63 72 204 --numbitplanes 4 --image "Demo9/foreground2.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo15Chars2_plane --outputscrcol ../tmp/Demo15Chars2_map.bin --convertwritepass

echo Convert chars1
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --numbitplanes 4 --rgbshift 5 6 5 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo15PaletteData.bin --chars --resetforcergb --forcergb 63 72 204 --image "Demo9/foreground2.png" --tilewh 8 8 --fitpalettes --splitmaps --outputplanes ../tmp/Demo15Chars1_plane --outputscrcol ../tmp/Demo15Chars1_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo15Chars1_map.bin    ..\tmp\Demo15Chars1_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo15Chars1_map.bin2   ..\tmp\Demo15Chars1_map.cmp2
..\..\bin\LZMPi.exe -cr ..\tmp\Demo15Chars1_plane0.bin ..\tmp\Demo15Chars1_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo15Chars1_plane1.bin ..\tmp\Demo15Chars1_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo15Chars1_plane2.bin ..\tmp\Demo15Chars1_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo15Chars1_plane3.bin ..\tmp\Demo15Chars1_plane3.cmp

echo Convert chars2
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --numbitplanes 4 --rgbshift 5 6 5 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo15PaletteData.bin --chars --resetforcergb --forcergb 255 0 255 --image "ShadowBeast/game foreground.png" --tilewh 8 8 --fitpalettes --splitmaps --outputplanes ../tmp/Demo15Chars2_plane --outputscrcol ../tmp/Demo15Chars2_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\Demo15Chars2_map.bin    ..\tmp\Demo15Chars2_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo15Chars2_map.bin2   ..\tmp\Demo15Chars2_map.cmp2
..\..\bin\LZMPi.exe -cr ..\tmp\Demo15Chars2_plane0.bin ..\tmp\Demo15Chars2_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo15Chars2_plane1.bin ..\tmp\Demo15Chars2_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo15Chars2_plane2.bin ..\tmp\Demo15Chars2_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\Demo15Chars2_plane3.bin ..\tmp\Demo15Chars2_plane3.cmp

popd
