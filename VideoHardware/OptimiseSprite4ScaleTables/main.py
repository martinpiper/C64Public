import sys
from glob import glob

allowedSizes = [False] * 256

if __name__ == '__main__':

    inputFileLines = open(sys.argv[1]).readlines()

    allFileData = ""
    for arg in sys.argv[4:]:
        files = glob(arg)
        for name in files:
            file = open(name, "r")
            allFileData += file.read()

    outputFile = open(sys.argv[2], "w")

    removedMacros = 0
    saveMacro = False
    for line in inputFileLines:
        if line.find(';') != -1:
            outputFile.write(line)
            continue

        if line.find('+') != -1:
            if saveMacro:
                outputFile.write(line)
                tokens = line.split()
                thisSize = int(tokens[1])
                allowedSizes[thisSize] = True
            else:
                outputFile.write(';' + line)
                removedMacros += 1
            saveMacro = False
            continue

        if allFileData.find(line.strip()) != -1:
            # The label is used somewhere in the source files
            saveMacro = True
            outputFile.write(line)
            continue

        outputFile.write(';' + line)
        continue

    print("removedMacros", removedMacros)

    i = 0
    numRemoved = 0
    numRemovedInARow = 0
    while (i < len(allowedSizes) - 1):
        if allowedSizes[i] and allowedSizes[i + 1]:
            # print("Removing size", i)
            allowedSizes[i] = False
            numRemoved += 1
            numRemovedInARow += 1
            if numRemovedInARow > 1:
                # Skip this and the next...
                # Don't remove too many in a row otherwise it rounds up too much and the sprites get too large
                i += 1
                numRemovedInARow = 0
        else:
            numRemovedInARow = 0
        i += 1

    print("numRemoved sizes", numRemoved)

    outputFileSizes = open(sys.argv[3], "w")
    i = 0
    while (i < len(allowedSizes)):
        if allowedSizes[i]:
            outputFileSizes.write(str(i) + "\n")
        i += 1
