@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp
if not exist ..\bin mkdir ..\bin

pushd Demo14\PerspectiveGen
python main.py
popd

echo Create combined chars screens data
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concath "Demo14\chars title.png" "demo14\chars game.png" ..\tmp\demo14chars1.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concath "Demo14\chars charset.png" "demo14\chars hiscore.png" ..\tmp\demo14chars2.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "..\tmp\demo14chars2.png" "..\tmp\demo14chars1.png" ..\tmp\demo14chars2.png

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concath "Demo14\MissionComplete\Screen1.png" "Demo14\MissionComplete\Screen2.png" ..\tmp\demo14chars3.png
rem Avoids a black screen, needs transparency in the corner
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concath "Demo14\MissionComplete\Screen3.png" "Demo14\MissionComplete\Screen3.png" ..\tmp\demo14chars4.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "..\tmp\demo14chars3.png" "..\tmp\demo14chars4.png" ..\tmp\demo14chars3.png

rem Intro screen processing
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concath "Demo14\IntroScreens\Picture1B.png" "Demo14\IntroScreens\Picture3B.png" ..\tmp\demo14Picture1_3B.png
rem Makes sure all the screens are filled vertically, avoiding residual data
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "..\tmp\demo14Picture1_3B.png" "..\tmp\demo14Picture1_3B.png" ..\tmp\demo14Picture1_3B.png
rem Tiles cannot have so many tiles
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concath "Demo14\IntroScreens\Picture2B.png" "Demo14\IntroScreens\Picture2B.png" ..\tmp\demo14Picture2B.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "..\tmp\demo14Picture2B.png" "..\tmp\demo14Picture2B.png" ..\tmp\demo14Picture2B.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "..\tmp\demo14Picture2B.png" "..\tmp\demo14Picture2B.png" ..\tmp\demo14Picture2B.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concath "Demo14\IntroScreens\Picture4B.png" "Demo14\IntroScreens\Picture4B.png" ..\tmp\demo14Picture4B.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "..\tmp\demo14Picture4B.png" "..\tmp\demo14Picture4B.png" ..\tmp\demo14Picture4B.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "..\tmp\demo14Picture4B.png" "..\tmp\demo14Picture4B.png" ..\tmp\demo14Picture4B.png

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concath "Demo14\IntroScreens\Screen1A.png" "Demo14\IntroScreens\Screen1A.png" ..\tmp\demo14Screen1A.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "..\tmp\demo14Screen1A.png" "..\tmp\demo14Screen1A.png" ..\tmp\demo14Screen1A.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "..\tmp\demo14Screen1A.png" "..\tmp\demo14Screen1A.png" ..\tmp\demo14Screen1A.png

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concath "Demo14\IntroScreens\Screen1B.png" "Demo14\IntroScreens\Screen1B.png" ..\tmp\demo14Screen1B.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "..\tmp\demo14Screen1B.png" "..\tmp\demo14Screen1B.png" ..\tmp\demo14Screen1B.png

rem Two are vertically stacked, since they are only scrolling horizontally
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concath "Demo14\IntroScreens\Screen2A.png" "Demo14\IntroScreens\Screen2A.png" ..\tmp\demo14Screen2A.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concath "Demo14\IntroScreens\Screen3A.png" "Demo14\IntroScreens\Screen3A.png" ..\tmp\demo14Screen3A.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "..\tmp\demo14Screen2A.png" "..\tmp\demo14Screen3A.png" ..\tmp\demo14Screen2_3A.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "..\tmp\demo14Screen2A.png" "..\tmp\demo14Screen3A.png" ..\tmp\demo14Screen2_3A.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "..\tmp\demo14Screen2_3A.png" "..\tmp\demo14Screen2_3A.png" ..\tmp\demo14Screen2_3A.png

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concath "Demo14\IntroScreens\Screen2B.png" "Demo14\IntroScreens\Screen2B.png" ..\tmp\demo14Screen2B.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concath "Demo14\IntroScreens\Screen3B.png" "Demo14\IntroScreens\Screen3B.png" ..\tmp\demo14Screen3B.png
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --concat "..\tmp\demo14Screen2B.png" "..\tmp\demo14Screen3B.png" ..\tmp\demo14Screen2_3B.png


