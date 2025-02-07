@echo off
..\acme.exe -v4 --msvc %3 %4 %5 %6
copy TestDecompression.map DebugFiles\%1_TestDecompression.map
copy TestDecompression.lbl DebugFiles\%1_TestDecompression.lbl
copy TestDecompression.pdb DebugFiles\%1_TestDecompression.pdb

..\ExternalTools\Gnu\bin\sed.exe -n -e "/LauncherAddress/s/^/static int %2_/p" TestDecompression.map >t.txt
..\ExternalTools\Gnu\bin\sed.exe -n -i -e "s/\$/ 0x/p" t.txt
..\ExternalTools\Gnu\bin\sed.exe -n -e "s/;.*$/;/p" t.txt >>PrgData.cpp
echo | set /p="..\bin\BinToC.exe TestDecompression.prg -skip 2 -keep $">t.bat
..\ExternalTools\Gnu\bin\sed.exe -n -e "/LauncherCodeSize/s/;.*$//p" TestDecompression.map | ..\ExternalTools\Gnu\bin\sed.exe -n -e "s/^.*\$//p" >>t.bat
echo static u8 %2_Data[] = {>>PrgData.cpp
call t.bat >>PrgData.cpp
echo };>>PrgData.cpp

del TestDecompression.map TestDecompression.lbl TestDecompression.pdb
del t.txt
del t.bat
del sed*
