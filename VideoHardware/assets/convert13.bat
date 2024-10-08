@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

echo Calculate palettes
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --minpalettesize 16 --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --resetforcergb --numbitplanes 4 --image "ColourWheel1.png" --startxy 0 0 --imagewh 1024 64 --imagequantize 82 --tilewh 8 8 --outputpalettes ../tmp/Demo13PaletteData0.bin --convertwritepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --minpalettesize 16 --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --resetforcergb --numbitplanes 4 --image "ColourWheel1.png" --startxy 0 64 --imagewh 1024 128 --imagequantize 73 --tilewh 8 8 --outputpalettes ../tmp/Demo13PaletteData1.bin --convertwritepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --minpalettesize 16 --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --resetforcergb --numbitplanes 4 --image "ColourWheel1.png" --startxy 0 128 --imagewh 1024 192 --imagequantize 65 --tilewh 8 8 --outputpalettes ../tmp/Demo13PaletteData2.bin --convertwritepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --minpalettesize 16 --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --resetforcergb --numbitplanes 4 --image "ColourWheel1.png" --startxy 0 192 --imagewh 1024 512 --imagequantize 77 --tilewh 8 8 --outputpalettes ../tmp/Demo13PaletteData3.bin --convertwritepass




java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --minpalettesize 16 --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --resetforcergb --numbitplanes 4 --image "Demo13\AH_KingTut_aga.png" --imagequantize 2282 --tilewh 8 8 --imagequantize 16 --nowritepass --palettequantize 16 --outputpalettes ../tmp/Demo13PaletteData1-0.bin --convertwritepass


echo Convert chars
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --splitmaps --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo13PaletteData0.bin --chars --resetforcergb --numbitplanes 4 --image "ColourWheel1.png" --startxy 0 0 --imagewh 1024 64 --tilewh 8 8 --fitpalettes --outputplanes ../tmp/t --outputscrcol ../tmp/t.bin --convertpass --preservedata --newpalettes --loadpaletteraw ../tmp/Demo13PaletteData1.bin --removeregions --image "ColourWheel1.png" --startxy 0 64 --imagewh 1024 128 --tilewh 8 8 --fitpalettes --outputplanes ../tmp/t --outputscrcol ../tmp/t.bin --convertpass --newpalettes --loadpaletteraw ../tmp/Demo13PaletteData2.bin --removeregions --image "ColourWheel1.png" --startxy 0 128 --imagewh 1024 192 --tilewh 8 8 --fitpalettes --outputplanes ../tmp/t --outputscrcol ../tmp/t.bin --convertpass --newpalettes --loadpaletteraw ../tmp/Demo13PaletteData3.bin --removeregions --image "ColourWheel1.png" --startxy 0 192 --imagewh 1024 512 --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo13Chars_plane --outputscrcol ../tmp/Demo13Chars_map.bin --convertwritepass


java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --splitmaps --palettesize 16 --rgbshift 5 6 5 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo13PaletteData1-0.bin --chars --resetforcergb --numbitplanes 4 --image "Demo13\AH_KingTut_aga.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo13Chars1_plane --outputscrcol ../tmp/Demo13Chars1_map.bin --convertwritepass



echo Compress

..\..\bin\LZMPi.exe -cr ../tmp/Demo13PaletteData0.bin ../tmp/Demo13PaletteData0.cmp
..\..\bin\LZMPi.exe -cr ../tmp/Demo13PaletteData1.bin ../tmp/Demo13PaletteData1.cmp
..\..\bin\LZMPi.exe -cr ../tmp/Demo13PaletteData2.bin ../tmp/Demo13PaletteData2.cmp
..\..\bin\LZMPi.exe -cr ../tmp/Demo13PaletteData3.bin ../tmp/Demo13PaletteData3.cmp


..\..\bin\LZMPi.exe -cr ../tmp/Demo13Chars_plane0.bin ../tmp/Demo13Chars_plane0.cmp
..\..\bin\LZMPi.exe -cr ../tmp/Demo13Chars_plane1.bin ../tmp/Demo13Chars_plane1.cmp
..\..\bin\LZMPi.exe -cr ../tmp/Demo13Chars_plane2.bin ../tmp/Demo13Chars_plane2.cmp
..\..\bin\LZMPi.exe -cr ../tmp/Demo13Chars_plane3.bin ../tmp/Demo13Chars_plane3.cmp
..\..\bin\LZMPi.exe -cr ../tmp/Demo13Chars_map.bin ../tmp/Demo13Chars_map.cmp
..\..\bin\LZMPi.exe -cr ../tmp/Demo13Chars_map.bin2 ../tmp/Demo13Chars_map.cmp2



..\..\bin\LZMPi.exe -cr ../tmp/Demo13PaletteData1-0.bin ../tmp/Demo13PaletteData1-0.cmp
..\..\bin\LZMPi.exe -cr ../tmp/Demo13Chars1_plane0.bin ../tmp/Demo13Chars1_plane0.cmp
..\..\bin\LZMPi.exe -cr ../tmp/Demo13Chars1_plane1.bin ../tmp/Demo13Chars1_plane1.cmp
..\..\bin\LZMPi.exe -cr ../tmp/Demo13Chars1_plane2.bin ../tmp/Demo13Chars1_plane2.cmp
..\..\bin\LZMPi.exe -cr ../tmp/Demo13Chars1_plane3.bin ../tmp/Demo13Chars1_plane3.cmp
..\..\bin\LZMPi.exe -cr ../tmp/Demo13Chars1_map.bin ../tmp/Demo13Chars1_map.cmp
..\..\bin\LZMPi.exe -cr ../tmp/Demo13Chars1_map.bin2 ../tmp/Demo13Chars1_map.cmp2

popd