echo Calculate palettes for these first
rem Want to ensure the landscape and sky colours are in a specific order, while remapping the clouds to the colours...
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 256 --forcergb 32 166 71 --forcergb 29 148 63 --forcergb 26 135 58 --forcergb 23 117 50 --forcergb 20 102 44   --forcergb 255 255 255 --forcergb 214 255 255 --forcergb 197 255 255 --forcergb 181 247 255 --forcergb 164 230 255 --forcergb 148 214 255 --forcergb 132 197 255 --forcergb 115 181 255 --forcergb 99 164 255 --forcergb 82 148 255 --forcergb 66 132 255 --forcergb 49 115 255 --forcergb 33 99 255 --forcergb 16 82 255   --nostacking --image "Demo14\clouds.png" --imagequantize 24 --tilewh 16 16 --outputpalettes ..\tmp\Demo14ScaledSprites4Clouds.pal --convertwritepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 --forcergb 255 0 255 --paletteaddrgbs 0 0 0   0 181 247 --paletteaddrgbs 0 0 0   247 181 247 --paletteaddrgbs 0 0 0   247 0 0  --paletteaddrgbs 0 0 0   247 247 148  --paletteaddrgbs 0 0 0   0 247 197  --nostacking --image "..\tmp\demo14chars2.png" --tilewh 8 8 --nowritepass --batchimages Demo14\MissionComplete\*.png --nowritepass --image "Demo14/runway.png" --tilewh 16 16 --imagequantize 16 --outputpalettes ..\tmp\Demo14TitleChars.pal --convertwritepass

rem Use both tile sizes to catch any potential palettes that don't fit 8x8 to 16x16
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 --forcergb 255 0 255 --nostacking --tilewh 8 8 --batchimages Demo14\IntroScreens\*.png --nowritepass --tilewh 16 16 --batchimages Demo14\IntroScreens\*.png --nowritepass --outputpalettes ..\tmp\Demo14TitleIntroScreens.pal --convertwritepass

echo Calculate palettes and clouds
rem The --loadpalette ordering here ensures that the palettes calculated above are appropriately included

SET FORCERGB_COMMON_1=--forcergb 255 0 255 --forcergb 200 0 200 --forcergb 150 0 150 --forcergb 100 0 100
SET FORCERGB_COMMON_2=--forcergb 255 0 255

rem Player airplane
set SPRITES_COMMON_1=--batchimagequantize 16 --batchimages Demo14\PlayerAirplane\Level\*.png Demo14\PlayerAirplane\Left1\*.png Demo14\PlayerAirplane\Left2\*.png Demo14\PlayerAirplane\Left3\*.png Demo14\PlayerAirplane\*.png
rem Green enemy, smoke, and missle used in the title screen
set SPRITES_COMMON_2=--batchimagequantize 16 --batchimages Demo14\EnemyPlaneGreen\*.png Demo14\EnemyPlaneGreen\Roll2\*.png Demo14\t-241.png Demo14\t-104.png Demo14\t-113.png Demo14\t-279.png Demo14\t-90.png Demo14\t-177.png
rem Bullets, reticle, bullet hit ground
set SPRITES_COMMON_3=--batchimages Demo14\BulletsExplosions\t-208.png Demo14\BulletsExplosions\t-793.png Demo14\BulletsExplosions\t-328.png "Demo14\BulletsExplosions\et-731.png" "Demo14\BulletsExplosions\et-300.png" "Demo14\BulletsExplosions\et-509.png" "Demo14\BulletsExplosions\et-514.png" "Demo14\BulletsExplosions\et-523.png" "Demo14\BulletsExplosions\et-501.png" "Demo14\BulletsExplosions\et-528.png" "Demo14\BulletsExplosions\et-301.png" "Demo14\BulletsExplosions\et-517.png" "Demo14\BulletsExplosions\et-365.png" "Demo14\BulletsExplosions\et-383.png" "Demo14\BulletsExplosions\et-408.png" Demo14\GameOver\*.png Demo14\speed.png Demo14\Radar\*.png Demo14\t-8.png Demo14\missioncomplete.png Demo14\Hand\*.png

