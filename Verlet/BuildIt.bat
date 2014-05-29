@echo off
del Verlet.prg

..\acme.exe -v3 --msvc Verlet.a
if not exist Verlet.prg goto error
..\bin\LZMPi.exe -c64 Verlet.prg Verlet.prg $400 >t.txt
if not exist Verlet.prg goto error

..\acme.exe -v3 --msvc CartBoot.a

rem Code plus large lookup tables
..\bin\MakeCart.exe -n -b 0 -r CartBoot.prg -c 0 2 $10000 -w -r Multiply.bin -b 1 -c 0 $0000 $10000 -w -b 2 -c 0 $2000 $10000 -w -b 3 -c 0 $4000 $10000 -w -b 4 -c 0 $6000 $10000 -w -b 5 -c 0 $8000 $10000 -w -b 6 -c 0 $a000 $10000 -w -b 7 -c 0 $c000 $10000 -w -b 8 -c 0 $e000 $10000 -w -o Verlet.crt >>t.txt

if not exist Verlet.crt goto error2

goto end
:error2
echo Verlet.crt not created!
:error
echo Verlet.prg not created!
:end
