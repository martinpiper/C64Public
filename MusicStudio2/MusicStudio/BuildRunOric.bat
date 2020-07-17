cd /D C:\Work\C64\MusicStudio2\MusicStudio
del /q t.tap
..\..\acme.exe -v4 --msvc --lib ..\..\ --lib ..\MusicStudioConvert\ ..\MusicStudioConvert\HeaderOric.a
if not exist t.tap goto error:
goto end

:error
echo t.tap not created!
exit /B -1
:end

cd /d C:\Users\Martin Piper\Downloads\Oricutron_win32_v12
oricutron.exe -t C:\Work\C64\MusicStudio2\MusicStudio\t.tap
