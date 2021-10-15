echo Assembling with ACME
del /q Loaders.bin
..\..\acme.exe --lib  ../ --lib ../../  -v4 --msvc Loaders.a

if not exist Loaders.bin goto :error

echo Writing data
rem Tiny header auto-boot loaders are used so use this...
..\..\bin\TapeTool.exe n 1 wn "test.tap" m "Loaders.map" ocb1 Loaders.bin otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd otl$c0 otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfbr "..\R.MUSC000.prg" 1 otl$c0 otfbr "..\..\BlankProjectComp\Tusari.scr" 2 $2 $3eb .BitmapLogoScreen otl$c0 otfbr "..\..\BlankProjectComp\Tusari.col" 3 otl$c0 otfbr "..\..\BlankProjectComp\Tusari.bmp" 4 $2 $1f43 .BitmapLogo otl$c0 otfbr "..\TestMultiplexor.prg" 5 otl$c0 c
rem If none of the auto-boot loaders are chosen in the "_config.a" file then the real loader written with "otft" needs to use "otf" instead.
rem ..\..\bin\TapeTool.exe wn "test.tap" m "Loaders.map" ocb1 Loaders.bin otl$c0 otf "Loaders.bin" 255 .RealCodeStart .RealCodeEnd .MainSecondLoaderStart otl$c0 otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfbr "..\R.MUSC000.prg" 1 otl$c0 otfbr "..\..\BlankProjectComp\Tusari.scr" 2 $2 $3eb .BitmapLogoScreen otl$c0 otfbr "..\..\BlankProjectComp\Tusari.col" 3 otl$c0 otfbr "..\..\BlankProjectComp\Tusari.bmp" 4 $2 $1f43 .BitmapLogo otl$c0 otfbr "..\TestMultiplexor.prg" 5 otl$c0 c


echo run test.tap
test.tap

goto end

:error
echo Loaders.bin not created!
exit /B -1
:end
