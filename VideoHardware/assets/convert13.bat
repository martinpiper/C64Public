@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

echo Calculate palettes
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --minpalettesize 16 --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --resetforcergb --numbitplanes 4 --image "ColourWheel1.png" --imagequantize 64 --tilewh 8 8 --outputpalettes ../tmp/Demo13PaletteData.bin --convertwritepass


echo Convert chars
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --splitmaps --palettesize 16 --rgbshift 4 4 4 --nostacking --newpalettes --loadpaletteraw ../tmp/Demo13PaletteData.bin --chars --resetforcergb --numbitplanes 4 --image "ColourWheel1.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo13Chars_plane --outputscrcol ../tmp/Demo13Chars_map.bin --convertwritepass

popd
