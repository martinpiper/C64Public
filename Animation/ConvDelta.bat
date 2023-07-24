mkdir Data
del Data\*.del /q

rem $1ff8 because the decompressed data loads at $e000 and we don't want to hit the IRQ vectors at $1ff8
DeltaCompression\Release\DeltaCompression.exe Data\frm*05d.sch $400 1 1 28 2 $1000 $1ff8 Data\frm*05d.del
rem DeltaCompression\Release\DeltaCompression.exe Data\frm*05d.sch $400 1 1 100 2 $1000 $1ff8 Data\frm*05d.del
rem DeltaCompression\Release\DeltaCompression.exe Data\frm*05d.sch $400 1 1 200 2 $1000 $1ff8 Data\frm*05d.del
rem DeltaCompression\Release\DeltaCompression.exe Data\frm*05d.sch $400 1 1 66 2 $1000 $1ff8 Data\frm*05d.del
rem DeltaCompression\Release\DeltaCompression.exe Data\frm*05d.sch 0 1 1 12 2 $1000 $1ff8 Data\frm*05d.del
rem DeltaCompression\Release\DeltaCompression.exe Data\frm*05d.sch $400 1 1 12 2 $1000 $1ff8 Data\frm*05d.del
rem DeltaCompression\Release\DeltaCompression.exe Data\frm*05d.sch $400 1 1 24 2 $1000 $1ff8 Data\frm*05d.del
rem DeltaCompression\Release\DeltaCompression.exe Data\frm*05d.sch $400 1 1 128 2 $1000 $1ff8 Data\frm*05d.del

rem DeltaCompression\Release\DeltaCompression.exe -savedictionary $400 Data\dictionary1.bin -savedictionary $400 Data\dictionary2.bin -fragment $0400 Data\frm*05d.scr -fragment $0800 Data\frm*05d.chr 1 6 7654 2 $1000 $1ff8 Data\frm*05d.del
rem del Data\*.del /q
rem DeltaCompression\Release\DeltaCompression.exe -optimisefile $0000 Data\dictionary1.bin -optimisefile $1000 Data\dictionary2.bin -fragment f$0000 Data\Dictionary1.bin -fragment f$1000 Data\Dictionary2.bin -fragment $0400 Data\frm*05d.scr -fragment $0800 Data\frm*05d.chr -maxframesize 16000 1 6 7654 2 $1000 $1ff8 Data\frm*05d.del
rem del Data\*.del /q
rem DeltaCompression\Release\DeltaCompression.exe -optimisefile $0000 Data\dictionary1.bin -optimisefile $1000 Data\dictionary2.bin -fragment f$0000 Data\Dictionary1.bin -fragment f$1000 Data\Dictionary2.bin -fragment $0400 Data\frm*05d.scr -fragment $0800 Data\frm*05d.chr -maxframesize 16000 1 6 7654 2 $1000 $1ff8 Data\frm*05d.del

echo Total
dir Data\frm0*.del | findstr " File(s)"
