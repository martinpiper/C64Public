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


Release\Compression.exe -c64 TestData\1.bin TempTestData\1.prg $400 $400
Release\Compression.exe -c64 TestData\2.bin TempTestData\2.prg $400 $400
Release\Compression.exe -c64 TestData\3.bin TempTestData\3.prg $400 $400
Release\Compression.exe -c64 TestData\4.bin TempTestData\4.prg $400 $400
Release\Compression.exe -c64 TestData\5.bin TempTestData\5.prg $400 $400
Release\Compression.exe -c64 TestData\6.bin TempTestData\6.prg $400 $400
Release\Compression.exe -c64 TestData\7.bin TempTestData\7.prg $400 $400
Release\Compression.exe -c64 TestData\8.bin TempTestData\8.prg $400 $400
Release\Compression.exe -c64 TestData\9.bin TempTestData\9.prg $400 $400

dir TempTestData\*.bin
