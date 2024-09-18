rmdir /S /Q TempTestData
mkdir TempTestData
Release\Compression.exe -c TestData\1.bin TempTestData\1.bin
Release\Compression.exe -c TestData\2.bin TempTestData\2.bin
Release\Compression.exe -c TestData\3.bin TempTestData\3.bin
Release\Compression.exe -c TestData\4.bin TempTestData\4.bin
Release\Compression.exe -c TestData\5.bin TempTestData\5.bin
Release\Compression.exe -c TestData\6.bin TempTestData\6.bin
Release\Compression.exe -c TestData\7.bin TempTestData\7.bin
Release\Compression.exe -c TestData\8.bin TempTestData\8.bin
Release\Compression.exe -c TestData\9.bin TempTestData\9.bin

Release\Compression.exe -ce TestData\1.bin TempTestData\1.bin2
Release\Compression.exe -ce TestData\2.bin TempTestData\2.bin2
Release\Compression.exe -ce TestData\3.bin TempTestData\3.bin2
Release\Compression.exe -ce TestData\4.bin TempTestData\4.bin2
Release\Compression.exe -ce TestData\5.bin TempTestData\5.bin2
Release\Compression.exe -ce TestData\6.bin TempTestData\6.bin2
Release\Compression.exe -ce TestData\7.bin TempTestData\7.bin2
Release\Compression.exe -ce TestData\8.bin TempTestData\8.bin2
Release\Compression.exe -ce TestData\9.bin TempTestData\9.bin2

Release\Compression.exe -cu TestData\1.bin TempTestData\1.bin3
Release\Compression.exe -cu TestData\2.bin TempTestData\2.bin3
Release\Compression.exe -cu TestData\3.bin TempTestData\3.bin3
Release\Compression.exe -cu TestData\4.bin TempTestData\4.bin3
Release\Compression.exe -cu TestData\5.bin TempTestData\5.bin3
Release\Compression.exe -cu TestData\6.bin TempTestData\6.bin3
Release\Compression.exe -cu TestData\7.bin TempTestData\7.bin3
Release\Compression.exe -cu TestData\8.bin TempTestData\8.bin3
Release\Compression.exe -cu TestData\9.bin TempTestData\9.bin3


Release\Compression.exe -c64 TestData\1.bin TempTestData\1.prg $400 $400
Release\Compression.exe -c64 TestData\2.bin TempTestData\2.prg $400 $400
Release\Compression.exe -c64 TestData\3.bin TempTestData\3.prg $400 $400
Release\Compression.exe -c64 TestData\4.bin TempTestData\4.prg $400 $400
Release\Compression.exe -c64 TestData\5.bin TempTestData\5.prg $400 $400
Release\Compression.exe -c64 TestData\6.bin TempTestData\6.prg $400 $400
Release\Compression.exe -c64 TestData\7.bin TempTestData\7.prg $400 $400
Release\Compression.exe -c64 TestData\8.bin TempTestData\8.prg $400 $400
Release\Compression.exe -c64 TestData\9.bin TempTestData\9.prg $400 $400


Release\Compression.exe -c64 C:\work\C64\RacingGame\Original\OriginalData.prg c:\temp\tc64.prg 2048
Release\Compression.exe -c64b C:\work\C64\RacingGame\Original\OriginalData.prg c:\temp\tc64b.prg 2048
Release\Compression.exe -c64m C:\work\C64\RacingGame\Original\OriginalData.prg c:\temp\tc64m.prg 2048
Release\Compression.exe -c64mb C:\work\C64\RacingGame\Original\OriginalData.prg c:\temp\tc64mb.prg 2048
Release\Compression.exe -c64e C:\work\C64\RacingGame\Original\OriginalData.prg c:\temp\tc64e.prg 2048
Release\Compression.exe -c64eb C:\work\C64\RacingGame\Original\OriginalData.prg c:\temp\tc64eb.prg 2048
Release\Compression.exe -c64em C:\work\C64\RacingGame\Original\OriginalData.prg c:\temp\tc64em.prg 2048
Release\Compression.exe -c64emb C:\work\C64\RacingGame\Original\OriginalData.prg c:\temp\tc64emb.prg 2048
Release\Compression.exe -c64mr C:\work\C64\RacingGame\Original\OriginalData.prg c:\temp\tc64mr.prg 2048
Release\Compression.exe -c64mrb C:\work\C64\RacingGame\Original\OriginalData.prg c:\temp\tc64mrb.prg 2048

dir TempTestData\*.bin
dir c:\temp\tc64*.prg /os