rem Landscape water and bushes and clouds
rem Landscape objects: 1 (rock) (also 18 smaller version) , 2 (green bush variation) , 3 (light green bush) , 4 (green yellow bush), 5 (green bush), 6 (sea) , 11 (cloud) , then explosions 16 , 17 , 19 , 20 , 21
rem Landscape explosions
set SPRITES_GAME_LANDSCAPE_COMMON=--batchimages Demo14\t-787.png Demo14\t-1565.png Demo14\t-1565rot.png Demo14\t-1607.png Demo14\t-1611.png Demo14\t-786.png Demo14\t-576.png Demo14\t-934.png Demo14\t-1039.png Demo14\t-14.png Demo14\t-14rot.png Demo14\t-42.png "Demo14\BulletsExplosions\t-231 - Copy.png" "Demo14\BulletsExplosions\t-231.png" "Demo14\BulletsExplosions\t-232 - Copy.png" "Demo14\BulletsExplosions\t-232.png" "Demo14\BulletsExplosions\t-258.png" "Demo14\BulletsExplosions\t-259.png" "Demo14\BulletsExplosions\et-317.png" "Demo14\BulletsExplosions\et-318.png" "Demo14\BulletsExplosions\et-319.png"
rem Enemies
set SPRITES_ENEMY_1=--batchimages Demo14\EnemyPlaneWhite\*.png
set SPRITES_ENEMY_2=--batchimages Demo14\EnemyPlaneHarrier\*.png Demo14\EnemyBomber\*.png
rem Game palette specific sprites
rem Landscape objects: 7
set SPRITES_GAME_LANDSCAPE_PALETTE_0=--batchimages Demo14\t-1145.png Demo14\t-1145rot.png Demo14\t-1122.png Demo14\t-1122rot.png
rem Landscape objects: 8 , 9 , 10
set SPRITES_GAME_LANDSCAPE_PALETTE_1=--batchimages Demo14\t-3081.png Demo14\t-3112.png
rem Landscape objects: 12 , 13 , 14 , 15 , 22 , 23 , 24 , 25 , 26
set SPRITES_GAME_LANDSCAPE_PALETTE_2=--batchimages Demo14\Canyon\t-1728.png Demo14\Canyon\t-1692.png Demo14\Canyon\t-1745.png Demo14\Canyon\t-1862.png Demo14\Canyon\t-1850.png Demo14\Canyon\t-1852.png Demo14\Canyon\t-1853.png Demo14\Canyon\t-1855.png Demo14\Canyon\t-1858.png
rem Landscape objects: Carrier
set SPRITES_GAME_LANDSCAPE_PALETTE_3=--batchimages Demo14\t-715.png
rem Landscape objects: Refueling
set SPRITES_GAME_LANDSCAPE_PALETTE_4=--batchimages Demo14\Refuel\rt-464.png Demo14\Refuel\rt-474.png Demo14\Refuel\rt-484.png Demo14\Refuel\rt-494.png Demo14\Refuel\rt-500.png Demo14\Refuel\rt-506.png Demo14\Refuel\rt-508left.png Demo14\Refuel\rt-508right.png Demo14\Refuel\rt-509left.png Demo14\Refuel\rt-511.png
rem Landscape objects: Columns 27 (single yellow) 28 (single grey)  (four grey, not assigned yet...)
set SPRITES_GAME_LANDSCAPE_PALETTE_5=--batchimages Demo14\Columns\*.png


rem For the title screen palettes we load the needed game assets first and then the specific extra sprites for the title screen itself
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 --onlyloadwholepalettes ..\tmp\Demo14ScaledSprites4Clouds.pal --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4TitleScreen.pal --resetforcergb %FORCERGB_COMMON_2% --loadpaletteraw ..\tmp\Demo14TitleChars.pal --loadpaletteraw ..\tmp\Demo14TitleIntroScreens.pal --loadpalettebestfit ..\tmp\Demo14ScaledSprites4Clouds.pal --writepass

rem This is the same ordering as above, but only for converting the main title screen chars, score panel etc
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 --onlyloadwholepalettes ..\tmp\Demo14ScaledSprites4Clouds.pal --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4TitleScreenForTitles.pal --resetforcergb %FORCERGB_COMMON_2% --loadpaletteraw ..\tmp\Demo14TitleChars.pal --loadpalettebestfit ..\tmp\Demo14ScaledSprites4Clouds.pal --writepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 %FORCERGB_COMMON_1% --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4TitleScreenSprites.pal %SPRITES_COMMON_1% --resetforcergb %FORCERGB_COMMON_2% %SPRITES_COMMON_2% --batchimages Demo14\Intro\*.png --writepass


rem Game entity sprite palette space
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 %FORCERGB_COMMON_1% --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4GameEntity0.pal %SPRITES_COMMON_1% --resetforcergb %FORCERGB_COMMON_2% %SPRITES_COMMON_2% %SPRITES_COMMON_3% %SPRITES_ENEMY_1% %SPRITES_ENEMY_2% --writepass


