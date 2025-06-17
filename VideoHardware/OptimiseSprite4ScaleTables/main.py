import sys
from glob import glob


if __name__ == '__main__':


    inputFileLines = open(sys.argv[1]).readlines()

    allFileData = ""
    for arg in sys.argv[3:]:
        files = glob(arg)
        for name in files:
            file = open(name,"r")
            allFileData += file.read()

    outputFile = open(sys.argv[2] , "w")

    removedMacros = 0
    saveMacro = False
    for line in inputFileLines:
        if line.find(';') != -1:
            outputFile.write(line)
            continue

        if line.find('+') != -1:
            if saveMacro:
                outputFile.write(line)
            else:
                outputFile.write(';'+line)
                removedMacros += 1
            saveMacro = False
            continue

        if allFileData.find(line.strip()) != -1:
            # The label is used somewhere in the source files
            saveMacro = True
            outputFile.write(line)
            continue

        outputFile.write(';'+line)
        continue

    print("removedMacros ", removedMacros)
