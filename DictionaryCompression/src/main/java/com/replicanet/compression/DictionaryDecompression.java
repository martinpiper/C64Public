package com.replicanet.compression;

import java.nio.ByteBuffer;
import java.util.Arrays;

public class DictionaryDecompression {
    int bitsCount;
    int currentBits;
    int currentOffset;

    byte[] input;
    byte[] output;

    boolean getFlag() {
        boolean ret = false;

        if (bitsCount == 8) {
            currentBits = input[currentOffset];
            currentOffset++;
            bitsCount = 0;
        }

        if ((currentBits & 0x80) == 0x80) {
            ret = true;
        }
        currentBits <<= 1;
        bitsCount++;

        return ret;
    }

    int getByte() {
        return ((int)input[currentOffset++]) & 0xff;
    }

    int getValue() {
        int ret = 0;
        boolean seqFlag;
        do {
            ret <<= 1;
            boolean theBit = getFlag();
            seqFlag = getFlag();
            if (theBit) {
                ret |= 0x01;
            }
        } while (seqFlag);
        return ret;
    }

    public byte[] decompress(byte[] inputData , byte[] dictionary) {
        input = inputData;

        bitsCount = 8;
        currentBits = 0;
        currentOffset = 0;

        output = new byte[1024*1024];
        int realSize = 0;

        boolean eof = false;
        while(!eof) {
            boolean flag = getFlag();
            if (flag) {
                // Offset
                int offset = getValue();
                if (offset >= ((1<<24)-1)) {
                    eof = true;
                    break;
                }
                offset <<= 8;
                offset |= getByte();

                int length = getValue() + 1;

                // Copy
                boolean fromDictionary = getFlag();

                if (fromDictionary) {
                    for (int i = 0 ; i < length ; i++) {
                        output[realSize + i] = dictionary[offset + i];
                    }
                } else {
                    for (int i = 0 ; i < length ; i++) {
                        output[realSize + i] = output[realSize - offset - 1 + i];
                    }
                }

                realSize += length;
            } else {
                // Literal
                int theByte = getByte();
                output[realSize] = (byte) theByte;
                realSize++;
            }
        }

        byte[] ret = new byte[realSize];
        System.arraycopy(output,0,ret,0,realSize);

        return ret;
    }
}