rem For the game landscape palettes we don't load the title screen sprites, this saves palette space for other game objects
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 %FORCERGB_COMMON_2% --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4Game0.pal %SPRITES_GAME_LANDSCAPE_COMMON% %SPRITES_GAME_LANDSCAPE_PALETTE_0% --writepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 %FORCERGB_COMMON_2% --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4Game1.pal %SPRITES_GAME_LANDSCAPE_COMMON% %SPRITES_GAME_LANDSCAPE_PALETTE_1% --writepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 %FORCERGB_COMMON_2% --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4Game2.pal %SPRITES_GAME_LANDSCAPE_COMMON% %SPRITES_GAME_LANDSCAPE_PALETTE_2% --writepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 %FORCERGB_COMMON_2% --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4Game3.pal %SPRITES_GAME_LANDSCAPE_COMMON% %SPRITES_GAME_LANDSCAPE_PALETTE_3% --writepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 %FORCERGB_COMMON_2% --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4Game4.pal %SPRITES_GAME_LANDSCAPE_COMMON% %SPRITES_GAME_LANDSCAPE_PALETTE_4% --writepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 %FORCERGB_COMMON_2% --nostacking --outputpalettes ..\tmp\Demo14ScaledSprites4Game5.pal %SPRITES_GAME_LANDSCAPE_COMMON% %SPRITES_GAME_LANDSCAPE_PALETTE_5% --writepass


rem All the palettes have been calculated, now convert the sprites. Game assets first then load the title screen palettes and sprites.
rem --allowedSizesClear --allowedSizesFileAdd "Demo14\more allowed sizes.txt" --allowedSizesFileAdd ..\tmp\Demo14ScaledSprites4SheetOptimised.txtTables.allowed.txt
rem But without any allowed sizes, so all sizes are used first
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4GameEntity0.pal --fitpalettes --nostacking --outputscaled4 ..\tmp\ScaledSprites4 --outputsprites ..\tmp\Demo14ScaledSprites4Sheet.txt %SPRITES_COMMON_1% %SPRITES_COMMON_2% %SPRITES_COMMON_3% %SPRITES_ENEMY_1% %SPRITES_ENEMY_2% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game0.pal %SPRITES_GAME_LANDSCAPE_COMMON% %SPRITES_GAME_LANDSCAPE_PALETTE_0% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4TitleScreenSprites.pal --batchimages Demo14\Intro\*.png --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game1.pal %SPRITES_GAME_LANDSCAPE_PALETTE_1% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game2.pal %SPRITES_GAME_LANDSCAPE_PALETTE_2% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game3.pal %SPRITES_GAME_LANDSCAPE_PALETTE_3% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game4.pal %SPRITES_GAME_LANDSCAPE_PALETTE_4% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game5.pal %SPRITES_GAME_LANDSCAPE_PALETTE_5% --writepass


rem Then optimise the spite size lists
pushd ..
rem Title first
del tmp\Demo14ScaledSprites4SheetOptimisedTitle.txtTables.a
python ..\VideoHardware\OptimiseSprite4ScaleTables\main.py tmp\Demo14ScaledSprites4Sheet.txtTables.a tmp\Demo14ScaledSprites4SheetOptimisedTitle.txtTables.a tmp\Demo14ScaledSprites4SheetOptimised.txtTables.allowed.txt asm\AfterBurner\*.a

rem Leave the optimised game sprite size list last
del tmp\Demo14ScaledSprites4SheetOptimisedGame.txtTables.a
python ..\VideoHardware\OptimiseSprite4ScaleTables\main.py tmp\Demo14ScaledSprites4Sheet.txtTables.a tmp\Demo14ScaledSprites4SheetOptimisedGame.txtTables.a tmp\Demo14ScaledSprites4SheetOptimised.txtTables.allowed.txt asm\AfterBurner\Game*.a asm\AfterBurner\Entity*.a asm\AfterBurner\Landscape*.a
popd

