@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

echo Convert chars and sprites
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --chars --forcergb 0 0 0 --palettesize 16 --startxy 0 0 --image BatBall\level1.png --tilewh 8 8 --nowritepass --image BatBall\Blocks.png --tilewh 8 8 --nowritepass --nochars --resetforcergb --forcergb 255 0 255 --spritexy 0 0 --startxy 0 0 --image BatBall\sprites.png --tilewh 16 16 --nowritepass --resetforcergb --forcergb 0 0 0 --startxy 0 0 --chars --image BatBall\level1.png  --tilewh 8 8 --outputplanes ../tmp/BatBallchars_plane --outputscrcol ../tmp/BatBallmap.bin --numbitplanes 4 --convertwritepass --nowrite --nochars --resetforcergb --forcergb 255 0 255 --spritexy 0 0 --startxy 0 0 --image BatBall\sprites.png --tilewh 16 16 --outputplanes ../tmp/BatBallsprite_plane --outputsprites ../tmp/BatBallspriteSheet.txt --outputpalettes ../tmp/BatBallPaletteData.bin --numbitplanes 4 --convertwritepass >..\tmp\batball.log


echo Convert the blocks on their own into a new "screen"
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --chars --rgbshift 5 6 5 --newpalettes --palettesize 16 --loadpalette ../tmp/BatBallPaletteData.bin --image BatBall\Blocks.png --tilewh 8 8 --fitpalettes --outputplanes ../tmp/BatBallcharsBlocks_plane --outputscrcol ../tmp/BatBallBlocksmap.bin --numbitplanes 4 --convertwritepass >>..\tmp\batball.log


echo Convert the Ship animation to tiles
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --forcergb 255 0 255 --palettesize 16 --startxy 0 0 --image BatBall\ShipAnim.png --tilewh 16 16 --imagequantize 8 --outputplanes ../tmp/BatBallAnim_plane --outputscrcol ../tmp/BatBallAnim_map.bin --outputpalettes ../tmp/BatBallAnim_palette.bin --numbitplanes 4 --convertwritepass >>..\tmp\batball.log


popd
