setlocal EnableDelayedExpansion
del *.bmp
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\ffmpeg.exe -i "C:\Users\Martin Piper\Downloads\Bad Apple.mp4" -f image2 frm%05d.bmp

C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -resize 320 -gravity north -background black -extent 320x200 -threshold 50%% -type truecolor frm*.bmp