rem And repeat the conversion with the allowed sizes updates, this is a duplicate of the above
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --allowedSizesClear --allowedSizesFileAdd "Demo14\more allowed sizes.txt" --allowedSizesFileAdd ..\tmp\Demo14ScaledSprites4SheetOptimised.txtTables.allowed.txt --rgbshift 5 6 5 --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4GameEntity0.pal --fitpalettes --nostacking --outputscaled4 ..\tmp\ScaledSprites4 --outputsprites ..\tmp\Demo14ScaledSprites4Sheet.txt %SPRITES_COMMON_1% %SPRITES_COMMON_2% %SPRITES_COMMON_3% %SPRITES_ENEMY_1% %SPRITES_ENEMY_2% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game0.pal %SPRITES_GAME_LANDSCAPE_COMMON% %SPRITES_GAME_LANDSCAPE_PALETTE_0% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4TitleScreenSprites.pal --batchimages Demo14\Intro\*.png --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game1.pal %SPRITES_GAME_LANDSCAPE_PALETTE_1% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game2.pal %SPRITES_GAME_LANDSCAPE_PALETTE_2% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game3.pal %SPRITES_GAME_LANDSCAPE_PALETTE_3% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game4.pal %SPRITES_GAME_LANDSCAPE_PALETTE_4% --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4Game5.pal %SPRITES_GAME_LANDSCAPE_PALETTE_5% --writepass


rem Then produce the final optimised list again from the last conversion
pushd ..
rem Title first
del tmp\Demo14ScaledSprites4SheetOptimisedTitle.txtTables.a
python ..\VideoHardware\OptimiseSprite4ScaleTables\main.py tmp\Demo14ScaledSprites4Sheet.txtTables.a tmp\Demo14ScaledSprites4SheetOptimisedTitle.txtTables.a tmp\Demo14ScaledSprites4SheetOptimised.txtTables.allowed.txt asm\AfterBurner\*.a

rem Leave the optimised game sprite size list last
del tmp\Demo14ScaledSprites4SheetOptimisedGame.txtTables.a
python ..\VideoHardware\OptimiseSprite4ScaleTables\main.py tmp\Demo14ScaledSprites4Sheet.txtTables.a tmp\Demo14ScaledSprites4SheetOptimisedGame.txtTables.a tmp\Demo14ScaledSprites4SheetOptimised.txtTables.allowed.txt asm\AfterBurner\Game*.a asm\AfterBurner\Entity*.a asm\AfterBurner\Landscape*.a
popd


rem Now convert the other assets
rem Normal screens, note the special case smaller palette to avoid picking up wrong colours from extra graphics
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --preferLaterPalettes --splitmaps --chars --rgbshift 5 6 5 --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4TitleScreenForTitles.pal --image "..\tmp\demo14chars2.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo14TitleChars_plane --outputscrcol ../tmp/Demo14TitleChars_map.bin --numbitplanes 4 --convertwritepass

..\..\acme.exe -v9 ..\asm\AfterBurner\ConvtabChar8x8.a
..\..\acme.exe -v9 ..\asm\AfterBurner\ConvtabChar8x16Top.a
..\..\acme.exe -v9 ..\asm\AfterBurner\ConvtabChar8x16Bot.a
..\..\ExternalTools\Gnu\bin\sed.exe -n -e "/^kTileScreenDebug____tmp_Demo14TitleChars_map_bin_x8_.*_colour/p" ..\tmp\Demo14TitleChars_map.bin.debug.txt >..\tmp\Demo14TitleChars_map.bin.debug.optimised.txt
..\..\ExternalTools\Gnu\bin\sed.exe -n -e "/^kTileScreenDebug____tmp_Demo14TitleChars_map_bin_x544_y480.*/p" ..\tmp\Demo14TitleChars_map.bin.debug.txt >>..\tmp\Demo14TitleChars_map.bin.debug.optimised.txt
..\..\ExternalTools\Gnu\bin\sed.exe -n -e "/^kTileScreenDebug____tmp_Demo14TitleChars_map_bin_x824_y48_.*/p" ..\tmp\Demo14TitleChars_map.bin.debug.txt >>..\tmp\Demo14TitleChars_map.bin.debug.optimised.txt

rem Score table data
..\..\acme.exe -v4 --lib ../ --lib ../../ --lib ../../../ --lib ../asm/ --msvc ../asm/AfterBurner/ScoreTableData.a


rem Mission complete chars
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --preferLaterPalettes --splitmaps --chars --rgbshift 5 6 5 --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4TitleScreen.pal --image "..\tmp\demo14chars3.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo14TitleChars2_plane --outputscrcol ../tmp/Demo14TitleChars2_map.bin --numbitplanes 4 --convertwritepass

