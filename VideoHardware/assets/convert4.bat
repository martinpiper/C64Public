@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

echo Calculate the best palette for the clouds, no --imagequantize 8
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 4 4 4 --newpalettes --forcergb 255 0 255 --palettesize 256 --image "clouds.png" --tilewh 16 16 --outputpalettes ../tmp/PaletteDataClouds.bin --nostacking --convertwritepass



echo Calculate optimal palette
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --newpalettes --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "RPG status 1024.png" --tilewh 8 8 --imagequantize 16 --nowritepass --numbitplanes 4 --image "Turrican/player.png" --tilewh 16 16 --imagequantize 16 --nowritepass --numbitplanes 4 --image "Turrican/1-1 trim.png" --tilewh 16 16 --imagequantize 16 --nostacking --nowritepass --loadpalettebestfit ../tmp/PaletteDataClouds.bin --outputpalettes ../tmp/TurricanPaletteData.bin --convertwritepass
rem java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --newpalettes --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "Turrican/1-1 trim 512.png" --tilewh 8 8 --imagequantize 16 --nowritepass --numbitplanes 4 --image "Turrican/player.png" --tilewh 16 16 --imagequantize 16 --nowritepass --numbitplanes 4 --image "Turrican/1-1 trim.png" --tilewh 16 16 --imagequantize 16 --nostacking --nowritepass --loadpalettebestfit ../tmp/PaletteDataClouds.bin --outputpalettes ../tmp/TurricanPaletteData.bin --convertwritepass



echo Convert sprites
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --newpalettes --loadpalette ../tmp/TurricanPaletteData.bin --numbitplanes 4 --image "Turrican/player.png" --tilewh 16 16 --fitpalettes --nostacking --outputplanes ../tmp/TurricanSprites_plane --outputsprites ../tmp/TurricanSpritesSheet.txt --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\TurricanSprites_plane0.bin ..\tmp\TurricanSprites_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\TurricanSprites_plane1.bin ..\tmp\TurricanSprites_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\TurricanSprites_plane2.bin ..\tmp\TurricanSprites_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\TurricanSprites_plane3.bin ..\tmp\TurricanSprites_plane3.cmp


echo Convert tiles
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 4 4 4 --newpalettes --loadpalette ../tmp/TurricanPaletteData.bin --numbitplanes 4 --image "Turrican/1-1 trim.png" --tilewh 16 16 --fitpalettes --nostacking --outputplanes ../tmp/TurricanTiles_plane --outputscrcol ../tmp/TurricanTiles_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\TurricanTiles_map.bin ..\tmp\TurricanTiles_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\TurricanTiles_plane0.bin ..\tmp\TurricanTiles_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\TurricanTiles_plane1.bin ..\tmp\TurricanTiles_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\TurricanTiles_plane2.bin ..\tmp\TurricanTiles_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\TurricanTiles_plane3.bin ..\tmp\TurricanTiles_plane3.cmp


echo Convert the chars
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --splitmaps --palettesize 16 --rgbshift 4 4 4 --newpalettes --loadpalette ../tmp/TurricanPaletteData.bin --chars --numbitplanes 4 --image "RPG status 1024.png" --tilewh 8 8 --fitpalettes --nostacking --outputplanes ../tmp/TurricanStatus_plane --outputscrcol ../tmp/TurricanStatus_map.bin --convertwritepass
..\..\bin\LZMPi.exe -cr ..\tmp\TurricanStatus_map.bin ..\tmp\TurricanStatus_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\TurricanStatus_map.bin2 ..\tmp\TurricanStatus_map.cmp2
..\..\bin\LZMPi.exe -cr ..\tmp\TurricanStatus_plane0.bin ..\tmp\TurricanStatus_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\TurricanStatus_plane1.bin ..\tmp\TurricanStatus_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\TurricanStatus_plane2.bin ..\tmp\TurricanStatus_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\TurricanStatus_plane3.bin ..\tmp\TurricanStatus_plane3.cmp


echo Convert the clouds with new palette data
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 4 4 4 --newpalettes --forcergb 255 0 255 --palettesize 256 --loadpalette ../tmp/TurricanPaletteData.bin --image "clouds.png" --tilewh 16 16 --fitpalettes --outputtilebytes ../tmp/TurricanClouds_tiles.bin --outputscrcol ../tmp/TurricanClouds_screen.bin --nostacking --convertwritepass



popd
