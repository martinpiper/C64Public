setlocal EnableDelayedExpansion

C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\ffmpeg.exe -i "C:\Users\Martin Piper\Downloads\Digital Leisure\Dragon's Lair\VIDEO_TS\VTS_06_1.VOB" -y -acodec pcm_u8 -ar 22050 -ac 1 t.wav

del *.bmp
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\ffmpeg.exe -i "C:\Users\Martin Piper\Downloads\Digital Leisure\Dragon's Lair\VIDEO_TS\VTS_06_1.VOB" -frames 992 -f image2 frm%%05d.bmp

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
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -resize 150 -gravity north -background black -extent 320x200 -type truecolor frm*.bmp
rem 898,347 bytes
