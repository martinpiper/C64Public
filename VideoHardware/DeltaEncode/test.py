import unittest

import main


class MyTestCase(unittest.TestCase):
    def test_encode_decode(self):
        # With extra end padding data
        testdata = bytes([0x80, 0x81, 0x81, 0x81, 0x81, 0x90, 0x90, 0x90, 0x90, 0x70, 0x70, 0x70, 0xff, 0x00, 0x00,
                          0xff, 0x40, 0x40, 0x80, 0x80, 0xa0, 0xa0, 0xb0, 0xb0, 0xe0, 0x80, 0x80, 0x80, 0x80, 0x80,
                          0x80, 0x80, 0x80, 0x80, 0x80])
        testdataFile = open("C:\\Temp\\testdata.bin", "wb")
        testdataFile.write(testdata)
        testdataFile.close()

        argvIn = ["main", "-c", "C:\\Temp\\testdata.bin", "7", "C:\\Temp\\testdata.vcd"]
        main.main(argvIn)

        argvOut = ["main", "-d", "C:\\Temp\\testdata.vcd", "C:\\Temp\\testdata.decomp"]
        main.main(argvOut)

        # Expected compressed data check
        expectedCompressedData = [0xAE, 0x1E, 0xE0, 0x41, 0xC0, 0x23, 0xC0, 0x1F, 0xF8, 0x07, 0xF8, 0x02, 0xFF, 0x01,
                                  0x01, 0x02, 0x04, 0x10, 0x40, 0xC0, 0x03, 0x07]
        readBack = open(argvOut[2], "rb")
        compressedData = readBack.read()
        readBack.close()
        self.assertEqual(list(compressedData), list(expectedCompressedData))

        # Check the decompressed data results in at least the original test data
        # Due to the input data being a bit stream, the output data length detection can truncate the last few bytes
        readBack = open(argvOut[3], "rb")
        decompData = list(readBack.read())
        readBack.close()

        self.assertEqual(list(testdata[:len(decompData)]), decompData)


if __name__ == '__main__':
    unittest.main()
