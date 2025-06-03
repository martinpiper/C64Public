@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp
if not exist ..\bin mkdir ..\bin

pushd Demo14\PerspectiveGen
python main.py
popd

echo Calculate palettes for these first
rem Want to ensure the landscape and sky colours are in a specific order, while remapping the clouds to the colours...
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 256 --forcergb 32 166 71 --forcergb 29 148 63 --forcergb 26 135 58 --forcergb 23 117 50 --forcergb 20 102 44   --forcergb 255 255 255 --forcergb 214 255 255 --forcergb 197 255 255 --forcergb 181 247 255 --forcergb 164 230 255 --forcergb 148 214 255 --forcergb 132 197 255 --forcergb 115 181 255 --forcergb 99 164 255 --forcergb 82 148 255 --forcergb 66 132 255 --forcergb 49 115 255 --forcergb 33 99 255 --forcergb 16 82 255   --nostacking --image "Demo14\clouds.png" --imagequantize 24 --tilewh 16 16 --outputpalettes ..\tmp\Demo14ScaledSprites4Clouds.pal --convertwritepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 --forcergb 255 0 255 --nostacking --image "Demo14\chars title.png" --tilewh 8 8 --nowritepass --image "Demo14/runway.png" --tilewh 16 16 --imagequantize 16 --outputpalettes ..\tmp\Demo14TitleChars.pal --convertwritepass

echo Calculate palettes and clouds
rem The --loadpalette ordering here ensures that the palettes calculated above are appropriately included

SET FORCERGB_COMMON_1=--forcergb 255 0 255 --forcergb 200 0 200 --forcergb 150 0 150 --forcergb 100 0 100
SET FORCERGB_COMMON_2=--forcergb 255 0 255

rem Player airplane
set SPRITES_COMMON_1=--batchimagequantize 16 --batchimages Demo14\PlayerAirplane\Level\*.png Demo14\PlayerAirplane\Left1\*.png Demo14\PlayerAirplane\Left2\*.png Demo14\PlayerAirplane\Left3\*.png Demo14\PlayerAirplane\*.png
rem Green enemy, smoke, and missle used in the title screen
set SPRITES_COMMON_2=--batchimages Demo14\EnemyPlaneGreen\*.png Demo14\EnemyPlaneGreen\Roll2\*.png Demo14\t-241.png Demo14\t-90.png
rem Landscape water and bushes and clouds
rem Landscape objects: 1 (also 18) , 2 , 3 , 4 , 5 , 6 , 11 , 16 , 17 , 19 , 20 , 21
rem Plus: Bullets, reticle, bullet hit ground
set SPRITES_COMMON_3=--batchimages "Demo14\t-787.png" Demo14\t-1565.png Demo14\t-1565rot.png Demo14\t-1607.png Demo14\t-1611.png Demo14\t-786.png "Demo14\t-576.png" "Demo14\t-934.png" "Demo14\t-1039.png" "Demo14\t-14.png" "Demo14\t-14rot.png" "Demo14\t-42.png" Demo14\BulletsExplosions\t-208.png Demo14\BulletsExplosions\t-793.png "Demo14\BulletsExplosions\t-231 - Copy.png" "Demo14\BulletsExplosions\t-231.png" "Demo14\BulletsExplosions\t-232 - Copy.png" "Demo14\BulletsExplosions\t-232.png" "Demo14\BulletsExplosions\t-258.png" "Demo14\BulletsExplosions\t-259.png" "Demo14\BulletsExplosions\et-317.png" "Demo14\BulletsExplosions\et-318.png" "Demo14\BulletsExplosions\et-319.png" "Demo14\BulletsExplosions\t-731.png" "Demo14\BulletsExplosions\t-300.png"
rem Enemies
set SPRITES_ENEMY_1=--batchimages Demo14\EnemyPlaneWhite\*.png
set SPRITES_ENEMY_2=--batchimages Demo14\EnemyPlaneHarrier\*.png
rem Game palette specific sprites
rem Landscape objects: 7
set SPRITES_GAME_PALETTE_0=--batchimages Demo14\t-1145.png
rem Landscape objects: 8 , 9 , 10
set SPRITES_GAME_PALETTE_1=--batchimages Demo14\t-3081.png Demo14\t-3112.png
rem Landscape objects: 12 , 13 , 14 , 15 , 22 , 23 , 24 , 25 , 26
set SPRITES_GAME_PALETTE_2=--batchimages Demo14\Canyon\t-1728.png Demo14\Canyon\t-1692.png Demo14\Canyon\t-1745.png Demo14\Canyon\t-1862.png Demo14\Canyon\t-1850.png Demo14\Canyon\t-1852.png Demo14\Canyon\t-1853.png Demo14\Canyon\t-1855.png Demo14\Canyon\t-1858.png
rem Landscape objects: Carrier
set SPRITES_GAME_PALETTE_3=--batchimages Demo14\t-715.png
rem Landscape objects: Refueling
set SPRITES_GAME_PALETTE_4=--batchimages Demo14\Refuel\rt-464.png Demo14\Refuel\rt-474.png Demo14\Refuel\rt-484.png Demo14\Refuel\rt-494.png Demo14\Refuel\rt-500.png Demo14\Refuel\rt-506.png Demo14\Refuel\rt-508left.png Demo14\Refuel\rt-508right.png Demo14\Refuel\rt-509left.png Demo14\Refuel\rt-511.png


