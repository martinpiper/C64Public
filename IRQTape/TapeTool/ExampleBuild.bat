@echo off

echo Setup the config file.
echo ; >_config.a
echo OpenBorders = 1 >>_config.a
echo MainLoad_EnableScreen = 1 >>_config.a
echo EnableBlockChecksum = 1 >>_config.a
echo EnableBlockCompression = 1 >>_config.a
echo TapeTurboSpeed = $80 >>_config.a
echo BitmapDisplay = 1 >>_config.a

rem Enable one of these loaders
echo NovaishLoad = 1 >>_config.a
rem echo SmallLoader = 1 >>_config.a
rem echo MartyLoad = 1 >>_config.a



echo Assembling with ACME
acme.exe --lib  ../ --lib ../../  -v4 --msvc _config.a  Loaders.a

echo Writing data

rem Writes the kernal auto-run data
TapeTool.exe n 1 wn "test.tap" m "Loaders.map" ocb1 Loaders.bin c

rem Tiny header auto-run loaders are used so use this...
rem Writes the first turbo data, the main loader, with the "TinyHeader" option
TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd c
rem If none of the auto-run loaders are chosen in the "_config.a" file then the real loader written with "otft" needs to use "otf" instead.
rem Writes the first turbo data, the main loader, with the full header
rem TapeTool.exe w  "test.tap" a m "Loaders.map" otl$c0 otf "Loaders.bin" 255 .RealCodeStart .RealCodeEnd .MainSecondLoaderStart c

rem Write the rest of the file data, with block compression enabled
TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 c
TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 otfbr "R.MUSC000.prg" 1 c
TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 otfbr "Tusari.scr" 2 $2 $3eb .BitmapLogoScreen c
TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 otfbr "Tusari.col" 3 c
TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 otfbr "Tusari.bmp" 4 $2 $1f43 .BitmapLogo c
TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 otfbr "TestMultiplexor.prg" 5 c
rem Writes a final lead out at the end of the tape
TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 c


echo run test.tap
test.tap
