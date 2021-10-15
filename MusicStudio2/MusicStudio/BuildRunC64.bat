cd /D C:\Work\C64\MusicStudio2\MusicStudio
del /q t.prg
..\..\acme.exe -v4 --msvc --lib ..\..\ --lib ..\MusicStudioConvert\ ..\MusicStudioConvert\HeaderSelf.a
if not exist t.prg goto error:
goto end

:error
echo t.tap not created!
exit /B -1
:end

cd /d C:\Downloads\WinVICE-3.1-x86-r34062\WinVICE-3.1-x86-r34062\
x64sc.exe -remotemonitor C:\Work\C64\MusicStudio2\MusicStudio\t.prg
