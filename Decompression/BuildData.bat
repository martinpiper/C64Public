@echo off
..\bin\LZMPi.exe -c Scroller.bin Scroller.cmp
..\bin\LZMPi.exe -ce Scroller.bin Scroller.cmp2
..\bin\LZMPi.exe -cr Scroller.bin Scroller.rle

echo // Built automatically by BuildData.bat>PrgData.cpp
call BuildDataFragment.bat sC64DecompNoEffect TestDecompression2.a
call BuildDataFragment.bat sC64DecompBorderEffect DefineDoBorderEffect.a TestDecompression2.a

call BuildDataFragment.bat sC64DecompNoEffectMax TestDecompression3.a
call BuildDataFragment.bat sC64DecompBorderEffectMax DefineDoBorderEffect.a TestDecompression3.a

call BuildDataFragment.bat sC64DecompNoEffectRNZip DefineDecompress_RNZip.a TestDecompression2.a
call BuildDataFragment.bat sC64DecompBorderEffectRNZip DefineDecompress_RNZip.a DefineDoBorderEffect.a TestDecompression2.a

call BuildDataFragment.bat sC64DecompNoEffectMaxRNZip DefineDecompress_RNZip.a TestDecompression3.a
call BuildDataFragment.bat sC64DecompBorderEffectMaxRNZip DefineDecompress_RNZip.a DefineDoBorderEffect.a TestDecompression3.a

call BuildDataFragment.bat sC64DecompNoEffectMaxRLE TestDecompression3RLE.a
call BuildDataFragment.bat sC64DecompBorderEffectMaxRLE DefineDoBorderEffect.a TestDecompression3RLE.a
