del *.bmp
rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe -verbose "C:\Downloads\ThunderBlade-reduced.mp4" -resize 320 -type truecolor cut%%05d.bmp
rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe -verbose "C:\Downloads\ThunderBlade-reduced.mp4" -resize 160 -type truecolor cut%%05d.bmp

C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe -verbose "C:\Downloads\AfterBurnerII-reduced.mp4" -resize 320 -type truecolor cut%%05d.bmp
rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe -verbose "C:\Downloads\AfterBurnerII-reduced.mp4" -resize 240 -type truecolor cut%%05d.bmp

C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe -verbose cut*.bmp -background black -gravity north -extent 320x200 frm%%05d.bmp
rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe -verbose cut*.bmp -background black -extent 320x200 frm%%05d.bmp
