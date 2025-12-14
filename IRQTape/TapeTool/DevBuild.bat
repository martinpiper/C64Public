echo Assembling with ACME
del /q Loaders.bin
..\..\acme.exe --lib  ../ --lib ../../  -v4 --msvc _config.a  Loaders.a

if not exist Loaders.bin goto :error

echo Writing data

rem Writes the kernal auto-run data
..\..\bin\TapeTool.exe n 1 wn "test.tap" m "Loaders.map" ocb1 Loaders.bin c

rem Tiny header auto-run loaders are used so use this...
rem Writes the first turbo data, the main loader, with the "TinyHeader" option
..\..\bin\TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd c
rem If none of the auto-run loaders are chosen in the "_config.a" file then the real loader written with "otft" needs to use "otf" instead.
rem Writes the first turbo data, the main loader, with the full header
rem ..\..\bin\TapeTool.exe w  "test.tap" a m "Loaders.map" otl$c0 otf "Loaders.bin" 255 .RealCodeStart .RealCodeEnd .MainSecondLoaderStart c

rem Write the rest of the file data, with block compression enabled
..\..\bin\TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 c
..\..\bin\TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 otfbr "..\R.MUSC000.prg" 1 c
..\..\bin\TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 otfbr "..\..\BlankProjectComp\Tusari.scr" 2 $2 $3eb .BitmapLogoScreen c
..\..\bin\TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 otfbr "..\..\BlankProjectComp\Tusari.col" 3 c
..\..\bin\TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 otfbr "..\..\BlankProjectComp\Tusari.bmp" 4 $2 $1f43 .BitmapLogo c
..\..\bin\TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 otfbr "..\TestMultiplexor.prg" 5 c
rem Writes a final lead out at the end of the tape
..\..\bin\TapeTool.exe n 1 w  "test.tap" a m "Loaders.map" otl$c0 c



rem Produce various testing TAP files with different variance
copy /Y test.tap test_p.tap
..\..\bin\TapeTool.exe d test_p.tap 7

copy /Y test.tap test_m.tap
..\..\bin\TapeTool.exe d test_m.tap -8

rem Random variance is the hardest for the kernal to deal with since the calibration code is more geared towards a tape deck that runs consistently faster or slower
rem A random variance of 3 seems to be the maximum supported
copy /Y test.tap test_r.tap
..\..\bin\TapeTool.exe v test_r.tap 3



rem Applying percentage variance
rem 115 is equivalent to d using 7, comparing the kernal short pulse result
copy /Y test.tap test_pp.tap
..\..\bin\TapeTool.exe d%% test_pp.tap 119

rem 85 is equivalent to d using -8, comparing the kernal short pulse result
copy /Y test.tap test_mp.tap
..\..\bin\TapeTool.exe d%% test_mp.tap 85

copy /Y test.tap test_rp.tap
..\..\bin\TapeTool.exe v%% test_rp.tap 114
copy /Y test.tap test_rm.tap
..\..\bin\TapeTool.exe v%% test_rm.tap 90



echo run test.tap
test.tap


rem Debug, skip the rest
rem goto end

echo run test_p.tap
test_p.tap

echo run test_m.tap
test_m.tap

echo run test_r.tap
test_r.tap


echo run test_pp.tap
test_pp.tap

echo run test_mp.tap
test_mp.tap

echo run test_rp.tap
test_rp.tap

echo run test_rm.tap
test_rm.tap


goto end

:error
echo Loaders.bin not created!
exit /B -1
:end
