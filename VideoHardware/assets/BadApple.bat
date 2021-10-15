rem Note the "--imagequantize 2" because the image has some greys

java -jar ..\..\..\ImageToBitplane\target\imagetobitplane-1.0-SNAPSHOT-jar-with-dependencies.jar --chars --rgbshift 4 4 4 --newpalettes --palettesize 8 --image "C:\temp\frm00117.bmp" --imagequantize 2 --tilewh 8 8 --outputplanes ../tmp/frm00117_ --outputscrcol ../tmp/frm00117.bin --nostacking --numbitplanes 4 --convertwritepass


..\..\bin\LZMPi.exe -cr ..\tmp\frm00117.bin ..\tmp\frm00117.bin
..\..\bin\LZMPi.exe -cr ..\tmp\frm00117_0.bin ..\tmp\frm00117_0.bin
..\..\bin\LZMPi.exe -cr ..\tmp\frm00117_1.bin ..\tmp\frm00117_1.bin
..\..\bin\LZMPi.exe -cr ..\tmp\frm00117_2.bin ..\tmp\frm00117_2.bin
..\..\bin\LZMPi.exe -cr ..\tmp\frm00117_3.bin ..\tmp\frm00117_3.bin
