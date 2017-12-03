cd TapeTool
"c:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv.exe" ReplicaNetWork.sln /Build "ReleaseMT|Win32" /Out c:\Temp\build6.txt
cd ..

pause

xcopy TapeTool\Release\TapeTool.exe ..\bin /y
rmdir /s /q ..\..\TrialBuild2\TapeToolBuild
mkdir ..\..\TrialBuild2\TapeToolBuild

xcopy ..\stdlib ..\..\TrialBuild2\TapeToolBuild\stdlib\ /s
xcopy TapeTool\_config.a ..\..\TrialBuild2\TapeToolBuild
xcopy ..\acme.exe ..\..\TrialBuild2\TapeToolBuild
xcopy TapeTool\ExampleBuild.bat ..\..\TrialBuild2\TapeToolBuild
xcopy TapeTool\Loaders.a ..\..\TrialBuild2\TapeToolBuild
xcopy R.MUS8000.PRG ..\..\TrialBuild2\TapeToolBuild
xcopy TapeTool\ReadMe.txt ..\..\TrialBuild2\TapeToolBuild
xcopy ScrollerMusicLoader.a ..\..\TrialBuild2\TapeToolBuild
xcopy TapeLoader*.a ..\..\TrialBuild2\TapeToolBuild
xcopy TapeTool\Release\TapeTool.exe ..\..\TrialBuild2\TapeToolBuild
xcopy TestMultiplexor.prg ..\..\TrialBuild2\TapeToolBuild
xcopy TurboTapeVars.a ..\..\TrialBuild2\TapeToolBuild
xcopy TurboTapeWrite.a ..\..\TrialBuild2\TapeToolBuild