rem Intro screens
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --preferLaterPalettes --splitmaps --chars --rgbshift 5 6 5 --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4TitleScreen.pal --image "..\tmp\demo14Picture1_3B.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo14IntroScreensPicture1_3B_plane --outputscrcol ../tmp/Demo14IntroScreensPicture1_3B_map.bin --numbitplanes 4 --convertwritepass
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 --forcergb 255 0 255 --loadpaletteraw ..\tmp\Demo14ScaledSprites4TitleScreen.pal --image "..\tmp\demo14Picture2B.png" --tilewh 16 16 --fitpalettes --outputplanes ../tmp/Demo14IntroScreensPicture2B_plane --outputscrcol ../tmp/Demo14IntroScreensPicture2B_map.bin --numbitplanes 4 --convertwritepass
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 --forcergb 255 0 255 --loadpaletteraw ..\tmp\Demo14ScaledSprites4TitleScreen.pal --image "..\tmp\demo14Picture4B.png" --tilewh 16 16 --fitpalettes --outputplanes ../tmp/Demo14IntroScreensPicture4B_plane --outputscrcol ../tmp/Demo14IntroScreensPicture4B_map.bin --numbitplanes 4 --convertwritepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 --forcergb 255 0 255 --loadpaletteraw ..\tmp\Demo14ScaledSprites4TitleScreen.pal --image "..\tmp\demo14Screen1A.png" --tilewh 16 16 --fitpalettes --outputplanes ../tmp/Demo14IntroScreensScreen1A_plane --outputscrcol ../tmp/Demo14IntroScreensScreen1A_map.bin --numbitplanes 4 --convertwritepass
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --preferLaterPalettes --splitmaps --chars --rgbshift 5 6 5 --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4TitleScreen.pal --image "..\tmp\demo14Screen1B.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo14IntroScreensScreen1B_plane --outputscrcol ../tmp/Demo14IntroScreensScreen1B_map.bin --numbitplanes 4 --convertwritepass

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 16 --forcergb 255 0 255 --loadpaletteraw ..\tmp\Demo14ScaledSprites4TitleScreen.pal --image "..\tmp\demo14Screen2_3A.png" --tilewh 16 16 --fitpalettes --outputplanes ../tmp/Demo14IntroScreensScreen2_3A_plane --outputscrcol ../tmp/Demo14IntroScreensScreen2_3A_map.bin --numbitplanes 4 --convertwritepass
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --preferLaterPalettes --splitmaps --chars --rgbshift 5 6 5 --newpalettes --palettesize 16 --loadpaletteraw ..\tmp\Demo14ScaledSprites4TitleScreen.pal --image "..\tmp\demo14Screen2_3B.png" --tilewh 8 8 --fitpalettes --outputplanes ../tmp/Demo14IntroScreensScreen2_3B_plane --outputscrcol ../tmp/Demo14IntroScreensScreen2_3B_map.bin --numbitplanes 4 --convertwritepass

rem Mode7 clouds and landscape
java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --rgbshift 5 6 5 --newpalettes --palettesize 256 --loadpalette ..\tmp\Demo14ScaledSprites4TitleScreen.pal --nostacking --image "Demo14\clouds.png" --tilewh 16 16 --fitpalettes --outputtilebytes ../tmp/Demo14CloudsTiles.bin --outputscrcol ../tmp/Demo14Clouds.bin --convertwritepass

rem Runway/road
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
rem java.exe -Dmusic.channel.remap.4=0 -Dmusic.channel.remap.5=1 -Dmusic.channel.remap.6=2 -Dmusic.channel.remap.7=3 -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod "..\assets\AfterBurner sound effects reduced2.xm" "target/exportedSoundEffectsAfterBurner" 65535 79361
java.exe -Dmusic.channel.remap.4=0 -Dmusic.channel.remap.5=1 -Dmusic.channel.remap.6=2 -Dmusic.channel.remap.7=3 -Dmusic.volume=1 -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --exportmod "..\assets\AfterBurner sound effects reduced3.xm" "target/exportedSoundEffectsAfterBurner" 65535 80481

cd ..

del /q tmp\Demo14FileResources.bin

python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0xa200 bytes 0x60 0x00 0x20 0x00

rem Exact address first
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x00
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0xb807 bytes 0x00
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x01
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0xb807 bytes 0x01
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin1
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x02
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0xb807 bytes 0x02
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin2
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x03
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0xb807 bytes 0x03
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin3
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x04
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0xb807 bytes 0x04
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin4
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x05
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0xb807 bytes 0x05
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin5
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x06
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0xb807 bytes 0x06
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin6
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x07
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0xb807 bytes 0x07
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin7
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x08
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0xb807 bytes 0x08
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin8
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x8807 bytes 0x09
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0xb807 bytes 0x09
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x05 0x0000 file tmp\ScaledSprites4.bin9

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
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x05
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4TitleScreen.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x06
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4TitleScreen.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x07
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4TitleScreen.pal

