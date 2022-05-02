package com.replicanet.compression;

import org.junit.Test;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;



import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;

public class DictionaryCompressionTest {
    @Test
    public void dictionaryInit() {
        DictionaryCompression compression = new DictionaryCompression();
        compression.dictionaryInit(1024);
        assertThat(compression.dictionary.length, is(equalTo(1024)));
        assertThat(compression.dictionaryUsage.length, is(equalTo(1024)));
        assertThat(compression.dictionaryUsed, is(equalTo(0)));
    }

    @Test
    public void writeByte1() {
        DictionaryCompression compression = new DictionaryCompression();
        compression.dictionaryInit(1024);
        compression.allocateOutput(256);

        assertThat(compression.outPos, is(equalTo(1)));
        assertThat(compression.flagsPos, is(equalTo(0)));
        assertThat(compression.out[0], is(equalTo((byte)0x00)));

        compression.writeByte((byte) 0x01);

        assertThat(compression.outPos, is(equalTo(2)));
        assertThat(compression.out[0], is(equalTo((byte)0x00)));
        assertThat(compression.out[1], is(equalTo((byte)0x01)));

        assertThat(compression.dictionaryUsed, is(equalTo(0)));
    }

    public void writeByte2() {
        DictionaryCompression compression = new DictionaryCompression();
        compression.dictionaryInit(1024);
        compression.allocateOutput(256);

        assertThat(compression.outPos, is(equalTo(1)));
        assertThat(compression.flagsPos, is(equalTo(0)));
        assertThat(compression.out[0], is(equalTo(0x00)));

        compression.writeByte((byte) 0x02);

        assertThat(compression.outPos, is(equalTo(2)));
        assertThat(compression.out[0], is(equalTo(0x00)));
        assertThat(compression.out[1], is(equalTo(0x02)));

        assertThat(compression.dictionaryUsed, is(equalTo(0)));
    }

    @Test
    public void writeBitFlag1() {
        DictionaryCompression compression = new DictionaryCompression();
        compression.dictionaryInit(1024);
        compression.allocateOutput(256);

        assertThat(compression.outPos, is(equalTo(1)));
        assertThat(compression.flagsPos, is(equalTo(0)));
        assertThat(compression.flagsCount, is(equalTo(0)));
        assertThat(compression.out[0], is(equalTo((byte)0x00)));

        compression.writeBitFlag(true);

        assertThat(compression.outPos, is(equalTo(1)));
        assertThat(compression.flagsPos, is(equalTo(0)));
        assertThat(compression.flagsCount, is(equalTo(1)));
        assertThat(compression.out[0], is(equalTo((byte)0x01)));
    }

    @Test
    public void writeBitFlag2() {
        DictionaryCompression compression = new DictionaryCompression();
        compression.dictionaryInit(1024);
        compression.allocateOutput(256);

        assertThat(compression.outPos, is(equalTo(1)));
        assertThat(compression.flagsPos, is(equalTo(0)));
        assertThat(compression.flagsCount, is(equalTo(0)));
        assertThat(compression.out[0], is(equalTo((byte)0x00)));

        compression.writeBitFlag(true);
        compression.writeBitFlag(false);

        compression.writeBitFlag(true);
        compression.writeBitFlag(false);

        compression.writeBitFlag(true);
        compression.writeBitFlag(false);

        compression.writeBitFlag(true);
        compression.writeBitFlag(false);

        compression.writeBitFlag(true);
        compression.writeBitFlag(false);

        assertThat(compression.outPos, is(equalTo(2)));
        assertThat(compression.flagsPos, is(equalTo(0x01)));
        assertThat(compression.flagsCount, is(equalTo(2)));
        assertThat(compression.out[0], is(equalTo((byte)0xaa)));
        assertThat(compression.out[1], is(equalTo((byte)0x02)));
    }

