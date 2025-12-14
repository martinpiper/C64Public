# Use variable coded delta encoding to compress PCM samples. More frequent smaller deltas are encoded to use shorter bit
# sequences.
# When using max delta bits of 7 this method is lossless. When using lower delta bits larger delta changes tend to get
# smoothed out into following samples, this tends to filter out loud higher frequencies first.
# This compression is quite simple to decompress, consisting of only a stream of bits from memory, and does not use
# complex mathematics or lookup tables. The goal being to be simple to implement in hardware.
import sys
import numpy as np
# pip install matplotlib
# For any x86/x64 install errors due to pip install matplotlib, try changing the target of MSVC before pip install:
# "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64_x86
import matplotlib.pyplot as plt

outputBits = []
numBitsDistribution = [0] * 8
bytesChanged = 0


# The idea here being that smaller delta values, particularly 0, are more common and use fewer bits compared to the less
# common larger delta values. Since delta values can be positive and negative the sign is stored and the absolute value
# of the delta is encoded.
def encodeDelta(previousValue, inValue):
    value = inValue
    if value == 0:
        outputBits.append(1)
        numBitsDistribution[0] += 1
        return 1

    # Set up the sign and normalise the value
    sign = 0
    if value < 0:
        value = -value
        sign = 1

    theBits = []
    testValue = value
    while testValue > 0:
        theBits.append(testValue & 0x01)
        testValue = testValue >> 1
        outputBits.append(0)

    theBits = theBits[::-1]
    assert theBits[0] == 1, \
        ("The most significant bit should always be one. It has a dual purpose to signal the end of the number of "
         "bits (after the zeros) and also the value to shift into the byte.")
    outputBits.extend(theBits)
    outputBits.append(sign)

    numBitsDistribution[len(theBits)] += 1

    return len(theBits)


if __name__ == '__main__':

    if len(sys.argv) < 5:
        print("Compress: -c <file name to compress> <max delta bits> <output file name>")
        print("Compress and display frequency details: -cv <file name to compress> <max delta bits> <output file "
              "name> <frequency in Hz>")
        exit(-1)

    if sys.argv[1].lower().startswith("-c"):
        maxDeltaBits = int(sys.argv[3])
        fileIn = open(sys.argv[2], "rb")
        inBytes = list(fileIn.read())

        print("input size byte", len(inBytes))

        if maxDeltaBits < 7:
            # Pre-process the file to minimise the delta size
            previousValue = 0x80
            i = 0
            while i < len(inBytes):
                newValue = (int(inBytes[i])) & 0xff
                delta = newValue - previousValue
                numOutputBits = encodeDelta(previousValue, delta)
                # Reduce the size of the delta to fit the bits requirement
                while numOutputBits > maxDeltaBits:
                    if delta > 0:
                        delta -= 1
                    else:
                        delta += 1
                    numOutputBits = encodeDelta(previousValue, delta)
                # Compute what the resultant sample value is using the maybe compressed delta...
                previousValue += delta
                # And update the source data so that the final compression does not use out of range deltas
                if inBytes[i] != previousValue:
                    bytesChanged += 1
                inBytes[i] = previousValue
                i += 1

            print("bytesChanged", bytesChanged)

        # Now encode the data for real
        outputBits = []
        numBitsDistribution = [0] * 8

        previousValue = 0x80
        signedData = []
        i = 0
        while i < len(inBytes):
            newValue = (int(inBytes[i])) & 0xff
            signedData.append(newValue - 0x80)
            delta = newValue - previousValue
#            print(delta)
            encodeDelta(previousValue, delta)
            previousValue = newValue
            i += 1

        fileOut = open(sys.argv[4], "wb")
        i = 0
        theByte = 0
        theByteBits = 0
        while i < len(outputBits):
            theByte <<= 1
            theByte |= outputBits[i]
            theByteBits += 1
            i += 1
            if theByteBits >= 8:
                fileOut.write(bytes([theByte]))
                theByteBits = 0
                theByte = 0

        # Ensure last byte is written if needed
        if theByteBits >= 8:
            fileOut.write(bytes([theByte]))

        fileOut.close()

        fileOut = open(sys.argv[4] + ".decomp", "wb")
        i = 0
        while i < len(inBytes):
            fileOut.write(bytes([inBytes[i]]))
            i += 1
        fileOut.close()

        totalBits = len(outputBits)
        print("totalBits", totalBits, " bytes", totalBits / 8)
        print("ratio", 100 * (totalBits / 8) / len(inBytes))
        print("numBitsDistribution", numBitsDistribution)

        if sys.argv[1].lower() == "-cv":
            fs = int(sys.argv[5])  # Sample frequency (Hz) of input data
            duration = len(signedData) / fs  # seconds
            t = np.linspace(0, duration, len(signedData), endpoint=False)

            # Apply FFT to the encoded data
            fft_result = np.fft.fft(signedData)
            freq = np.fft.fftfreq(t.shape[-1], d=1 / fs)

            print(np.abs(fft_result))

            plt.plot(freq, np.abs(fft_result))
            plt.title('FFT of input file')
            plt.xlabel('Frequency (Hz)')
            plt.ylabel('Amplitude')
            plt.show()