rem For the title screen palettes we load the needed game assets first and then the specific extra sprites for the title screen itself
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 --onlyloadwholepalettes ..\tmp\Demo14ScaledSprites4Clouds.pal --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4TitleScreen.pal --resetforcergb %FORCERGB_COMMON_2% --loadpaletteraw ..\tmp\Demo14TitleChars.pal --loadpalettebestfit ..\tmp\Demo14ScaledSprites4Clouds.pal --writepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 %FORCERGB_COMMON_1% --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4TitleScreenSprites.pal %SPRITES_COMMON_1% --resetforcergb %FORCERGB_COMMON_2% %SPRITES_COMMON_2% %SPRITES_COMMON_3% %SPRITES_ENEMY_1% %SPRITES_ENEMY_2% --batchimages Demo14\Intro\*.png --writepass

rem For the game palettes we don't load the title screen sprites, this saves palette space for other game objects
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 %FORCERGB_COMMON_1% --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4Game0.pal %SPRITES_COMMON_1% --resetforcergb %FORCERGB_COMMON_2% %SPRITES_COMMON_2% %SPRITES_COMMON_3% %SPRITES_ENEMY_1% %SPRITES_ENEMY_2% %SPRITES_GAME_PALETTE_0% --writepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 %FORCERGB_COMMON_1% --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4Game1.pal %SPRITES_COMMON_1% --resetforcergb %FORCERGB_COMMON_2% %SPRITES_COMMON_2% %SPRITES_COMMON_3% %SPRITES_ENEMY_1% %SPRITES_ENEMY_2% %SPRITES_GAME_PALETTE_1% --writepass

rem Note: No SPRITES_ENEMY_1 and SPRITES_ENEMY_2
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 %FORCERGB_COMMON_1% --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4Game2.pal %SPRITES_COMMON_1% --resetforcergb %FORCERGB_COMMON_2% %SPRITES_COMMON_2% %SPRITES_COMMON_3% %SPRITES_GAME_PALETTE_2% --writepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 %FORCERGB_COMMON_1% --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4Game3.pal %SPRITES_COMMON_1% --resetforcergb %FORCERGB_COMMON_2% %SPRITES_COMMON_2% %SPRITES_COMMON_3% %SPRITES_ENEMY_1% %SPRITES_ENEMY_2% %SPRITES_GAME_PALETTE_3% --writepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 %FORCERGB_COMMON_1% --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4Game4.pal %SPRITES_COMMON_1% --resetforcergb %FORCERGB_COMMON_2% %SPRITES_COMMON_2% %SPRITES_COMMON_3% %SPRITES_ENEMY_1% %SPRITES_ENEMY_2% %SPRITES_GAME_PALETTE_4% --writepass


rem All the palettes have been calculated, now convert the sprites. Game assets first then load the title screen palettes and sprites.
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game0.pal --fitpalettes --nostacking --outputscaled4 ..\tmp\ScaledSprites4 --outputsprites ..\tmp\Demo14ScaledSprites4Sheet.txt %SPRITES_COMMON_1% %SPRITES_COMMON_2% %SPRITES_COMMON_3% %SPRITES_ENEMY_1% %SPRITES_ENEMY_2% %SPRITES_GAME_PALETTE_0% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4TitleScreenSprites.pal --batchimages Demo14\Intro\*.png --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game1.pal %SPRITES_GAME_PALETTE_1% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game2.pal %SPRITES_GAME_PALETTE_2% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game3.pal %SPRITES_GAME_PALETTE_3% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game4.pal %SPRITES_GAME_PALETTE_4% --writepass

rem Now convert the other assets
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --splitmaps --chars --rgbshift 5 6 5 --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4TitleScreen.pal --image "Demo14\chars title.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo14TitleChars_plane --outputscrcol ../tmp/Demo14TitleChars_map.bin --numbitplanes 4 --convertwritepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 256 --loadpalette ..\tmp\Demo14ScaledSprites4TitleScreen.pal --nostacking --image "Demo14\clouds.png" --tilewh 16 16 --fitpalettes --outputtilebytes ../tmp/Demo14CloudsTiles.bin --outputscrcol ../tmp/Demo14Clouds.bin --convertwritepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 --forcergb 255 0 255 --loadpaletteraw ..\tmp\Demo14ScaledSprites4TitleScreen.pal --image "Demo14\runway.png" --tilewh 16 16 --fitpalettes --outputplanes ../tmp/Demo14Runway_plane --outputscrcol ../tmp/Demo14Runway_map.bin --numbitplanes 4 --convertwritepass