    @Test
    public void addLiteral() {
        DictionaryCompression compression = new DictionaryCompression();
        compression.dictionaryInit(1024);
        compression.allocateOutput(256);

        assertThat(compression.outPos, is(equalTo(1)));
        assertThat(compression.flagsPos, is(equalTo(0)));
        assertThat(compression.flagsCount, is(equalTo(0)));
        assertThat(compression.out[0], is(equalTo((byte)0x00)));

        compression.addLiteral((byte)0x13);

        assertThat(compression.outPos, is(equalTo(2)));
        assertThat(compression.flagsPos, is(equalTo(0)));
        assertThat(compression.flagsCount, is(equalTo(1)));
        assertThat(compression.out[0], is(equalTo((byte)0x00)));
        assertThat(compression.out[1], is(equalTo((byte)0x13)));
    }

    @Test
    public void bitsForValue1() {
        DictionaryCompression compression = new DictionaryCompression();
        assertThat(compression.bitsForValue(1), is(equalTo(2)));
    }

    @Test
    public void bitsForValue2() {
        DictionaryCompression compression = new DictionaryCompression();
        assertThat(compression.bitsForValue(2), is(equalTo(4)));
    }

    @Test
    public void bitsForValue3() {
        DictionaryCompression compression = new DictionaryCompression();
        assertThat(compression.bitsForValue(3), is(equalTo(4)));
    }

    @Test
    public void encodeForValue1() {
        DictionaryCompression compression = new DictionaryCompression();
        compression.allocateOutput(256);

        compression.encodeForValue(0x00);

        assertThat(compression.outPos, is(equalTo(1)));
        assertThat(compression.flagsPos, is(equalTo(0)));
        assertThat(compression.flagsCount, is(equalTo(2)));
        assertThat(compression.out[0], is(equalTo((byte)0x00)));
    }

    @Test
    public void encodeForValue2() {
        DictionaryCompression compression = new DictionaryCompression();
        compression.allocateOutput(256);

        compression.encodeForValue(0x01);

        assertThat(compression.outPos, is(equalTo(1)));
        assertThat(compression.flagsPos, is(equalTo(0)));
        assertThat(compression.flagsCount, is(equalTo(2)));
        assertThat(compression.out[0], is(equalTo((byte)0x02)));
    }

    @Test
    public void encodeForValue3() {
        DictionaryCompression compression = new DictionaryCompression();
        compression.allocateOutput(256);

        compression.encodeForValue(0x07);

        assertThat(compression.outPos, is(equalTo(1)));
        assertThat(compression.flagsPos, is(equalTo(0)));
        assertThat(compression.flagsCount, is(equalTo(6)));
        assertThat(compression.out[0], is(equalTo((byte)0x3e)));
    }

    @Test
    public void flushCurrentRange() {
        DictionaryCompression compression = new DictionaryCompression();
        compression.dictionaryInit(1024);
        compression.allocateOutput(256);

        assertThat(compression.currentRangePos, is(equalTo(0)));
        assertThat(compression.dictionaryUsed, is(equalTo(0)));

        compression.addLiteral((byte)0x01);
        compression.addLiteral((byte)0x02);
        compression.addLiteral((byte)0x03);
        compression.addLiteral((byte)0x04);
        compression.addLiteral((byte)0x05);

        assertThat(compression.currentRangePos, is(equalTo(5)));
        assertThat(compression.dictionaryUsed, is(equalTo(0)));

        compression.flushCurrentRange();

        assertThat(compression.currentRangePos, is(equalTo(0)));
        assertThat(compression.dictionaryUsed, is(equalTo(5)));

        assertThat(compression.dictionary[0], is(equalTo((byte)0x01)));
        assertThat(compression.dictionary[1], is(equalTo((byte)0x02)));
        assertThat(compression.dictionary[2], is(equalTo((byte)0x03)));
        assertThat(compression.dictionary[3], is(equalTo((byte)0x04)));
        assertThat(compression.dictionary[4], is(equalTo((byte)0x05)));

        assertThat(compression.dictionaryUsage[0], is(equalTo(1)));
        assertThat(compression.dictionaryUsage[1], is(equalTo(1)));
        assertThat(compression.dictionaryUsage[2], is(equalTo(1)));
        assertThat(compression.dictionaryUsage[3], is(equalTo(1)));
        assertThat(compression.dictionaryUsage[4], is(equalTo(1)));
    }

