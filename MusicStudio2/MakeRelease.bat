@echo off
del t.lbl t.map t.prg t.sid /s

cd MusicStudio
C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe MusicStudio.sln /m /p:Configuration=Release /p:Platform=Win32
cd ..

if not exist MusicStudio\Release\MusicStudioLauncher.exe goto error
cd ..
cd ..
mkdir Release
cd Release
rmdir /s /q MusicStudio2.2
mkdir MusicStudio2.2
cd MusicStudio2.2
copy ..\..\C64\MusicStudio2\MusicStudio\Release\MusicStudioLauncher.exe
copy ..\..\C64\MusicStudio2\MusicStudio\ReadMe.txt
mkdir ExampleFiles
cd ExampleFiles
xcopy ..\..\..\C64\MusicStudio2\ExampleFiles\*.* /s
cd ..
mkdir MusicStudio2Bin
cd MusicStudio2Bin
copy ..\..\..\C64\acme.exe
xcopy ..\..\..\C64\stdlib\*.* stdlib\ /s
mkdir MusicStudio2
cd MusicStudio2
xcopy ..\..\..\..\C64\MusicStudio2\MusicStudio\Release\MusicStudio.exe MusicStudio\
xcopy ..\..\..\..\C64\MusicStudio2\MusicStudio\UserImages.bmp MusicStudio\
xcopy ..\..\..\..\C64\MusicStudio2\MusicStudioConvert\HeaderPRG.a MusicStudioConvert\
xcopy ..\..\..\..\C64\MusicStudio2\MusicStudioConvert\HeaderSelf.a MusicStudioConvert\
xcopy ..\..\..\..\C64\MusicStudio2\MusicStudioConvert\HeaderSID.a MusicStudioConvert\
xcopy ..\..\..\..\C64\MusicStudio2\MusicStudioConvert\HeaderSID2.a MusicStudioConvert\
xcopy ..\..\..\..\C64\MusicStudio2\MusicStudioConvert\HeaderOric.a MusicStudioConvert\
xcopy ..\..\..\..\C64\MusicStudio2\MusicStudioConvert\MusicPlayer2.a MusicStudioConvert\

cd ..\..\..\
explorer .

cd ..\C64\MusicStudio2

echo Done!

goto end

:error
echo Missing file.
:end
pause
