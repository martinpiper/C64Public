@echo off
IF EXIST Animation.prg del /Q Animation.prg

..\acme.exe -v4 --msvc Animation.a
if not exist Animation.prg goto error
..\bin\LZMPi.exe -c64 Animation.prg Animation.prg $400 >t.txt
if not exist Animation.prg goto error


..\bin\MakeCart.exe -te -n -a $8000 -b 0 -r ..\Citadel2\Citadel2Cart.prg -c 0 2 $ffff -w -a $a000 -b 0 -c $1ffc 2 4 -w -r Animation.prg -a $8000 -b 1 -c 0 $0001 $ffff -w -a $8000 -b 4 -m Data\frm0*.del $4000 -o Animation.crt
rem  >>t.txt
rem -a $a000 -b 1 -c 0 $2001 $ffff -w -a $8000 -b 2 -c 0 $4001 $ffff -w -a $a000 -b 2 -c 0 $6001 $ffff -w -a $8000 -b 3 -c 0 $8001 $ffff -w -a $a000 -b 3 -c 0 $a001 $ffff -w

goto end
:error
echo Animation.prg not created!
exit -1
:end
