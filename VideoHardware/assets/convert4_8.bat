@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

echo Calculate the best palette for the clouds, no --imagequantize 8
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 4 4 4 --newpalettes --forcergb 255 0 255 --palettesize 256 --image "clouds.png" --tilewh 16 16 --outputpalettes ../tmp/PaletteDataClouds.bin --nostacking --convertwritepass



echo Calculate optimal palette
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --palettesize 8 --rgbshift 4 4 4 --newpalettes --resetforcergb --forcergb 255 0 255 --numbitplanes 3 --image "RPG status.png" --tilewh 8 8 --imagequantize 8 --nowritepass --numbitplanes 3 --image "Turrican/player.png" --tilewh 16 16 --imagequantize 8 --nowritepass --numbitplanes 3 --image "Turrican/1-1 trim.png" --tilewh 16 16 --imagequantize 8 --nostacking --nowritepass --loadpalettebestfit ../tmp/PaletteDataClouds.bin --outputpalettes ../tmp/TurricanPaletteData.bin --convertwritepass



echo Convert sprites
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --palettesize 8 --rgbshift 4 4 4 --newpalettes --loadpalette ../tmp/TurricanPaletteData.bin --numbitplanes 3 --image "Turrican/player.png" --tilewh 16 16 --fitpalettes --nostacking --outputplanes ../tmp/TurricanSprites_plane --outputsprites ../tmp/TurricanSpritesSheet.txt --convertwritepass


echo Convert tiles
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --palettesize 8 --rgbshift 4 4 4 --newpalettes --loadpalette ../tmp/TurricanPaletteData.bin --numbitplanes 3 --image "Turrican/1-1 trim.png" --tilewh 16 16 --fitpalettes --nostacking --outputplanes ../tmp/TurricanTiles_plane --outputscrcol ../tmp/TurricanTiles_map.bin --convertwritepass


echo Convert the chars
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --paletteoffset 0 --palettesize 8 --rgbshift 4 4 4 --newpalettes --loadpalette ../tmp/TurricanPaletteData.bin --chars --numbitplanes 3 --image "RPG status.png" --tilewh 8 8 --fitpalettes --nostacking --outputplanes ../tmp/TurricanStatus_plane --outputscrcol ../tmp/TurricanStatus_map.bin --convertwritepass


echo Convert the clouds with new palette data
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 4 4 4 --newpalettes --forcergb 255 0 255 --palettesize 256 --loadpalette ../tmp/TurricanPaletteData.bin --image "clouds.png" --tilewh 16 16 --fitpalettes --outputtilebytes ../tmp/TurricanClouds_tiles.bin --outputscrcol ../tmp/TurricanClouds_screen.bin --nostacking --convertwritepass

popd
