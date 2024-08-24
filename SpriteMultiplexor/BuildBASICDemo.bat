..\acme.exe --lib ..\ --lib ..\..\ -v4 --msvc BASICInterface.a
..\bin\Tok64.exe /stomp /toprg BASICDemo.txt

..\ExternalTools\C1541\c1541.exe -attach BASICDemo.d64 -format basicdemo,69 -write BASICDemo.prg basicdemo -write TestData\Sprites.prg sprites -write target\BASICInterface.prg mplxc000