echo Convert music
cd ..\tmp
if not exist target mkdir target
rem java.exe -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod ..\assets\AfterBurnerNoYM.xm "target/exportedMusicAfterBurner" 65535 198141
rem java.exe -Dmusic.channel.remap.4=-1 -Dmusic.channel.remap.5=-1 -Dmusic.channel.remap.6=-1 -Dmusic.channel.remap.7=-1 -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod ..\assets\AfterBurnerNoYM.xm "target/exportedMusicAfterBurner1" 65535 190205
rem java.exe -Dmusic.channel.remap.0=-1 -Dmusic.channel.remap.1=-1 -Dmusic.channel.remap.2=-1 -Dmusic.channel.remap.3=-1 -Dmusic.channel.remap.4=0 -Dmusic.channel.remap.5=1 -Dmusic.channel.remap.6=2 -Dmusic.channel.remap.7=3 -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod ..\assets\AfterBurnerNoYM.xm "target/exportedMusicAfterBurner2" 1 1
rem java.exe -Dmusic.channel.remap.4=0 -Dmusic.channel.remap.5=1 -Dmusic.channel.remap.6=2 -Dmusic.channel.remap.7=3 -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod ..\assets\AfterBurnerNoYM.xm "target/exportedMusicAfterBurner" 65535 198141
rem java.exe -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --playmod "target/exportedMusicAfterBurner"
rem start java.exe -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --playmod "target/exportedMusicAfterBurner1" && start java.exe -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --playmod "target/exportedMusicAfterBurner2"

echo Convert sound effects
rem java.exe -Dmusic.channel.remap.4=0 -Dmusic.channel.remap.5=1 -Dmusic.channel.remap.6=2 -Dmusic.channel.remap.7=3 -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod "..\assets\AfterBurner sound effects reduced.xm" "target/exportedSoundEffectsAfterBurner" 65535 168864
java.exe -Dmusic.channel.remap.4=0 -Dmusic.channel.remap.5=1 -Dmusic.channel.remap.6=2 -Dmusic.channel.remap.7=3 -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod "..\assets\AfterBurner sound effects reduced2.xm" "target/exportedSoundEffectsAfterBurner" 65535 74753

cd ..

del /q tmp\Demo14FileResources.bin

python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0xa200 bytes 0x60 0x00 0x20 0x00

rem Exact address first
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x00
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x01
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin1
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x02
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin2
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x03
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin3
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x04
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin4
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x05
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin5
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x06
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin6

rem Then non-exact audio hardware using the EBBS
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x04 0x0000 file tmp/target/exportedSoundEffectsAfterBurnerSamples.bin

python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x00
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4TitleScreen.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x01
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4TitleScreen.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x02
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4TitleScreen.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x03
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4TitleScreen.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x04
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4TitleScreen.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x0f
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4TitleScreen.pal

python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x10
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4Game0.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x11
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4Game1.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x12
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4Game2.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x13
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4Game3.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x14
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4Game4.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x1f
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4TitleScreenSprites.pal

python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x80 0x4000 file tmp/Demo14TitleChars_map.bin
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x80 0x8000 file tmp/Demo14TitleChars_map.bin2
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x20 0x2000 file tmp/Demo14TitleChars_plane0.bin
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x20 0x4000 file tmp/Demo14TitleChars_plane1.bin
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x20 0x8000 file tmp/Demo14TitleChars_plane2.bin
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x20 0x0000 file tmp/Demo14TitleChars_plane3.bin

python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x80 0x2000 file tmp/Demo14Runway_map.bin
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x40 0x2000 file tmp/Demo14Runway_plane0.bin
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x40 0x4000 file tmp/Demo14Runway_plane1.bin
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x40 0x8000 file tmp/Demo14Runway_plane2.bin
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x40 0x0000 file tmp/Demo14Runway_plane3.bin

python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x08 0x2000 file tmp/Demo14Clouds.bin
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x08 0x4000 file tmp/Demo14CloudsTiles.bin
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x08 0x8000 file tmp/Demo14CloudsTiles.bin2
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0xa000 bytes 0 0x01 0 0xb5 0 0 0 0x01 0 0x4b 0xff 0xff 0 0x21 0 0 0x61 0 0 0 0x00 0x1f

rem End marker
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x00

popd

dir ..\tmp\ScaledSprites4.bin*
dir ..\tmp\Demo14ScaledSprites4*.pal
