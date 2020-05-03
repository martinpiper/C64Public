pushd %~dp0

rmdir /s /q ..\..\Builds\VideoHardwareSDK
mkdir ..\..\Builds
mkdir ..\..\Builds\VideoHardwareSDK

mkdir ..\..\Builds\VideoHardwareSDK\BDD6502
mkdir ..\..\Builds\VideoHardwareSDK\BDD6502\target

mkdir ..\..\Builds\VideoHardwareSDK\BombJack
mkdir ..\..\Builds\VideoHardwareSDK\BombJack\stdlib

mkdir ..\..\Builds\VideoHardwareSDK\C64
mkdir ..\..\Builds\VideoHardwareSDK\C64\stdlib
mkdir ..\..\Builds\VideoHardwareSDK\C64\VideoHardware

mkdir ..\..\Builds\VideoHardwareSDK\ImageToBitplane
mkdir ..\..\Builds\VideoHardwareSDK\ImageToBitplane\target


xcopy ..\..\BDD6502\features\* ..\..\Builds\VideoHardwareSDK\BDD6502\features\ /s
xcopy ..\..\BDD6502\target\*jar-with-dependencies.jar ..\..\Builds\VideoHardwareSDK\BDD6502\target\
copy ..\..\BDD6502\target\syntax.html ..\..\Builds\VideoHardwareSDK\BDD6502\
copy ..\..\BDD6502\README.md ..\..\Builds\VideoHardwareSDK\BDD6502\


xcopy ..\..\BombJack\README.md ..\..\Builds\VideoHardwareSDK\BombJack\
xcopy ..\..\BombJack\*.pdsprj ..\..\Builds\VideoHardwareSDK\BombJack\
xcopy ..\..\BombJack\V3.0\* ..\..\Builds\VideoHardwareSDK\BombJack\V3.0\
xcopy ..\..\BombJack\stdlib\* ..\..\Builds\VideoHardwareSDK\BombJack\stdlib\ /s


xcopy ..\..\C64\README.md ..\..\Builds\VideoHardwareSDK\C64\
xcopy ..\..\C64\acme.exe ..\..\Builds\VideoHardwareSDK\C64\
xcopy ..\..\C64\bin\* ..\..\Builds\VideoHardwareSDK\C64\bin\ /s
xcopy ..\..\C64\stdlib\* ..\..\Builds\VideoHardwareSDK\C64\stdlib\ /s
xcopy ..\..\C64\VideoHardware\README.md ..\..\Builds\VideoHardwareSDK\C64\VideoHardware\
copy "..\..\C64\VideoHardware\SDK README.md" ..\..\Builds\VideoHardwareSDK\README.md
xcopy ..\..\C64\VideoHardware\BuildIt.bat ..\..\Builds\VideoHardwareSDK\C64\VideoHardware\
xcopy ..\..\C64\VideoHardware\FeatureEditor.bat ..\..\Builds\VideoHardwareSDK\C64\VideoHardware\
xcopy ..\..\C64\VideoHardware\asm\* ..\..\Builds\VideoHardwareSDK\C64\VideoHardware\asm\ /s
xcopy ..\..\C64\VideoHardware\assets\* ..\..\Builds\VideoHardwareSDK\C64\VideoHardware\assets\ /s
xcopy ..\..\C64\VideoHardware\features\* ..\..\Builds\VideoHardwareSDK\C64\VideoHardware\features\ /s
xcopy ..\..\C64\VideoHardware\testdata\* ..\..\Builds\VideoHardwareSDK\C64\VideoHardware\testdata\ /s


xcopy ..\..\ImageToBitplane\README.md ..\..\Builds\VideoHardwareSDK\ImageToBitplane\
xcopy ..\..\ImageToBitplane\target\*jar-with-dependencies.jar ..\..\Builds\VideoHardwareSDK\ImageToBitplane\target\
