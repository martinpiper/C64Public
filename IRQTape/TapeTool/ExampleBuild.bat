@echo off

echo Setup the config file.
echo OpenBorders = 1 >_config.a
echo MainLoad_EnableScreen = 1 >>_config.a
echo EnableBlockChecksum = 1 >>_config.a
echo EnableBlockCompression = 1 >>_config.a
echo TapeTurboSpeed = $80 >>_config.a

rem Enable one of these loaders
echo NovaishLoad = 1 >>_config.a
rem echo SmallLoader = 1 >>_config.a
rem echo MartyLoad = 1 >>_config.a



echo Assembling with ACME
acme.exe --lib  ../ --lib ../../  -v4 --msvc Loaders.a

echo Writing data
rem Tiny header auto-boot loaders are used so use this...
TapeTool.exe wn "test.tap" m "Loaders.map" ocb1 Loaders.bin otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd otl$c0 otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfbr "R.MUS8000.PRG" 1 otl$c0 otfbr "TestMultiplexor.prg" 2 otl$c0 c
rem If none of the auto-boot loaders are chosen in the "_config.a" file then the real loader written with "otft" needs to use "otf" instead.
rem TapeTool.exe wn "test.tap" m "Loaders.map" ocb1 Loaders.bin otl$c0 otf "Loaders.bin" 255 .RealCodeStart .RealCodeEnd .MainSecondLoaderStart otl$c0 otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfbr "R.MUS8000.PRG" 1 otl$c0 otfbr "TestMultiplexor.prg" 2 otl$c0 c


echo run test.tap
test.tap
