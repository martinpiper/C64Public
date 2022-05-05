package com.replicanet.compression;

import java.io.File;
import java.io.IOException;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class Main {
    static boolean bestOptimiseDictionary = false;
    static boolean bestLargeThenSmall = false;
    static int bestCompressedSize = 0;
    static int bestBitsTweakCopy = 0;
    static int bestBitsTweakDictionary = 0;

    public static synchronized void updateDetails(
            boolean lbestOptimiseDictionary,
            boolean lbestLargeThenSmall,
            int lbestCompressedSize,
            int lbestBitsTweakCopy,
            int lbestBitsTweakDictionary
    ) {
        bestOptimiseDictionary = lbestOptimiseDictionary;
        bestLargeThenSmall = lbestLargeThenSmall;
        bestCompressedSize = lbestCompressedSize;
        bestBitsTweakCopy = lbestBitsTweakCopy;
        bestBitsTweakDictionary = lbestBitsTweakDictionary;
    }
    public static void main(String args[]) throws Exception {
        if (args.length < 4) {
            System.out.println("Usage: <dictionary size in bytes (decimal)> <output file name> <start offset in files (skip start bytes) (decimal)> [input files]");
            System.exit(-1);
        }

        int dictionarySize = Integer.parseInt(args[0]);
        String outputFilename = args[1];
        int skip = Integer.parseInt(args[2]);


        System.out.println("Trying compression options...");
        // First pass
        updateDetails(false , false , 0 , 0 , 0);
        bestCompressedSize = runCompressionPass(false, args, dictionarySize, outputFilename, skip , false , false , 0 , 0);

        // Try options
        ExecutorService es = Executors.newCachedThreadPool();
        for (int bitsTweakDictionary = 0 ; bitsTweakDictionary < 5 ; bitsTweakDictionary++) {
            for (int bitsTweakCopy = 0; bitsTweakCopy < 5; bitsTweakCopy++) {

                int finalBitsTweakCopy = bitsTweakCopy;
                int finalBitsTweakDictionary = bitsTweakDictionary;
                Runnable runnable = new Runnable() {
                    @Override
                    public void run() {
//                        System.out.println("Thread: " + Thread.currentThread().getId());
                        int totalCompressedSize = 0;
                        try {
                            totalCompressedSize = runCompressionPass(false, args, dictionarySize, outputFilename, skip, true, false, finalBitsTweakCopy, finalBitsTweakDictionary);
                            if (totalCompressedSize < bestCompressedSize) {
                                updateDetails(true , false , totalCompressedSize , finalBitsTweakCopy, finalBitsTweakDictionary);
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                };
                es.execute(runnable);


                runnable = new Runnable() {
                    @Override
                    public void run() {
//                        System.out.println("Thread: " + Thread.currentThread().getId());
                        int totalCompressedSize = 0;
                        try {
                            totalCompressedSize = runCompressionPass(false, args, dictionarySize, outputFilename, skip, true, true, finalBitsTweakCopy, finalBitsTweakDictionary);
                            if (totalCompressedSize < bestCompressedSize) {
                                updateDetails(true , true , totalCompressedSize , finalBitsTweakCopy, finalBitsTweakDictionary);
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                };
                es.execute(runnable);


                runnable = new Runnable() {
                    @Override
                    public void run() {
//                        System.out.println("Thread: " + Thread.currentThread().getId());
                        int totalCompressedSize = 0;
                        try {
                            totalCompressedSize = runCompressionPass(false, args, dictionarySize, outputFilename, skip, false, false, finalBitsTweakCopy, finalBitsTweakDictionary);
                            if (totalCompressedSize < bestCompressedSize) {
                                updateDetails(false , false , totalCompressedSize , finalBitsTweakCopy, finalBitsTweakDictionary);
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                };
                es.execute(runnable);
            }
        }

        System.out.println("Waiting for threads...");
        es.shutdown();
        es.awaitTermination(5 , TimeUnit.MINUTES);

        System.out.println("Final pass... " + bestOptimiseDictionary + " " + bestLargeThenSmall + " " + bestBitsTweakCopy + " " + bestBitsTweakDictionary);
        runCompressionPass(true, args, dictionarySize, outputFilename, skip , bestOptimiseDictionary , bestLargeThenSmall , bestBitsTweakCopy , bestBitsTweakDictionary);
    }

    private static int runCompressionPass(boolean save , String[] args, int dictionarySize, String outputFilename, int skip , boolean optimiseDictionary , boolean largeThenSmallDictionary , int bitsTweakCopy , int bitsTweakDictionary) throws IOException {
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
                totalCompressedSize += compression.compressFile(false, args[i], outputFilename + "_" + file.getName() + ".cmp", skip, bitsTweakCopy, bitsTweakDictionary);
            }
        }

        if (optimiseDictionary || largeThenSmallDictionary) {
            compression.optimiseDictionary(dictionarySize);
        }

        totalCompressedSize = 0;
        for (int i = 3; i < args.length; i++) {
            File file = new File(args[i]);
            totalCompressedSize += compression.compressFile(save, args[i], outputFilename + "_" + file.getName() + ".cmp", skip, bitsTweakCopy, bitsTweakDictionary);
        }

        if (save) {
            compression.writeDictionary(outputFilename + ".dic");
            System.out.println("Total compressed size = " + totalCompressedSize);
            System.out.println("Dictionary size = " + compression.getDictionaryUsed());
        }
        return totalCompressedSize;
    }
}
