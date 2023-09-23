@echo off
..\bin\LZMPi.exe -c Scroller.bin Scroller.cmp
..\bin\LZMPi.exe -ce Scroller.bin Scroller.cmp2
..\bin\LZMPi.exe -cr Scroller.bin Scroller.rle
..\bin\LZMPi.exe -cu Scroller.bin Scroller.cmp4

mkdir DebugFiles

echo // Built automatically by BuildData.bat>PrgData.cpp
call BuildDataFragment.bat c64 sC64DecompNoEffect TestDecompression2.a
call BuildDataFragment.bat c64b sC64DecompBorderEffect DefineDoBorderEffect.a TestDecompression2.a

call BuildDataFragment.bat c64m sC64DecompNoEffectMax TestDecompression3.a
call BuildDataFragment.bat c64mb sC64DecompBorderEffectMax DefineDoBorderEffect.a TestDecompression3.a

call BuildDataFragment.bat c64e sC64DecompNoEffectRNZip DefineDecompress_RNZip.a TestDecompression2.a
call BuildDataFragment.bat c64e sC64DecompBorderEffectRNZip DefineDecompress_RNZip.a DefineDoBorderEffect.a TestDecompression2.a

call BuildDataFragment.bat c64me sC64DecompNoEffectMaxRNZip DefineDecompress_RNZip.a TestDecompression3.a
call BuildDataFragment.bat c64mbe sC64DecompBorderEffectMaxRNZip DefineDecompress_RNZip.a DefineDoBorderEffect.a TestDecompression3.a

call BuildDataFragment.bat c64mr sC64DecompNoEffectMaxRLE TestDecompression3RLE.a
call BuildDataFragment.bat c64mbr sC64DecompBorderEffectMaxRLE DefineDoBorderEffect.a TestDecompression3RLE.a

call BuildDataFragment.bat c64mu sC64DecompNoEffectMaxRNZipU TestDecompression4U.a
call BuildDataFragment.bat c64mbu sC64DecompBorderEffectMaxRNZipU DefineDoBorderEffect.a TestDecompression4U.a