    @Test
    public void compressData1() throws IOException {
        byte[] inputData = new byte[]{0x00 , 0x01 , 0x02 , 0x03};
        byte[] expected = new byte[]{0x0f , 0x00 , 0x01 , 0x02 , 0x03 , (byte)0xff, (byte)0xff, (byte)0xff, (byte)0xff, (byte)0xff, (byte)0xf0};
        byte[] expected2 = new byte[]{0x4c , 0x00 , 0x01 , (byte)0xff , (byte)0xff, (byte)0xff, (byte)0xff, (byte)0xff, (byte)0xff, (byte)0x80};
        byte outputData[] = commonDataTest(inputData , expected , expected2);
    }

    @Test
    public void compressData2() throws IOException {
        byte[] inputData = new byte[]{0x77 , 0x77 , 0x77 , 0x77};
        byte[] expected = new byte[]{0x4c , 0x77 , 0x00 , 0x7f , (byte)0xff , (byte)0xff, (byte)0xff , (byte)0xff, (byte)0xff, (byte) 0x80};
        byte[] expected2 = new byte[]{0x4c , 0x77 , 0x00 , 0x7f , (byte)0xff , (byte)0xff, (byte)0xff , (byte)0xff, (byte)0xff, (byte) 0x80};
        byte outputData[] = commonDataTest(inputData , expected , expected2);
    }

    @Test
    public void compressData3() throws IOException {
        byte[] inputData = new byte[]{0x01 , 0x77 , 0x77 , 0x77 , 0x77 , 0x77 , 0x77 , 0x77 , 0x77 , 0x77 , 0x77 , 0x77 , 0x77 , 0x02};
        byte[] expected = new byte[]{0x26 , 0x01 , 0x77 , 0x00 , (byte)0xe1 , 0x02 , (byte) 0xff, (byte) 0xff , (byte) 0xff , (byte) 0xff , (byte) 0xff , (byte) 0xfe};
        byte[] expected2 = new byte[]{0x26 , 0x01 , 0x77 , 0x00 , (byte)0xe1 , 0x02 , (byte) 0xff, (byte) 0xff , (byte) 0xff , (byte) 0xff , (byte) 0xff , (byte) 0xfe};
        byte outputData[] = commonDataTest(inputData , expected , expected2);
    }

    @Test
    public void optimiseDictionary() {
        DictionaryCompression compression = new DictionaryCompression();
        compression.dictionaryInit(1024);

        assertThat(compression.dictionaryUsed, is(equalTo(0)));

        compression.dictionary[0] = 0x11;
        compression.dictionaryUsage[0] = 1;
        compression.dictionaryUID[0] = 1;
        compression.dictionary[1] = 0x12;
        compression.dictionaryUsage[1] = 1;
        compression.dictionaryUID[1] = 1;
        compression.dictionary[2] = 0x13;
        compression.dictionaryUsage[2] = 3;
        compression.dictionaryUID[2] = 2;
        compression.dictionary[3] = 0x14;
        compression.dictionaryUsage[3] = 4;
        compression.dictionaryUID[3] = 3;
        compression.dictionary[4] = 0x15;
        compression.dictionaryUsage[4] = 5;
        compression.dictionaryUID[4] = 3;

        compression.dictionaryUsed = 5;

        compression.optimiseDictionary();

        assertThat(compression.dictionary[0], is(equalTo((byte)0x14)));
        assertThat(compression.dictionaryUsage[0], is(equalTo(4)));
        assertThat(compression.dictionary[1], is(equalTo((byte)0x15)));
        assertThat(compression.dictionaryUsage[1], is(equalTo(5)));
        assertThat(compression.dictionary[2], is(equalTo((byte)0x13)));
        assertThat(compression.dictionaryUsage[2], is(equalTo(3)));
        assertThat(compression.dictionary[3], is(equalTo((byte)0x11)));
        assertThat(compression.dictionaryUsage[3], is(equalTo(1)));
        assertThat(compression.dictionary[4], is(equalTo((byte)0x12)));
        assertThat(compression.dictionaryUsage[4], is(equalTo(1)));
    }