python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x10
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4GameEntity0.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x11
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4GameEntity0.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x12
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4GameEntity0.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x13
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4GameEntity0.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x14
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4GameEntity0.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x15
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4GameEntity0.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x16
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4GameEntity0.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x17
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4TitleScreenSprites.pal

python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x08
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4Game0.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x09
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4Game1.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x0a
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4Game2.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x0b
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4Game3.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x0c
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4Game4.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x0d
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4Game5.pal
python ResourceGenerator\main.py tmp/Demo14FileResources.bin 0x01 0x9e0c bytes 0x0f
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

rem Mission complete resources
del /q tmp\Demo14FileResourcesMissionComplete.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesMissionComplete.bin 0x80 0x4000 file tmp/Demo14TitleChars2_map.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesMissionComplete.bin 0x80 0x8000 file tmp/Demo14TitleChars2_map.bin2
python ResourceGenerator\main.py tmp/Demo14FileResourcesMissionComplete.bin 0x20 0x2000 file tmp/Demo14TitleChars2_plane0.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesMissionComplete.bin 0x20 0x4000 file tmp/Demo14TitleChars2_plane1.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesMissionComplete.bin 0x20 0x8000 file tmp/Demo14TitleChars2_plane2.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesMissionComplete.bin 0x20 0x0000 file tmp/Demo14TitleChars2_plane3.bin
rem End marker
python ResourceGenerator\main.py tmp/Demo14FileResourcesMissionComplete.bin 0x00
rem Mission complete restore resources
del /q tmp\Demo14FileResourcesMissionCompleteRestore.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesMissionCompleteRestore.bin 0x80 0x4000 file tmp/Demo14TitleChars_map.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesMissionCompleteRestore.bin 0x80 0x8000 file tmp/Demo14TitleChars_map.bin2
python ResourceGenerator\main.py tmp/Demo14FileResourcesMissionCompleteRestore.bin 0x20 0x2000 file tmp/Demo14TitleChars_plane0.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesMissionCompleteRestore.bin 0x20 0x4000 file tmp/Demo14TitleChars_plane1.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesMissionCompleteRestore.bin 0x20 0x8000 file tmp/Demo14TitleChars_plane2.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesMissionCompleteRestore.bin 0x20 0x0000 file tmp/Demo14TitleChars_plane3.bin
rem End marker
python ResourceGenerator\main.py tmp/Demo14FileResourcesMissionCompleteRestore.bin 0x00

