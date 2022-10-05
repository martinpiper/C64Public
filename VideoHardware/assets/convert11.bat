@echo off
pushd %~dp0

if not exist ..\tmp mkdir ..\tmp

echo Calculate optimal separate palettes and combine

echo Convert chars
..\..\bin\LZMPi.exe -cr "C:\VICE\C64\chargen" ..\tmp\Demo11Chars_plane0.cmp


popd
