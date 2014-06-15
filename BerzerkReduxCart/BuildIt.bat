@echo off
del BerzerkRedux.crt

..\acme.exe -v3 --msvc BerzerkReduxCart.a
if not exist BerzerkReduxCart.prg goto error

..\bin\MakeCart.exe -n -b 0 -r BerzerkReduxCart.prg -c 0 2 $ffff -w -r ../BerzerkRedux/BerzerkRedux.prg -b 1 -c 0 $0001 $ffff -w -b 2 -c 0 $2001 $ffff -w -b 3 -c 0 $4001 $ffff -w -b 4 -c 0 $6001 $ffff -w -b 5 -c 0 $8001 $ffff -w -b 6 -c 0 $a001 $ffff -w -b 7 -r fw86muscomp.prg -c 0 2 $ffff -w -o BerzerkReduxCart.crt

goto end
:error
echo BerzerkReduxCart.prg not created!
exit -1
:end