rem Runway restore resources, with palette 0
del /q tmp\Demo14FileResourcesRunwayRestore.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesRunwayRestore.bin 0x01 0x9e0c bytes 0x00
python ResourceGenerator\main.py tmp/Demo14FileResourcesRunwayRestore.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4TitleScreen.pal
python ResourceGenerator\main.py tmp/Demo14FileResourcesRunwayRestore.bin 0x80 0x2000 file tmp/Demo14Runway_map.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesRunwayRestore.bin 0x40 0x2000 file tmp/Demo14Runway_plane0.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesRunwayRestore.bin 0x40 0x4000 file tmp/Demo14Runway_plane1.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesRunwayRestore.bin 0x40 0x8000 file tmp/Demo14Runway_plane2.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesRunwayRestore.bin 0x40 0x0000 file tmp/Demo14Runway_plane3.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesRunwayRestore.bin 0x00
rem Intro screens
del /q tmp\Demo14FileResourcesPicture1_3B.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture1_3B.bin 0x01 0x9e0c bytes 0x00
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture1_3B.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4TitleScreen.pal
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture1_3B.bin 0x90 0x4000 file tmp/Demo14IntroScreensPicture1_3B_map.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture1_3B.bin 0x90 0x8000 file tmp/Demo14IntroScreensPicture1_3B_map.bin2
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture1_3B.bin 0x30 0x2000 file tmp/Demo14IntroScreensPicture1_3B_plane0.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture1_3B.bin 0x30 0x4000 file tmp/Demo14IntroScreensPicture1_3B_plane1.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture1_3B.bin 0x30 0x8000 file tmp/Demo14IntroScreensPicture1_3B_plane2.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture1_3B.bin 0x30 0x0000 file tmp/Demo14IntroScreensPicture1_3B_plane3.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture1_3B.bin 0x00
del /q tmp\Demo14FileResourcesPicture2B.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture2B.bin 0x80 0x2000 file tmp/Demo14IntroScreensPicture2B_map.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture2B.bin 0x40 0x2000 file tmp/Demo14IntroScreensPicture2B_plane0.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture2B.bin 0x40 0x4000 file tmp/Demo14IntroScreensPicture2B_plane1.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture2B.bin 0x40 0x8000 file tmp/Demo14IntroScreensPicture2B_plane2.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture2B.bin 0x40 0x0000 file tmp/Demo14IntroScreensPicture2B_plane3.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture2B.bin 0x00
del /q tmp\Demo14FileResourcesPicture4B.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture4B.bin 0x80 0x2000 file tmp/Demo14IntroScreensPicture4B_map.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture4B.bin 0x40 0x2000 file tmp/Demo14IntroScreensPicture4B_plane0.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture4B.bin 0x40 0x4000 file tmp/Demo14IntroScreensPicture4B_plane1.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture4B.bin 0x40 0x8000 file tmp/Demo14IntroScreensPicture4B_plane2.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture4B.bin 0x40 0x0000 file tmp/Demo14IntroScreensPicture4B_plane3.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesPicture4B.bin 0x00
del /q tmp\Demo14FileResourcesScreen1.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen1.bin 0x01 0x9e0c bytes 0x18
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen1.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4TitleScreen.pal
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen1.bin 0x01 0x9e0c bytes 0x00
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen1.bin 0x90 0x4000 file tmp/Demo14IntroScreensScreen1B_map.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen1.bin 0x90 0x8000 file tmp/Demo14IntroScreensScreen1B_map.bin2
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen1.bin 0x30 0x2000 file tmp/Demo14IntroScreensScreen1B_plane0.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen1.bin 0x30 0x4000 file tmp/Demo14IntroScreensScreen1B_plane1.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen1.bin 0x30 0x8000 file tmp/Demo14IntroScreensScreen1B_plane2.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen1.bin 0x30 0x0000 file tmp/Demo14IntroScreensScreen1B_plane3.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen1.bin 0x80 0x2000 file tmp/Demo14IntroScreensScreen1A_map.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen1.bin 0x40 0x2000 file tmp/Demo14IntroScreensScreen1A_plane0.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen1.bin 0x40 0x4000 file tmp/Demo14IntroScreensScreen1A_plane1.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen1.bin 0x40 0x8000 file tmp/Demo14IntroScreensScreen1A_plane2.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen1.bin 0x40 0x0000 file tmp/Demo14IntroScreensScreen1A_plane3.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen1.bin 0x00
del /q tmp\Demo14FileResourcesScreen2_3.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen2_3.bin 0x01 0x9e0c bytes 0x18
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen2_3.bin 0x01 0x9c00 file tmp\Demo14ScaledSprites4TitleScreen.pal
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen2_3.bin 0x01 0x9e0c bytes 0x00
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen2_3.bin 0x90 0x4000 file tmp/Demo14IntroScreensScreen2_3B_map.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen2_3.bin 0x90 0x8000 file tmp/Demo14IntroScreensScreen2_3B_map.bin2
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen2_3.bin 0x30 0x2000 file tmp/Demo14IntroScreensScreen2_3B_plane0.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen2_3.bin 0x30 0x4000 file tmp/Demo14IntroScreensScreen2_3B_plane1.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen2_3.bin 0x30 0x8000 file tmp/Demo14IntroScreensScreen2_3B_plane2.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen2_3.bin 0x30 0x0000 file tmp/Demo14IntroScreensScreen2_3B_plane3.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen2_3.bin 0x80 0x2000 file tmp/Demo14IntroScreensScreen2_3A_map.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen2_3.bin 0x40 0x2000 file tmp/Demo14IntroScreensScreen2_3A_plane0.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen2_3.bin 0x40 0x4000 file tmp/Demo14IntroScreensScreen2_3A_plane1.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen2_3.bin 0x40 0x8000 file tmp/Demo14IntroScreensScreen2_3A_plane2.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen2_3.bin 0x40 0x0000 file tmp/Demo14IntroScreensScreen2_3A_plane3.bin
python ResourceGenerator\main.py tmp/Demo14FileResourcesScreen2_3.bin 0x00


popd

dir ..\tmp\ScaledSprites4.bin*
dir ..\tmp\Demo14ScaledSprites4*.pal
dir ..\tmp\Demo14FileResources*.bin
