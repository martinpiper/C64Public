rem C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe -verbose c:\Downloads\21742405_10156586661483362_3258881911300292608_n.gif -coalesce -resize 320 -type truecolor frm%%05d.bmp


C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe -verbose c:\Downloads\21742405_10156586661483362_3258881911300292608_n.gif -coalesce -resize 160 -type truecolor sml%%05d.bmp

C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe -verbose sml*.bmp -background black -extent 320x200 frm%%05d.bmp
