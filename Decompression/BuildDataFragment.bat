@echo off
..\acme.exe -v3 --msvc %2 %3 %4
..\ExternalTools\Gnu\bin\sed.exe -n -e "/LauncherAddress/s/^/static int %1_/p" TestDecompression.map >t.txt
..\ExternalTools\Gnu\bin\sed.exe -n -i -e "s/\$/ 0x/p" t.txt
..\ExternalTools\Gnu\bin\sed.exe -n -e "s/;.*$/;/p" t.txt >>PrgData.cpp
echo | set /p="..\bin\BinToC.exe TestDecompression.prg -skip 2 -keep $">t.bat
..\ExternalTools\Gnu\bin\sed.exe -n -e "/LauncherCodeSize/s/;.*$//p" TestDecompression.map | ..\ExternalTools\Gnu\bin\sed.exe -n -e "s/^.*\$//p" >>t.bat
echo static u8 %1_Data[] = {>>PrgData.cpp
call t.bat >>PrgData.cpp
echo };>>PrgData.cpp


del t.txt
del t.bat
del sed*
