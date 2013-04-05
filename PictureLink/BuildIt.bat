@echo off
del PictureLink.prg

echo Compress the picture data from the Koala paint file
..\bin\LZMPi.exe -c ..\BerzerkRedux\BerzerkLoadingPic.prg PicBitmap 2 8000
..\bin\LZMPi.exe -c ..\BerzerkRedux\BerzerkLoadingPic.prg PicScreen 8002 1000
..\bin\LZMPi.exe -c ..\BerzerkRedux\BerzerkLoadingPic.prg PicColour 9002 1000

..\bin\LZMPi.exe -c fw86atc000.prg fw86atc000Comp.prg 2

..\acme.exe -v3 --msvc PictureLink.a

:end