    private byte[] commonDataTest(byte[] inputData, byte[] expected, byte[] expected2) throws IOException {
        DictionaryCompression compression = new DictionaryCompression();
        compression.dictionaryInit(1024);

        org.apache.commons.io.HexDump.dump(inputData , 0, System.out ,0);
        byte outputData[] = compression.compressData(inputData);
        org.apache.commons.io.HexDump.dump(outputData , 0, System.out ,0);
        reportCompression(compression, outputData.length);

        assertThat(outputData,is(equalTo(expected)));

        byte outputData2[] = compression.compressData(inputData);
        org.apache.commons.io.HexDump.dump(outputData2 , 0, System.out ,0);
        reportCompression(compression, outputData2.length);

        assertThat(outputData2,is(equalTo(expected2)));

        DictionaryDecompression decompression = new DictionaryDecompression();
        byte[] decompData = decompression.decompress(outputData,compression.dictionary);
        assertThat(inputData,is(equalTo(decompData)));

        byte[] decompData2 = decompression.decompress(outputData2,compression.dictionary);
        assertThat(inputData,is(equalTo(decompData2)));

        return outputData;
    }

    private void commonFileCompressionTest(DictionaryCompression compression, String sourceFilename, String outputFile) throws IOException {
        int compressedSize = compression.compressFile(sourceFilename, outputFile, 2);
        reportCompression(compression, compressedSize);

        Path inPath = Paths.get(sourceFilename);
        byte[] inputData = Files.readAllBytes(inPath);
        inputData = Arrays.copyOfRange(inputData, 2, inputData.length);

        Path outPath = Paths.get(outputFile);
        byte[] outputData = Files.readAllBytes(outPath);

        DictionaryDecompression decompression = new DictionaryDecompression();
        byte[] decompData = decompression.decompress(outputData,compression.dictionary);

        assertThat(inputData,is(equalTo(decompData)));
    }

    int totalCompressedSize = 0;

    @Test
    public void compressFile() throws IOException {
        totalCompressedSize = 0;
        DictionaryCompression compression = new DictionaryCompression();
        compression.dictionaryInit(1024);

        if (true) {
//            compression.dictionaryInit(1024);
            commonFileCompressionTest(compression,"TestData/Dinosaur_Disco.prg", "target/Dinosaur_Disco.cmp");
        }


        if (true) {
//            compression.dictionaryInit(1024);
            commonFileCompressionTest(compression,"TestData/Lobster.prg", "target/Lobster.cmp");
        }


        if (true) {
//            compression.dictionaryInit(1024);
            commonFileCompressionTest(compression,"TestData/MeanStreak.prg", "target/MeanStreak.cmp");
        }


        if (true) {
//            compression.dictionaryInit(1024);
            commonFileCompressionTest(compression,"TestData/Mule.prg", "target/Mule.cmp");
        }

        System.out.println("** With dictionary");

        if (true) {
            commonFileCompressionTest(compression,"TestData/Dinosaur_Disco.prg", "target/Dinosaur_Disco.cmp2");
        }


        if (true) {
            commonFileCompressionTest(compression,"TestData/Lobster.prg", "target/Lobster.cmp2");
        }


        if (true) {
            commonFileCompressionTest(compression,"TestData/MeanStreak.prg", "target/MeanStreak.cmp2");
        }


        if (true) {
            commonFileCompressionTest(compression,"TestData/Mule.prg", "target/Mule.cmp2");
        }

        System.out.println("** optimiseDictionary: Start");
        compression.optimiseDictionary();
        System.out.println("** optimiseDictionary: End");

        if (true) {
            commonFileCompressionTest(compression,"TestData/Dinosaur_Disco.prg", "target/Dinosaur_Disco.cmp3");
        }


        if (true) {
            commonFileCompressionTest(compression,"TestData/Lobster.prg", "target/Lobster.cmp3");
        }


        if (true) {
            commonFileCompressionTest(compression,"TestData/MeanStreak.prg", "target/MeanStreak.cmp3");
        }


        if (true) {
            commonFileCompressionTest(compression,"TestData/Mule.prg", "target/Mule.cmp3");
        }

        compression.writeDictionary("target/dictionary.bin");

        System.out.println("totalCompressedSize: " + totalCompressedSize);
    }

    private void reportCompression(DictionaryCompression compression, int compressedSize) {
        System.out.println("Original size: " + compression.getOriginalSize());
        System.out.println("Compressed size: " + compressedSize);
        System.out.println("Dictionary used: " + compression.getDictionaryUsed());

        totalCompressedSize += compressedSize;
    }
}
