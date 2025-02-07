@echo off
..\bin\LZMPi.exe -c Scroller.bin Scroller.cmp
..\bin\LZMPi.exe -ce Scroller.bin Scroller.cmp2
..\bin\LZMPi.exe -cr Scroller.bin Scroller.rle
..\bin\LZMPi.exe -cu Scroller.bin Scroller.cmp4
..\bin\LZMPi.exe -cv Scroller.bin Scroller.cmp5

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

call BuildDataFragment.bat c64v sC64DecompNoEffectRNZipV TestDecompression5V.a
call BuildDataFragment.bat c64bv sC64DecompBorderEffectRNZipV DefineDoBorderEffect.a TestDecompression5V.a

call BuildDataFragment.bat c64vh sC64DecompNoEffectRNZipVH DefineDoExtraWorkMemoryHigh.a TestDecompression5V.a
call BuildDataFragment.bat c64bvh sC64DecompBorderEffectRNZipVH DefineDoExtraWorkMemoryHigh.a DefineDoBorderEffect.a TestDecompression5V.a


echo // Processor port options of the above>>PrgData.cpp
call BuildDataFragment.bat c64pp sC64DecompNoEffectProcessorPort DefineDoProcessorPort.a TestDecompression2.a
call BuildDataFragment.bat c64bpp sC64DecompBorderEffectProcessorPort DefineDoProcessorPort.a DefineDoBorderEffect.a TestDecompression2.a

call BuildDataFragment.bat c64mpp sC64DecompNoEffectMaxProcessorPort DefineDoProcessorPort.a TestDecompression3.a
call BuildDataFragment.bat c64mbpp sC64DecompBorderEffectMaxProcessorPort DefineDoProcessorPort.a DefineDoBorderEffect.a TestDecompression3.a

call BuildDataFragment.bat c64epp sC64DecompNoEffectRNZipProcessorPort DefineDoProcessorPort.a DefineDecompress_RNZip.a TestDecompression2.a
call BuildDataFragment.bat c64epp sC64DecompBorderEffectRNZipProcessorPort DefineDoProcessorPort.a DefineDecompress_RNZip.a DefineDoBorderEffect.a TestDecompression2.a

call BuildDataFragment.bat c64mepp sC64DecompNoEffectMaxRNZipProcessorPort DefineDoProcessorPort.a DefineDecompress_RNZip.a TestDecompression3.a
call BuildDataFragment.bat c64mbepp sC64DecompBorderEffectMaxRNZipProcessorPort DefineDoProcessorPort.a DefineDecompress_RNZip.a DefineDoBorderEffect.a TestDecompression3.a

call BuildDataFragment.bat c64mrpp sC64DecompNoEffectMaxRLEProcessorPort DefineDoProcessorPort.a TestDecompression3RLE.a
call BuildDataFragment.bat c64mbrpp sC64DecompBorderEffectMaxRLEProcessorPort DefineDoProcessorPort.a DefineDoBorderEffect.a TestDecompression3RLE.a

call BuildDataFragment.bat c64mupp sC64DecompNoEffectMaxRNZipUProcessorPort DefineDoProcessorPort.a TestDecompression4U.a
call BuildDataFragment.bat c64mbupp sC64DecompBorderEffectMaxRNZipUProcessorPort DefineDoProcessorPort.a DefineDoBorderEffect.a TestDecompression4U.a

call BuildDataFragment.bat c64vpp sC64DecompNoEffectRNZipVProcessorPort DefineDoProcessorPort.a TestDecompression5V.a
call BuildDataFragment.bat c64bvpp sC64DecompBorderEffectRNZipVProcessorPort DefineDoProcessorPort.a DefineDoBorderEffect.a TestDecompression5V.a

call BuildDataFragment.bat c64vhpp sC64DecompNoEffectRNZipVHProcessorPort DefineDoProcessorPort.a DefineDoExtraWorkMemoryHigh.a TestDecompression5V.a
call BuildDataFragment.bat c64bvhpp sC64DecompBorderEffectRNZipVHProcessorPort DefineDoProcessorPort.a DefineDoExtraWorkMemoryHigh.a DefineDoBorderEffect.a TestDecompression5V.a
