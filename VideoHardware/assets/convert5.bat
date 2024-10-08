@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

echo Calculate optimal palette
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 5 6 5 --newpalettes --resetforcergb --forcergb 255 0 255 --numbitplanes 4 --image "Road/RoadSky.png" --tilewh 16 16 --imagequantize 16 --outputpalettes ../tmp/RoadPaletteData.bin --convertwritepass



echo Convert tiles
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --palettesize 16 --rgbshift 5 6 5 --newpalettes --loadpalette ../tmp/RoadPaletteData.bin --numbitplanes 4 --image "Road/RoadSky.png" --tilewh 16 16 --fitpalettes --nostacking --outputplanes ../tmp/RoadTiles_plane --outputscrcol ../tmp/RoadTiles_map.bin --convertwritepass

popd
