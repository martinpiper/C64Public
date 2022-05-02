package com.replicanet.compression;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;

public class DictionaryCompression {
    byte dictionary[];
    int dictionaryUsage[];
    int dictionaryUID[];
    int dictionaryNewID;

    public int getDictionaryUsed() {
        return dictionaryUsed;
    }

    int dictionaryUsed = 0;

    byte currentRange[];
    int currentRangePos = 0;

    public int getOriginalSize() {
        return originalSize;
    }

    int originalSize;

    public void dictionaryInit(int size) {
        dictionary = new byte[size];
        dictionaryUsage = new int[size];
        dictionaryUID = new int[size];
        dictionaryUsed = 0;
        dictionaryNewID = 0;

        currentRange = new byte[size];
        currentRangePos = 0;

        outPos = 1;
    }

    int flagsPos;
    int flagsCount;

    byte out[];
    int outPos;

    void addLiteral(byte data) {
        writeBitFlag(false);

        writeByte(data);

        // Add current literals output so far for later dictionary consideration
        if (currentRangePos < currentRange.length) {
            currentRange[currentRangePos] = data;
            currentRangePos++;
        }
    }

    void writeByte(byte data) {
        out[outPos] = data;
        outPos++;
    }

    void flushCurrentRange() {
        if (currentRangePos == 0) {
            return;
        }

        // check the range exists in the dictionary
        int rangePos = 0;
        for (int pos = 0 ; pos < dictionaryUsed ; pos ++) {
            if (dictionary[pos] != currentRange[rangePos]) {
                if (rangePos > 0) {
                    rangePos = 0;
                    // Only rewind the dictionary position if we need to rescan the current range again
                    pos--;
                }
                continue;
            }
            rangePos++;

            if (rangePos == currentRangePos) {
                // Range in dictionary already
                currentRangePos = 0;
                return;
            }
        }

        // The whole range wasn't in the dictionary, so add it
        for (int pos = 0 ; pos < currentRangePos ; pos ++) {
            if (dictionaryUsed >= dictionary.length) {
                currentRangePos = 0;
                return;
            }
            dictionary[dictionaryUsed] = currentRange[pos];
            dictionaryUsage[dictionaryUsed] = 1;
            dictionaryUID[dictionaryUsed] = dictionaryNewID;
            dictionaryUsed++;
        }
        dictionaryNewID++;
        currentRangePos = 0;
    }

    void writeBitFlag(boolean flag) {
        flushCheck();

        out[flagsPos] = (byte) (out[flagsPos] << 1);
        if (flag) {
            out[flagsPos] |= 1;
        }
        flagsCount++;
    }

    void flushCheck() {
        if (flagsCount == 8) {
            flagsPos = outPos;
            flagsCount = 0;
            outPos++;
        }
    }

    int bitsForValue(int value) {
        assert(value >= 0);

        int bits = 0;
        while (value >= 2) {
            value >>= 1;
            bits+=2;
        }
        bits+=2;

        return bits;
    }

    void encodeForValue(int value) {
        assert(value >= 0);

        // Count the bits needed for this value...
        int bitMask = 1;
        while (value >= bitMask) {
            bitMask <<= 1;
        }
        bitMask >>= 1;

        while (bitMask > 1) {
            if ((value & bitMask) == bitMask) {
                writeBitFlag(true);
            } else {
                writeBitFlag(false);
            }
            bitMask >>= 1;
            writeBitFlag(true);
        }

        if ((value & 0x01) == 0x01) {
            writeBitFlag(true);
        } else {
            writeBitFlag(false);
        }
        writeBitFlag(false);
    }

    public int compressFile(String sourceFilename, String destinationFilename , int skipStart) throws IOException {
        Path inPath = Paths.get(sourceFilename);
        byte[] data = Files.readAllBytes(inPath);
        if (skipStart > 0) {
            data = Arrays.copyOfRange(data, skipStart, data.length);
        }

        byte[] newOut = compressData(data);

        Path outPath = Paths.get(destinationFilename);
        Files.write(outPath, newOut);

        return newOut.length;
    }

