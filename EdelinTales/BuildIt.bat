@echo off
del SpriteTest.prg

..\acme.exe -v3 --msvc SpriteTest.a
if not exist SpriteTest.prg goto error
..\bin\LZMPi.exe -c64b SpriteTest.prg SpriteTest.prg $800 >t.txt
if not exist SpriteTest.prg goto error
goto end
:error
echo SpriteTest.prg not created!
exit -1
:end
