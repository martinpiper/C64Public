echo Assembling with ACME
del /q Loaders.bin
..\..\acme.exe --lib  ../ --lib ../../  -v4 --msvc _config.a  Loaders.a

if not exist Loaders.bin goto :error

echo Writing data
rem Tiny header auto-boot loaders are used so use this...
..\..\bin\TapeTool.exe n 1 wn "test.tap" m "Loaders.map" ocb1 Loaders.bin otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd otl$c0 otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfbr "..\R.MUSC000.prg" 1 otl$c0 otfbr "..\..\BlankProjectComp\Tusari.scr" 2 $2 $3eb .BitmapLogoScreen otl$c0 otfbr "..\..\BlankProjectComp\Tusari.col" 3 otl$c0 otfbr "..\..\BlankProjectComp\Tusari.bmp" 4 $2 $1f43 .BitmapLogo otl$c0 otfbr "..\TestMultiplexor.prg" 5 otl$c0 c
rem If none of the auto-boot loaders are chosen in the "_config.a" file then the real loader written with "otft" needs to use "otf" instead.
rem ..\..\bin\TapeTool.exe wn "test.tap" m "Loaders.map" ocb1 Loaders.bin otl$c0 otf "Loaders.bin" 255 .RealCodeStart .RealCodeEnd .MainSecondLoaderStart otl$c0 otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfbr "..\R.MUSC000.prg" 1 otl$c0 otfbr "..\..\BlankProjectComp\Tusari.scr" 2 $2 $3eb .BitmapLogoScreen otl$c0 otfbr "..\..\BlankProjectComp\Tusari.col" 3 otl$c0 otfbr "..\..\BlankProjectComp\Tusari.bmp" 4 $2 $1f43 .BitmapLogo otl$c0 otfbr "..\TestMultiplexor.prg" 5 otl$c0 c


rem Produce various testing TAP files with different variance
copy /Y test.tap test_p.tap
..\..\bin\TapeTool.exe d test_p.tap 7

copy /Y test.tap test_m.tap
..\..\bin\TapeTool.exe d test_m.tap -8

rem Random variance is the hardest for the kernal to deal with since the calibration code is more geared towards a tape deck that runs consistently faster or slower
rem A random variance of 3 seems to be the maximum supported
copy /Y test.tap test_r.tap
..\..\bin\TapeTool.exe v test_r.tap 3


echo run test.tap
test.tap

echo run test_p.tap
test_p.tap

echo run test_m.tap
test_m.tap

echo run test_r.tap
test_r.tap

goto end

:error
echo Loaders.bin not created!
exit /B -1
:end