    public byte[] compressData(byte[] data) {
        originalSize = data.length;

        allocateOutput(data.length);

        for (int pos = 0 ; pos < data.length ; ) {
            // Search previous data for length match
            if (pos > 0) {
                int bestPos = -1;
                int bestSave = 0;
                int bestLen = 0;
                boolean isDictionary = false;

                // Check for behind copy case. Also handles forward RLE and forward repeating runs
                for (int searchPos = pos - 1 ; searchPos >= 0 ; searchPos--) {
                    for (int checkOffset = 0; checkOffset < data.length - pos ; checkOffset++) {
                        if (data[pos + checkOffset] == data[searchPos + checkOffset]) {
                            // Matching...
                            // Size of flag, plus dictionary flag, plus 8 bits for position, plus other bits for position and length
                            // Note: Length is zero based and the deltaPos is adjusted -1
                            int deltaPos = (pos - searchPos) - 1;
                            int thisLen = checkOffset + 1;
                            int thisBits = 2 + bitsForValue(deltaPos >> 8) + 8 + bitsForValue(thisLen - 1);
                            int thisSave = (thisLen * 9) - thisBits;
                            if (thisSave > bestSave) {
                                bestPos = deltaPos;
                                bestSave = thisSave;
                                bestLen = thisLen;
                            }
                            continue;
                        }
                        break;
                    }
                }

                // Dictionary copy check. Needs extra bit to flag for dictionary instead of copy
                for (int searchPos = 0 ; searchPos <= dictionaryUsed ; searchPos++) {
                    for (int checkOffset = 0; checkOffset < data.length - pos ; checkOffset++) {
                        // This avoids trying to copy too much from the dictionary
                        if (searchPos + checkOffset >= dictionaryUsed) {
                            break;
                        }
                        if (data[pos + checkOffset] == dictionary[searchPos + checkOffset]) {
                            // Matching...
                            // Size of flag, plus dictionary flag, plus 8 bits for position, plus other bits for position and length
                            // Note: Length is zero based and the deltaPos is adjusted -1
                            int deltaPos = searchPos;
                            int thisLen = checkOffset + 1;
                            int thisBits = 2 + bitsForValue(deltaPos >> 8) + 8 + bitsForValue(thisLen - 1);
                            int thisSave = (thisLen * 9) - thisBits;
                            if (thisSave > bestSave) {
                                bestPos = deltaPos;
                                bestSave = thisSave;
                                bestLen = thisLen;
                                isDictionary = true;
                            }
                            continue;
                        }
                        break;
                    }
                }

                if (bestPos != -1 && bestSave > 0) {
                    // Some saving compared to literals so encode it
                    flushCurrentRange();
                    writeBitFlag(true);
                    if (isDictionary) {
                        increaseDictionaryUsageForUID(dictionaryUID[bestPos]);  // This option marginally better with test data
//                        increaseDictionaryUsageForSpan(bestPos , bestPos + bestLen);
                    }
                    encodeForValue(bestPos>>8);
                    writeByte((byte)bestPos);
                    encodeForValue(bestLen - 1);
                    writeBitFlag(isDictionary);
                    pos += bestLen;
                    continue;
                }
            }

            // Otherwise add literal
            addLiteral(data[pos]);
            pos++;
        }

        outputEnd();

        flushCurrentRange();
        flushFlags();

        byte[] newOut = Arrays.copyOfRange(out, 0, outPos);
        return newOut;
    }

    private void increaseDictionaryUsageForUID(int thisUID) {
        // Increase the usage for the whole span, using the span UID
        for (int i = 0 ; i < dictionaryUsed ; i++) {
            if (dictionaryUID[i] == thisUID) {
                dictionaryUsage[i]++;
            }
        }
    }

    private void increaseDictionaryUsageForSpan(int start , int end) {
        // Increase the usage for the whole span, using the span UID
        for (int i = start ; i < end ; i++) {
            dictionaryUsage[i]++;
        }
    }

    private void outputEnd() {
        // Output a really long copy offset from the dictionary to signal the end of the file
        writeBitFlag(true);
        encodeForValue((1<<24)-1);
    }

    private void flushFlags() {
        // Flush any flags
        while (flagsCount < 8) {
            out[flagsPos] = (byte) (out[flagsPos] << 1);
            flagsCount++;
        }
    }

    void allocateOutput(int length) {
        assert(length > 0);
        out = new byte[16 + (length * 2)];    // Beyond worst case :D
        outPos = 1;
        flagsPos = 0;
        flagsCount = 0;
        currentRangePos = 0;
    }

    public void writeDictionary(String destinationFilename) throws IOException {
        Path outPath = Paths.get(destinationFilename);
        byte[] newOut = new byte[dictionaryUsed];
        System.arraycopy(dictionary,0,newOut,0,dictionaryUsed);
        Files.write(outPath, newOut);
    }

    public void optimiseDictionary() {
        int maxUsage = 0;

        for (int i = 0; i < dictionaryUsed ; i++) {
            if (dictionaryUsage[i] > maxUsage) {
                maxUsage = dictionaryUsage[i];
            }
        }

        byte newDictionary[] = new byte[dictionary.length];
        int newDictionaryUsage[] = new int[dictionaryUsage.length];
        int newDictionaryUID[] = new int[dictionaryUID.length];

        // Taking the most used entries, preserve the ordering and spans, but move them towards the start of the dictionary
        // This causes shorter offset codes for more frequently used sections
        int idx = 0;
        int thisUsage = maxUsage;
        while (thisUsage >= 0) {
            for (int i = 0; i < dictionaryUsed ; i++) {
                if (dictionaryUsage[i] == thisUsage) {
                    int thisUID = dictionaryUID[i];
                    for (int j = 0 ; j < dictionaryUsed ; j++) {
                        if (dictionaryUID[j] == thisUID) {
                            newDictionary[idx] = dictionary[j];
                            newDictionaryUsage[idx] = dictionaryUsage[j];
                            newDictionaryUID[idx] = dictionaryUID[j];
                            // Flag the old entry as not used, since it has been copied
                            dictionaryUsage[j] = -1;
                            dictionaryUID[j] = -1;
                            idx++;
                            i = j;
                        }
                    }
                }
            }
            thisUsage--;
        }

        assert(idx == dictionaryUsed);
        dictionary = newDictionary;
        dictionaryUsage = newDictionaryUsage;
        dictionaryUID = newDictionaryUID;
    }
}
