from unittest import TestCase
import charpad


class Test(TestCase):
    def commonNoTilesPerCharChecks1(self):
        self.assertEqual(charpad.getBackgroundColour(), 0x00)
        self.assertEqual(charpad.getMulticolour1(), 0x0b)
        self.assertEqual(charpad.getMulticolour2(), 0x0c)
        self.assertEqual(charpad.getColouringMethod(), charpad.colouringMethodPerCharacter)
        self.assertEqual(charpad.getNumCharacters(), 256)
        self.assertEqual(charpad.getCharacters()[0:16], bytearray([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x50, 0x50, 0xa4, 0xa4, 0xa9, 0xa9]))
        self.assertEqual(sum(charpad.getCharacters()), 34184)
        self.assertEqual(charpad.getCharacterAttributes()[0:16], bytearray([0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9]))
        self.assertEqual(sum(charpad.getCharacterAttributes()), 228)

    def commonTilesPerCharChecks1(self):
        self.assertEqual(charpad.usingTiles(), True)
        self.assertEqual(charpad.getNumTiles(), 10)
        self.assertEqual(charpad.getTilesWidth(), 4)
        self.assertEqual(charpad.getTilesHeight(), 4)
        self.assertEqual(charpad.getTileData()[0:16], bytearray([2, 3, 3, 3, 4 ,0, 7, 8, 4, 6, 6, 6, 0, 0, 0, 0]))
        self.assertEqual(sum(charpad.getTileData()), 836)


    def commonTilesMapChecks1(self):
        self.assertEqual(charpad.usingTiles(), True)
        self.assertEqual(charpad.getMapWidth(), 20)
        self.assertEqual(charpad.getMapHeight(), 2)
        self.assertEqual(charpad.getMapData()[0:10], bytearray([0, 1, 2, 3, 4, 5, 6, 7, 8, 9]))
        self.assertEqual(sum(charpad.getMapData()), 45)


    def commonNoTilesMapChecks1(self):
        self.assertEqual(charpad.usingTiles(), False)
        self.assertEqual(charpad.getMapWidth(), 40)
        self.assertEqual(charpad.getMapHeight(), 5)
        self.assertEqual(charpad.getMapData()[0:10], bytearray([2, 3, 3, 3, 3, 3, 3, 3, 3, 3]))
        self.assertEqual(sum(charpad.getMapData()), 836)


# TODO: Need test for:
#        self.assertEqual(charpad.getGlobalColour(), 0x0c)

    def test_load_ctmfile_no_tiles_per_char_1(self):
        charpad.loadCTMFile("TestData/CharPad no tiles per char 2_0_310715.ctm")
        self.commonNoTilesPerCharChecks1()
        self.commonNoTilesMapChecks1()

    def test_load_ctmfile_no_tiles_per_char_2(self):
        charpad.loadCTMFile("TestData/CharPad no tiles per char 2_7_2.ctm")
        self.commonNoTilesPerCharChecks1()
        self.commonNoTilesMapChecks1()

    def test_load_ctmfile_4x4_tiles_per_char_1(self):
        charpad.loadCTMFile("TestData/CharPad 4x4 tiles per char 2_0_310715.ctm")
        self.commonNoTilesPerCharChecks1()
        self.commonTilesPerCharChecks1()
        self.commonTilesMapChecks1();

    def test_load_ctmfile_4x4_tiles_per_char_2(self):
        charpad.loadCTMFile("TestData/CharPad 4x4 tiles per char 2_7_2.ctm")
        self.commonNoTilesPerCharChecks1()
        self.commonTilesPerCharChecks1()
        self.commonTilesMapChecks1();

