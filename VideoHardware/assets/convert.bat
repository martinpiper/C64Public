@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

echo Calculate the best palette for the clouds, no --imagequantize 8
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 4 4 4 --newpalettes --forcergb 255 0 255 --palettesize 256 --image "clouds.png" --tilewh 16 16 --outputpalettes ../tmp/PaletteDataClouds.bin --outputtilebytes ../tmp/clouds_tiles.bin --outputscrcol ../tmp/clouds_screen.bin --nostacking --convertwritepass >..\tmp\clouds.log
	
echo Convert tiles and sprites, including the cloud palette in any spaces left
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 4 4 4 --newpalettes --forcergb 0 0 0 --paletteoffset 0 --palettesize 8 --startxy 0 0 --image map.png --tilewh 16 16 --imagequantize 8 --nowritepass --resetforcergb --forcergb 255 0 255 --spritexy 0 0xd0 --startxy 0 0 --image sprites.png --tilewh 16 16 --imagequantize 32 --usestacking --nowritepass --resetforcergb --forcergb 255 0 255 --loadpalettebestfit ../tmp/PaletteDataClouds.bin --resetforcergb --forcergb 0 0 0 --startxy 0 0 --image map.png --tilewh 16 16 --imagequantize 8 --outputplanes ../tmp/background_plane --outputscrcol ../tmp/map.bin --nostacking --numbitplanes 3 --convertwritepass --nowrite --resetforcergb --forcergb 255 0 255 --spritexy 0 0xd0 --startxy 0 0 --image sprites.png --tilewh 16 16 --imagequantize 32 --outputplanes ../tmp/sprite_plane --outputsprites ../tmp/spriteSheet.txt --outputpalettes ../tmp/PaletteData.bin --usestacking --numbitplanes 3 --convertwritepass >..\tmp\map.log
		
echo Convert the clouds with new palette data
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 4 4 4 --newpalettes --forcergb 255 0 255 --palettesize 256 --loadpalette ../tmp/PaletteData.bin --image "clouds.png" --tilewh 16 16 --fitpalettes --outputtilebytes ../tmp/clouds_tiles.bin --outputscrcol ../tmp/clouds_screen.bin --nostacking --convertwritepass >..\tmp\clouds2.log


echo Convert music
cd ..\tmp
if not exist target mkdir target
java.exe -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod ..\assets\asikwp_-_twistmachine.mod


popd
