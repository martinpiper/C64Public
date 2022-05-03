package com.replicanet.compression;

import java.io.File;
import java.io.IOException;

public class Main {
    public static void main(String args[]) throws Exception {
        if (args.length < 4) {
            System.out.println("Usage: <dictionary size in bytes (decimal)> <output file name> <start offset in files (skip start bytes) (decimal)> [input files]");
            System.exit(-1);
        }

        int dictionarySize = Integer.parseInt(args[0]);
        String outputFilename = args[1];
        int skip = Integer.parseInt(args[2]);

        boolean bestOptimiseDictionary = false;
        boolean bestLargeThenSmall = false;
        int bestCompressedSize = 0;
        int bestBitsTweakCopy = 0;
        int bestBitsTweakDictionary = 0;

        System.out.println("Trying compression options...");
        // First pass
        bestCompressedSize = runCompressionPass(args, dictionarySize, outputFilename, skip , false , false , 0 , 0);

        // Try options
        for (int bitsTweakDictionary = 0 ; bitsTweakDictionary < 5 ; bitsTweakDictionary++) {
            for (int bitsTweakCopy = 0; bitsTweakCopy < 5; bitsTweakCopy++) {
                int totalCompressedSize = runCompressionPass(args, dictionarySize, outputFilename, skip, true, false, bitsTweakCopy, bitsTweakDictionary);
                if (totalCompressedSize < bestCompressedSize) {
                    bestCompressedSize = totalCompressedSize;
                    bestOptimiseDictionary = true;
                    bestLargeThenSmall = false;
                    bestBitsTweakCopy = bitsTweakCopy;
                    bestBitsTweakDictionary = bitsTweakDictionary;
                }

                totalCompressedSize = runCompressionPass(args, dictionarySize, outputFilename, skip, true, true, bitsTweakCopy, bitsTweakDictionary);
                if (totalCompressedSize < bestCompressedSize) {
                    bestCompressedSize = totalCompressedSize;
                    bestOptimiseDictionary = true;
                    bestLargeThenSmall = true;
                    bestBitsTweakCopy = bitsTweakCopy;
                    bestBitsTweakDictionary = bitsTweakDictionary;
                }

                totalCompressedSize = runCompressionPass(args, dictionarySize, outputFilename, skip, false, false, bitsTweakCopy, bitsTweakDictionary);
                if (totalCompressedSize < bestCompressedSize) {
                    bestCompressedSize = totalCompressedSize;
                    bestOptimiseDictionary = true;
                    bestLargeThenSmall = true;
                    bestBitsTweakCopy = bitsTweakCopy;
                    bestBitsTweakDictionary = bitsTweakDictionary;
                }
            }
        }

        System.out.println("Final pass... " + bestOptimiseDictionary + " " + bestLargeThenSmall + " " + bestBitsTweakCopy + " " + bestBitsTweakDictionary);
        runCompressionPass(args, dictionarySize, outputFilename, skip , bestOptimiseDictionary , bestLargeThenSmall , bestBitsTweakCopy , bestBitsTweakDictionary);
    }

    private static int runCompressionPass(String[] args, int dictionarySize, String outputFilename, int skip , boolean optimiseDictionary , boolean largeThenSmallDictionary , int bitsTweakCopy , int bitsTweakDictionary) throws IOException {
        DictionaryCompression compression = new DictionaryCompression();
        if (largeThenSmallDictionary) {
            compression.dictionaryInit(dictionarySize * 128);
        } else {
            compression.dictionaryInit(dictionarySize);
        }

        int totalCompressedSize = 0;
        if (dictionarySize > 0) {
            for (int i = 3; i < args.length; i++) {
                File file = new File(args[i]);
                totalCompressedSize += compression.compressFile(args[i], outputFilename + "_" + file.getName() + ".cmp", skip, bitsTweakCopy, bitsTweakDictionary);
            }
        }

        if (optimiseDictionary || largeThenSmallDictionary) {
            compression.optimiseDictionary(dictionarySize);
        }

        totalCompressedSize = 0;
        for (int i = 3; i < args.length; i++) {
            File file = new File(args[i]);
            totalCompressedSize += compression.compressFile(args[i], outputFilename + "_" + file.getName() + ".cmp", skip, bitsTweakCopy, bitsTweakDictionary);
        }

        compression.writeDictionary(outputFilename + ".dic");
        System.out.println("Total compressed size = " + totalCompressedSize);
        System.out.println("Dictionary size = " + compression.getDictionaryUsed());
        return totalCompressedSize;
    }
}
