del *.bmp
copy "C:\Users\Martin Piper\Downloads\HMScenes1\scene6-everyframe\scene6-everyframe\*.bmp" cut1*.bmp

C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe -verbose cut*.bmp -background black -extent 320x200 -type truecolor frm%%05d.bmp

rem exit /B

C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -brightness-contrast -80 frm00000.bmp
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -brightness-contrast -60 frm00001.bmp
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -brightness-contrast -40 frm00002.bmp
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -brightness-contrast -20 frm00003.bmp
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -brightness-contrast -10 frm00004.bmp


C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -brightness-contrast -10 frm00145.bmp
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -brightness-contrast -20 frm00146.bmp
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -brightness-contrast -40 frm00147.bmp
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -brightness-contrast -60 frm00148.bmp
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -brightness-contrast -80 frm00149.bmp
