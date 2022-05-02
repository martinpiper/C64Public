package com.replicanet.compression;

import com.replicanet.compression.DictionaryCompression;

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
        int bestCompressedSize = 0;

        System.out.println("Trying options...");
        // First pass
        bestCompressedSize = runCompressionPass(args, dictionarySize, outputFilename, skip , false);

        // Try options
        int totalCompressedSize = runCompressionPass(args, dictionarySize, outputFilename, skip , true);
        if (totalCompressedSize < bestCompressedSize) {
            bestCompressedSize = totalCompressedSize;
            bestOptimiseDictionary = true;
        }

        System.out.println("Final pass...");
        runCompressionPass(args, dictionarySize, outputFilename, skip , bestOptimiseDictionary);
    }

    private static int runCompressionPass(String[] args, int dictionarySize, String outputFilename, int skip , boolean optimiseDictionary) throws IOException {
        DictionaryCompression compression = new DictionaryCompression();
        compression.dictionaryInit(dictionarySize);

        int totalCompressedSize = 0;
        if (dictionarySize > 0) {
            for (int i = 3; i < args.length; i++) {
                File file = new File(args[i]);
                totalCompressedSize += compression.compressFile(args[i], outputFilename + "_" + file.getName() + ".cmp", skip);
            }
        }

        if (optimiseDictionary) {
            compression.optimiseDictionary();
        }

        totalCompressedSize = 0;
        for (int i = 3; i < args.length; i++) {
            File file = new File(args[i]);
            totalCompressedSize += compression.compressFile(args[i], outputFilename + "_" + file.getName() + ".cmp", skip);
        }

        compression.writeDictionary(outputFilename + ".dic");
        System.out.println("Total compressed size = " + totalCompressedSize);
        System.out.println("Dictionary size = " + compression.getDictionaryUsed());
        return totalCompressedSize;
    }
}
