@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

echo Calculate palettes and data
rem Want to preserve as much of the cloud horizon and green landscape area as possible, while remapping the clouds to the colours...
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 256 --forcergb 255 0 255 --nostacking --image "Demo14\clouds.png" --rgbfactor 160 228 248 1000 --rgbfactor 112 180 248 1000 --rgbfactor 16 80 248 1000 --rgbfactor 80 148 248 1000 --rgbfactor 32 96 248 1000 --rgbfactor 48 112 248 1000 --rgbfactor 64 132 248 1000 --imagequantize 20 --tilewh 16 16 --outputpalettes ..\tmp\Demo14ScaledSprites4Clouds.pal --convertwritepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 --forcergb 255 0 255 --forcergb 200 0 200 --forcergb 150 0 150 --forcergb 100 0 100 --nostacking --outputscaled4 ..\tmp\ScaledSprites4 --outputsprites ..\tmp\Demo14ScaledSprites4Sheet.txt --outputpalettes ..\tmp\Demo14ScaledSprites4.pal --batchimagequantize 16 --batchimages Demo14\PlayerAirplane\Level\*.png Demo14\PlayerAirplane\Left1\*.png Demo14\PlayerAirplane\Left2\*.png Demo14\PlayerAirplane\Left3\*.png Demo14\PlayerAirplane\*.png --resetforcergb --forcergb 255 0 255 --batchimages Demo14\Intro\*.png "Demo14\t-787.png" "Demo14\t-576.png" "Demo14\t-934.png" "Demo14\t-1039.png" Demo14\EnemyPlaneGreen\*.png Demo14\EnemyPlaneGreen\Roll2\*.png Demo14\EnemyPlaneHarrier\*.png Demo14\EnemyPlaneWhite\*.png --loadpalettebestfit ..\tmp\Demo14ScaledSprites4Clouds.pal --writepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 --forcergb 255 0 255 --palettesize 256 --loadpalette ..\tmp\Demo14ScaledSprites4.pal --nostacking --image "Demo14\clouds.png" --tilewh 16 16 --fitpalettes --outputtilebytes ../tmp/Demo14CloudsTiles.bin --outputscrcol ../tmp/Demo14Clouds.bin --convertwritepass

popd
