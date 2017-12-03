@echo off
IF EXIST AnimationBitmap.prg del /Q AnimationBitmap.prg

..\acme.exe -v3 --msvc AnimationBitmap.a
if not exist AnimationBitmap.prg goto error
..\bin\LZMPi.exe -c64mr AnimationBitmap.prg AnimationBitmap.prg $c000 >t.txt
if not exist AnimationBitmap.prg goto error

..\bin\MakeCart.exe -te -n -a $8000 -b 0 -r ..\Citadel2\Citadel2Cart.prg -c 0 2 $ffff -w -a $a000 -b 0 -c $1ffc 2 4 -w -r AnimationBitmap.prg -a $8000 -b 1 -c 0 $0001 $ffff -w -a $a000 -b 1 -c 0 $2001 $ffff -w -a $8000 -b 2 -c 0 $4001 $ffff -w -a $a000 -b 2 -c 0 $6001 $ffff -w -a $8000 -b 3 -c 0 $8001 $ffff -w -a $a000 -b 3 -c 0 $a001 $ffff -w -a $8000 -b 4 -m data\frm0*.del $4000 -o AnimationBitmap.crt
rem >>t.txt


goto end
:error
echo AnimationBitmap.prg not created!
exit -1
:end
