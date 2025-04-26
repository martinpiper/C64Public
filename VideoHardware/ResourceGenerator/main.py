import sys

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("<file to append to> <ebbs> <address if ebbs != 0> file <input file if ebbs != 0>")
        print("<file to append to> <ebbs> <address if ebbs != 0> bytes <byte>...")
        print("<file to append to> checksum <block size> <number of bytes> <bytes per checksum> <output label file>")
        exit(-1)

    if sys.argv[2] == "checksum":
        fileOut = open(sys.argv[1], "r+b")
        labelFile = open(sys.argv[6], "w")

        blockSize = int(sys.argv[3], 0)
        numBytes = int(sys.argv[4], 0)
        checksumSize = int(sys.argv[5], 0)

        fileOut.seek(0, 2)
        # Align the data to the numBytes block size so that the appended checksum data does not interfere with any
        # block checksum calculations
        while (fileOut.tell() % numBytes) != 0:
            fileOut.write(bytes([0]))

        fileLen = fileOut.tell()

        labelFile.write("checksumsStart = $" + hex(fileLen & 0xffffff)[2:] + "\n")
        labelFile.write("checksumsCount = $" + hex(int((fileLen / blockSize) + 1) & 0xffff)[2:] + "\n")
        labelFile.write("checksumsBlockSize = $" + hex(blockSize & 0xffffff)[2:] + "\n")
        labelFile.write("checksumsNumBytes = $" + hex(numBytes & 0xffffff)[2:] + "\n")
        labelFile.write("checksumsSize = $" + hex(checksumSize & 0xffffff)[2:] + "\n")
        labelFile.flush()
        labelFile.close()

        bytesToWrite = bytes()
        i = 0
        while i < fileLen:
            fileOut.seek(i, 0)
            thisChecksum = 0
            bytesToRead = numBytes
            while bytesToRead > 0:
                aByte = fileOut.read(1)
                thisChecksum = thisChecksum + (aByte[0] & 0xff)
                bytesToRead = bytesToRead - 1

            aChecksumSize = checksumSize
            while aChecksumSize > 0:
                bytesToWrite = bytesToWrite + bytes([thisChecksum & 0xff])
                thisChecksum = thisChecksum >> 8
                aChecksumSize = aChecksumSize - 1

            i = i + blockSize

        fileOut.seek(0, 2)
        fileOut.write(bytesToWrite)

        print("Checksum data size: " + str(len(bytesToWrite)))

        fileOut.flush()
        fileOut.close()

        exit(0)

    fileOut = open(sys.argv[1], "ab")
    ebbs = int(sys.argv[2], 0)

    if ebbs == 0:
        fileOut.write(bytes([0]))
        exit(0)

    if sys.argv[4] == "file":
        fileIn = open(sys.argv[5], "rb")
        inBytes = fileIn.read()
    elif sys.argv[4] == "bytes":
        inBytes = bytes()
        i = 5
        while i < len(sys.argv):
            theByte = int(sys.argv[i], 0)
            inBytes = inBytes + bytes([theByte & 0xff])
            i = i + 1
    else:
        print("Unknown option type: " + sys.argv[4])
        exit(-1)

    addr = int(sys.argv[3], 0)

    while len(inBytes) > 0:
        # DMA in chunks of up to this size
        thisChunk = inBytes[0:32768]
        fileOut.write(bytes([ebbs & 0xff]))
        fileOut.write(bytes([addr & 0xff, (addr >> 8) & 0xff]))
        dmaLen = 65535 - len(thisChunk)
        fileOut.write(bytes([dmaLen & 0xff, (dmaLen >> 8) & 0xff]))
        fileOut.write(thisChunk)

        # Advance through the data
        addr = addr + len(thisChunk)
        inBytes = inBytes[32768:]
