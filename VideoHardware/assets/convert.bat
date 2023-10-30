@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

echo Calculate the best palette for the clouds, no --imagequantize 16
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 4 4 4 --newpalettes --forcergb 255 0 255 --palettesize 256 --image "clouds.png" --tilewh 16 16 --outputpalettes ../tmp/PaletteDataClouds.bin --outputtilebytes ../tmp/clouds_tiles.bin --outputscrcol ../tmp/clouds_screen.bin --nostacking --convertwritepass >..\tmp\clouds.log



echo Calculate optimal palette from status, tiles, sprites, including the cloud palette in any spaces left. Convert tiles and sprites.
rem No --forcergb transparent information for the tiles graphics, because the priority register moves the tiles layer is moved to the last plane with the mode7 clouds infront.
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 4 4 4 --newpalettes --palettesize 16 --chars --forcergb 255 0 255 --image "RPG status 1024.png" --tilewh 8 8 --imagequantize 16 --nowritepass --resetforcergb --nochars --startxy 0 0 --image map.png --tilewh 16 16 --imagequantize 16 --nowritepass --resetforcergb --forcergb 255 0 255 --spritexy 0 0xd0 --startxy 0 0 --image sprites.png --tilewh 16 16 --imagequantize 32 --usestacking --nowritepass --resetforcergb --forcergb 255 0 255 --loadpalettebestfit ../tmp/PaletteDataClouds.bin --nostacking --numbitplanes 4 --resetforcergb --startxy 0 0 --image map.png --tilewh 16 16 --imagequantize 16 --outputplanes ../tmp/background_plane --outputscrcol ../tmp/map.bin --convertwritepass --nowrite --resetforcergb --forcergb 255 0 255 --spritexy 0 0xd0 --startxy 0 0 --image sprites.png --tilewh 16 16 --imagequantize 32 --outputplanes ../tmp/sprite_plane --outputsprites ../tmp/spriteSheet.txt --outputpalettes ../tmp/PaletteData.bin --usestacking --numbitplanes 4 --convertwritepass >..\tmp\map.log

..\..\bin\LZMPi.exe -cr ..\tmp\sprite_plane0.bin ..\tmp\sprite_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\sprite_plane1.bin ..\tmp\sprite_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\sprite_plane2.bin ..\tmp\sprite_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\sprite_plane3.bin ..\tmp\sprite_plane3.cmp

..\..\bin\LZMPi.exe -cr ..\tmp\map.bin ..\tmp\map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\background_plane0.bin ..\tmp\background_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\background_plane1.bin ..\tmp\background_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\background_plane2.bin ..\tmp\background_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\background_plane3.bin ..\tmp\background_plane3.cmp


echo Convert the chars on their own into a new "screen"
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --splitmaps --chars --rgbshift 4 4 4 --newpalettes --palettesize 16 --loadpalette ../tmp/PaletteData.bin --image "RPG status 1024.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/status_plane --outputscrcol ../tmp/status_map.bin --nostacking --numbitplanes 4 --convertwritepass >>..\tmp\map.log
..\..\bin\LZMPi.exe -cr ..\tmp\status_map.bin ..\tmp\status_map.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\status_map.bin2 ..\tmp\status_map.cmp2
..\..\bin\LZMPi.exe -cr ..\tmp\status_plane0.bin ..\tmp\status_plane0.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\status_plane1.bin ..\tmp\status_plane1.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\status_plane2.bin ..\tmp\status_plane2.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\status_plane3.bin ..\tmp\status_plane3.cmp



echo Convert the clouds with new palette data
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 4 4 4 --newpalettes --forcergb 255 0 255 --palettesize 256 --loadpalette ../tmp/PaletteData.bin --image "clouds.png" --tilewh 16 16 --fitpalettes --outputtilebytes ../tmp/clouds_tiles.bin --outputscrcol ../tmp/clouds_screen.bin --nostacking --convertwritepass >..\tmp\clouds2.log
..\..\bin\LZMPi.exe -cr ..\tmp\clouds_screen.bin ..\tmp\clouds_screen.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\clouds_tiles.bin ..\tmp\clouds_tiles.cmp
..\..\bin\LZMPi.exe -cr ..\tmp\clouds_tiles.bin2 ..\tmp\clouds_tiles.cmp2


echo Convert music
cd ..\tmp
if not exist target mkdir target
java.exe -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod ..\assets\asikwp_-_twistmachine.mod "target/exportedMusic" 1 1

popd
