# File -> Settings -> Project:Footy -> Project Structure -> Add Content Root: stdlib
# Or: set PYTHONPATH=c:\work\c64\stdlib
import charpad
# pip install numpy
import numpy as np


def padded_hex(i, l):
    given_int = i
    given_len = l

    hex_result = hex(given_int)[2:]  # remove '0x' from beginning of str
    num_hex_chars = len(hex_result)
    extra_zeros = '0' * (given_len - num_hex_chars)  # may not get used..

    return ('0x' + hex_result if num_hex_chars == given_len else
            '?' * given_len if num_hex_chars > given_len else
            '0x' + extra_zeros + hex_result if num_hex_chars < given_len else
            None)


def split_chunks(l, n):
    for i in range(0, len(l), n):
        yield l[i:i + n]


charpad.loadCTMFile("Pitch.ctm")

print("width", charpad.getMapWidth())
print("height", charpad.getMapHeight())

rows = list(split_chunks(charpad.getMapData(), charpad.getMapWidth()))

print("split rows", len(rows))

rowNum = 0


def writeArrayAsBy(thing, prefix="", suffix=""):
    first = True
    for theByte in thing:
        if (not first):
            outFile.write(" , ")
        first = False
        outFile.write(prefix + "$" + padded_hex(int(theByte) & 0xff, 2)[2:] + suffix)


def writeArrayAsWord(thing, prefix="", suffix=""):
    first = True
    for theByte in thing:
        if (not first):
            outFile.write(" , ")
        first = False
        outFile.write(prefix + "$" + padded_hex(int(theByte) & 0xffff, 4)[4:] + suffix)


