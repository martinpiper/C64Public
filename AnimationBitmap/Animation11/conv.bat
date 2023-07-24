setlocal EnableDelayedExpansion

C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\ffmpeg.exe -i "C:\Users\Martin Piper\Downloads\Digital Leisure\Dragon's Lair\VIDEO_TS\VTS_06_1.VOB" -y -acodec pcm_u8 -ar 22050 -ac 1 t.wav
rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\ffmpeg.exe -i "C:\Downloads\spaceace.mp4"  -y -acodec pcm_u8 -ar 22050 -ac 1 t.wav

del *.bmp
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\ffmpeg.exe -i "C:\Users\Martin Piper\Downloads\Digital Leisure\Dragon's Lair\VIDEO_TS\VTS_06_1.VOB" -frames 992 -f image2 frm%%05d.bmp
rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\ffmpeg.exe -i "C:\Downloads\spaceace.mp4" -frames 992 -f image2 frm%%05d.bmp

rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -resize 200 -gravity north -background black -extent 320x200 -type truecolor frm*.bmp
rem 898,347 bytes


rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -resize 200 -gravity north -background black -extent 320x200 -wavelet-denoise 0.1 -type truecolor frm*.bmp
rem 898,374 bytes


rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -resize 200 -gravity north -background black -extent 320x200 -kuwahara 4 -type truecolor frm*.bmp
rem 557,872 bytes
rem Looks very abstract though :)


rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -resize 200 -gravity north -background black -extent 320x200 -kuwahara 2 -type truecolor frm*.bmp
rem 647,989 bytes


rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -resize 200 -gravity north -background black -extent 320x200 -kuwahara 0.5 -type truecolor frm*.bmp
rem 808,463 bytes
rem The best so far, not much detail lost




rem ** Resize to smaller to take into account the samples
rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -resize 150 -gravity north -background black -extent 320x200 -type truecolor frm*.bmp
rem 898,347 bytes

rem This is used with samples, it's small
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -resize 120 -gravity center -background black -extent 320x200 -type truecolor frm*.bmp

rem Use this when using the windowed conversion mode
rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -resize 176 -gravity NorthWest -background black -extent 320x200 -type truecolor frm*.bmp


rem This completes the palette remap with dither and resize
rem TODO: Black point correction
rem +dither turns off dithering which improves compression
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose +dither -scale "50%%x100%%" -remap ..\..\CharPack\Palette.bmp  -resize "160x200" *.bmp
rem https://legacy.imagemagick.org/Usage/quantize/#colors
rem This produces dithering with a lot of change between frames, not good for compression, better visual quality
rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -dither FloydSteinberg -scale "50%%x100%%" -remap ..\..\CharPack\Palette.bmp  -resize "160x200" *.bmp
rem This produces dithering with less change between frames, not good for compression (but slightly improved compared to FloydSteinberg?), slightly worse visual quality than FloydSteinberg
rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -dither Riemersma -scale "50%%x100%%" -remap ..\..\CharPack\Palette.bmp  -resize "160x200" *.bmp

rem Expand the image back again
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -sample "200%%x100%%" -depth 8 -type truecolor *.bmp
