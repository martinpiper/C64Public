@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

rem Note -sample used
rem c:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe ShadowBeast\beast_attract_playfield_1.png -sample 256x256! ShadowBeast\beast_attract_playfield_1_256.png

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "ShadowBeast/just blimps.png" "ShadowBeast/just trees.png" "../tmp/sprites1.png"
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "ShadowBeast/just blimps.png" "ShadowBeast/ani_beast.png" "../tmp/sprites2.png"

echo Calculate optimal palette
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --newpalettes --chars --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "ShadowBeast/beast_attract_playfield_1_512.png" --tilewh 8 8 --nowritepass --image "ShadowBeast/game foreground.png" --tilewh 8 8 --nowritepass --nochars --image "../tmp/sprites1.png" --tilewh 16 16 --imagequantize 16 --nowritepass --image "../tmp/sprites2.png" --tilewh 16 16 --imagequantize 16 --nowritepass --image "ShadowBeast/Shadow of the Beast - Background 2.png" --tilewh 16 16 --imagequantize 16 --outputpalettes ../tmp/ShadowBeastPaletteData.bin --convertwritepass

echo Convert chars
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --chars --newpalettes --loadpalette ../tmp/ShadowBeastPaletteData.bin --numbitplanes 4 --image "ShadowBeast/beast_attract_playfield_1_512.png" --tilewh 8 8 --fitpalettes --nostacking --outputplanes ../tmp/ShadowBeastChars_plane --outputscrcol ../tmp/ShadowBeastChars_map.bin --convertwritepass

echo Convert chars2
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --chars --newpalettes --loadpalette ../tmp/ShadowBeastPaletteData.bin --numbitplanes 4 --image "ShadowBeast/game foreground.png" --tilewh 8 8 --fitpalettes --nostacking --outputplanes ../tmp/ShadowBeastChars2_plane --outputscrcol ../tmp/ShadowBeastChars2_map.bin --convertwritepass

echo Convert tiles
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --newpalettes --loadpalette ../tmp/ShadowBeastPaletteData.bin --numbitplanes 4 --image "ShadowBeast/Shadow of the Beast - Background 2.png" --tilewh 16 16 --fitpalettes --nostacking --outputplanes ../tmp/ShadowBeastTiles_plane --outputscrcol ../tmp/ShadowBeastTiles_map.bin --convertwritepass


echo Convert sprites
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --newpalettes --loadpalette ../tmp/ShadowBeastPaletteData.bin --numbitplanes 4 --image "../tmp/sprites1.png" --tilewh 16 16 --fitpalettes --nostacking --outputplanes ../tmp/ShadowBeastSprites_plane --outputsprites ../tmp/ShadowBeastSpritesSheet.txt --convertwritepass

echo Convert sprites 2
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --newpalettes --loadpalette ../tmp/ShadowBeastPaletteData.bin --numbitplanes 4 --image "../tmp/sprites2.png" --tilewh 16 16 --fitpalettes --nostacking --outputplanes ../tmp/ShadowBeastSprites2_plane --outputsprites ../tmp/ShadowBeastSprites2Sheet.txt --convertwritepass

echo Convert music
cd ..\tmp
if not exist target mkdir target
java.exe -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod "..\assets\SOTB - TITLE.mod" "target/SotBexportedMusic" 1 1

java.exe -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod "..\assets\SOTB - EXPLORATION.mod" "target/SotBexportedMusic2" 65535 71482

popd