for row in rows:
    #    print(row)
    revRow = row[::-1]
    currentByte = 0
    currentDrawAddress = []
    currentDrawCharacter = []
    currentClearAddress = []
    currentLengthOffset = []
    currentDraw = []
    hackRTS = []
    hackRTS.append(0)
    lastHackRTSPos = 0
    currentDraw.append(0x60)  # rts

    currentClear = []
    hackRTSClear = []
    hackRTSClear.append(0)
    lastHackRTSClearPos = 0
    currentClear.append(0x60)  # rts

    zeroRun = 0
    rightPosOffset = 0
    for byte in revRow:
        rightPosOffset = rightPosOffset + 1
        if byte == 0:
            currentDrawAddress.append(-1)
            currentClearAddress.append(-1)
            currentDrawCharacter.append(currentByte)
            currentLengthOffset.append(zeroRun)
            zeroRun = zeroRun + 1
        else:
            # If the generated code is too large, then insert a fake rts, which uses bit $60, so the bmi can reach
            if len(currentDraw) > lastHackRTSPos + 230:
                lastHackRTSPos = len(currentDraw)
                currentDraw.append(0x24)  # bit zp
                hackRTS.append(len(currentDraw))
                currentDraw.append(0x60)  # pretend rts

            if len(currentClear) > lastHackRTSClearPos + 230:
                lastHackRTSClearPos = len(currentClear)
                currentClear.append(0x24)  # bit zp
                hackRTSClear.append(len(currentClear))
                currentClear.append(0x60)  # pretend rts

            # Reverse the offsets when the screen X position adjustment is skipped with certain X pos offsets
            i = len(currentLengthOffset) - 1
            while i >= 0:
                if currentDrawAddress[i] == -1:
                    currentLengthOffset[i] = zeroRun - currentLengthOffset[i]
                else:
                    break
                i = i - 1

            # Output any needed screen X position skips
            if currentByte != 0:
                if zeroRun >= 4:
                    currentDraw.append(0x98)  # tya
                    currentDraw.append(0xe9)  # sbc
                    currentDraw.append(zeroRun)
                    if rightPosOffset > 30:
                        currentDraw.append(0x30)  # bmi
                        currentDraw.append(-1234)  # To calculate
                    currentDraw.append(0xa8)  # tay

                    currentClear.append(0x98)  # tya
                    currentClear.append(0xe9)  # sbc
                    currentClear.append(zeroRun)
                    if rightPosOffset > 30:
                        currentClear.append(0x30)  # bmi
                        currentClear.append(-1234)  # To calculate
                    currentClear.append(0xa8)  # tay
                    currentClear.append(0xa9)  # lda #
                    currentClear.append(0x00)  # Always 0

                    currentByte = None  # Indeterminate
                elif zeroRun > 0:
                    while zeroRun > 0:
                        currentDraw.append(0x88)  # dey
                        currentClear.append(0x88)  # dey
                        zeroRun = zeroRun - 1
                    if rightPosOffset > 30:
                        currentDraw.append(0x30)  # bmi
                        currentDraw.append(-1234)  # To calculate
                        currentClear.append(0x30)  # bmi
                        currentClear.append(-1234)  # To calculate

            zeroRun = 0

            # Output the byte data
            if currentByte != byte:
                currentByte = byte
                currentDraw.append(0xa9)  # lda #
                currentDraw.append(byte)

            currentDrawAddress.append(len(currentDraw))
            currentClearAddress.append(len(currentClear))
            currentDrawCharacter.append(currentByte)
            currentLengthOffset.append(0)

            currentDraw.append(0x91)  # sta (zp),y
            currentDraw.append(0x02)

            currentDraw.append(0x88)  # dey
            if rightPosOffset > 30:
                currentDraw.append(0x30)  # bmi
                currentDraw.append(-1234)  # To calculate

            currentClear.append(0x91)  # sta (zp),y
            currentClear.append(0x02)

            currentClear.append(0x88)  # dey
            if rightPosOffset > 30:
                currentClear.append(0x30)  # bmi
                currentClear.append(-1234)  # To calculate

    hackRTS.append(len(currentDraw))
    currentDraw.append(0x60)  # rts
    hackRTSClear.append(len(currentClear))
    currentClear.append(0x60)  # rts

    # Calculate any branch offsets to the nearest branch
    i = 0
    while i < len(currentDraw):
        if currentDraw[i] == -1234:
            # Try to find the best offset
            bestOffset = -1234

            for anRTSPos in hackRTS:
                if anRTSPos < i and i - anRTSPos <= 127:
                    # Behind
                    bestOffset = 255 - (i - anRTSPos)
                    break
                elif anRTSPos > i and anRTSPos - i <= 127:
                    # Ahead
                    bestOffset = (anRTSPos - i) - 1
                    break

            currentDraw[i] = bestOffset
        i = i + 1

    i = 0
    while i < len(currentClear):
        if currentClear[i] == -1234:
            # Try to find the best offset
            bestOffset = -1234

            for anRTSPos in hackRTSClear:
                if anRTSPos < i and i - anRTSPos <= 127:
                    # Behind
                    bestOffset = 255 - (i - anRTSPos)
                    break
                elif anRTSPos > i and anRTSPos - i <= 127:
                    # Ahead
                    bestOffset = (anRTSPos - i) - 1
                    break

            currentClear[i] = bestOffset
        i = i + 1

    # Reverse the final tables and adjust any remaining values
    currentDrawAddress = currentDrawAddress[::-1]
    currentClearAddress = currentClearAddress[::-1]
    currentDrawCharacter = currentDrawCharacter[::-1]
    currentLengthOffset = currentLengthOffset[::-1]
    i = 0
    lastBest = 0
    lastBestClear = 0
    lastBestChar = 0
    while i < len(currentDrawAddress):
        if currentDrawAddress[i] == -1:
            currentDrawAddress[i] = lastBest
            currentClearAddress[i] = lastBestClear
            currentDrawCharacter[i] = lastBestChar
        else:
            lastBest = currentDrawAddress[i]
            lastBestClear = currentClearAddress[i]
            lastBestChar = currentDrawCharacter[i]
        i = i + 1
    currentDrawAddress = currentDrawAddress[-(len(row) - 39):]
    currentClearAddress = currentClearAddress[-(len(row) - 39):]
    currentDrawCharacter = currentDrawCharacter[-(len(row) - 39):]
    currentLengthOffset = currentLengthOffset[-(len(row) - 39):]

    partName = str(rowNum).rjust(2, '0')

    outFile = open("bin/row" + partName + "Draw.prg", "wb")
    outFile.write(bytearray([0]))
    outFile.write(bytearray([0x80]))
    outFile.write(bytearray(currentDraw))
    outFile.close()

    outFile = open("bin/row" + partName + "Clear.prg", "wb")
    outFile.write(bytearray([0]))
    outFile.write(bytearray([0x80]))
    outFile.write(bytearray(currentClear))
    outFile.close()

    outFile = open("bin/row" + partName + "DrawTable.prg", "wb")
    outFile.write(bytearray([0]))
    outFile.write(bytearray([0xc0]))
    for theByte in currentDrawAddress:
        outFile.write(bytearray([theByte & 0xff]))
    for theByte in currentDrawAddress:
        outFile.write(bytearray([(theByte >> 8) & 0xff]))
    for theByte in currentClearAddress:
        outFile.write(bytearray([theByte & 0xff]))
    for theByte in currentClearAddress:
        outFile.write(bytearray([(theByte >> 8) & 0xff]))
    for theByte in currentDrawCharacter:
        outFile.write(bytearray([theByte]))
    for theByte in currentLengthOffset:
        outFile.write(bytearray([theByte]))
    outFile.close()

    outFile = open("bin/row" + partName + "DrawTable.a", "w")
    outFile.write("row" + partName + "AddressLo\t\t!by ")
    writeArrayAsWord(currentDrawAddress, "<(RowDraw" + partName + " + ", ")")
    outFile.write("\n")
    outFile.write("row" + partName + "AddressHi\t\t!by ")
    writeArrayAsWord(currentDrawAddress, ">(RowDraw" + partName + " + ", ")")
    outFile.write("\n")
    outFile.write("row" + partName + "AddressClearLo\t!by ")
    writeArrayAsWord(currentClearAddress, "<(RowClear" + partName + " + ", ")")
    outFile.write("\n")
    outFile.write("row" + partName + "AddressClearHi\t!by ")
    writeArrayAsWord(currentClearAddress, ">(RowClear" + partName + " + ", ")")
    outFile.write("\n")
    outFile.write("row" + partName + "DrawCharacter\t!by ")
    writeArrayAsBy(currentDrawCharacter)
    outFile.write("\n")
    outFile.write("row" + partName + "LengthOffset\t!by ")
    writeArrayAsBy(currentLengthOffset, "39 - " )
    outFile.write("\n")
    outFile.close()

    print("row", rowNum, "length", len(currentDraw))

    rowNum = rowNum + 1
