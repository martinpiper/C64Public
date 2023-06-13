# Parses CTM files and returns data from those files

# C:\work\c64\ExternalTools\CharPad 2.0 310715\Help\CharPad 2 - Help - CTM (V5) Format.txt
# file:///C:/work/c64/ExternalTools/CharPad%20Free%20Edition%202.7.2/Help/File%20Format%20-%20CTM%20(V7).htm

fileContent = []

fileExpectedVersions = [0x05, 0x07]

colouringMethodGlobal = 0
colouringMethodPerTile = 1
colouringMethodPerCharacter = 2


def raiseException():
    raise Exception("CTM file not a known version " + fileExpectedVersions)


def getLoHiValue(offset):
    return fileContent[offset] + (fileContent[offset + 1] << 8)


def loadCTMFile(filename):
    global fileContent
    with open(filename, mode='rb') as file:
        fileContent = file.read()

    if fileContent[0] != 0x43 or fileContent[1] != 0x54 or fileContent[2] != 0x4d:
        raise Exception("Not a CTM file")

    if fileContent[3] not in fileExpectedVersions:
        raiseException


def getBackgroundColour():
    if fileContent[3] == 0x05 or fileContent[3] == 0x07:
        return fileContent[4]
    raiseException()


def getMulticolour1():
    if fileContent[3] == 0x05 or fileContent[3] == 0x07:
        return fileContent[5]
    raiseException()


def getMulticolour2():
    if fileContent[3] == 0x05 or fileContent[3] == 0x07:
        return fileContent[6]
    raiseException()


def getGlobalColour():
    if fileContent[3] == 0x05 or fileContent[3] == 0x07:
        return fileContent[7]
    raiseException()


def getColouringMethod():
    if fileContent[3] == 0x05:
        return fileContent[8]
    if fileContent[3] == 0x07:
        return fileContent[9]
    raiseException()


def getNumCharacters():
    if fileContent[3] == 0x05:
        return getLoHiValue(10) + 1
    if fileContent[3] == 0x07:
        return getLoHiValue(14) + 1
    raiseException()


def getCharacters():
    if fileContent[3] == 0x05:
        return fileContent[20:20+(getNumCharacters()*8)]
    if fileContent[3] == 0x07:
        return fileContent[16:16+(getNumCharacters()*8)]
    raiseException()


def getCharacterAttributes():
    if fileContent[3] == 0x05:
        return fileContent[20+(getNumCharacters()*8):20+(getNumCharacters()*8)+getNumCharacters()]
    if fileContent[3] == 0x07:
        return fileContent[16+(getNumCharacters()*8)+2:16+(getNumCharacters()*8)+2+getNumCharacters()]
    raiseException()


def usingTiles():
    if fileContent[3] == 0x05:
        return (fileContent[9] & 0x01) == 0x01
    if fileContent[3] == 0x07:
        return (fileContent[11] & 0x01) == 0x01
    raiseException()


def getTileConfigOffset():
    if fileContent[3] == 0x07:
        return 16 + (getNumCharacters() * 8) + 2 + getNumCharacters() + 2
    raiseException()


def getTileDataOffset():
    if fileContent[3] == 0x05:
        return 20 + (getNumCharacters() * 8) + getNumCharacters()
    if fileContent[3] == 0x07:
        return 16 + (getNumCharacters() * 8) + 2 + getNumCharacters() + 2 + 2 + 1 + 1
    raiseException()


def getMapConfigOffset():
    if fileContent[3] == 0x07:
        if usingTiles():
            # For now assuming no tags and default "Untitled" names
            return getTileDataOffset() + (getNumTiles() * getTilesWidth() * getTilesHeight() * 2) + 2 + getNumTiles() + 2 + (getNumTiles() * 9) + 2
        else:
            return 16 + (getNumCharacters() * 8) + 2 + getNumCharacters() + 2
    raiseException()


def getMapDataOffset():
    if fileContent[3] == 0x05:
        if usingTiles():
            # Fior now ignore any per tile colouring option
            return 20 + (getNumCharacters() * 8) + getNumCharacters() + (getNumTiles() * getTilesWidth() * getTilesHeight() * 2)
        else:
            return 20 + (getNumCharacters() * 8) + getNumCharacters()
    if fileContent[3] == 0x07:
        return getMapConfigOffset() + 2 + 2
    raiseException()

def getNumTiles():
    if usingTiles():
        if fileContent[3] == 0x05:
            return getLoHiValue(12) + 1
        if fileContent[3] == 0x07:
            offset = getTileConfigOffset()
            return getLoHiValue(offset) + 1
    raiseException()



def getTilesWidth():
    if usingTiles():
        if fileContent[3] == 0x05:
            return fileContent[14]
        if fileContent[3] == 0x07:
            offset = getTileConfigOffset() + 2
            return fileContent[offset]
    raiseException()


def getTilesHeight():
    if usingTiles():
        if fileContent[3] == 0x05:
            return fileContent[15]
        if fileContent[3] == 0x07:
            offset = getTileConfigOffset() + 2 + 1
            return fileContent[offset]
    raiseException()


def getTileData():
    if usingTiles():
        offset = getTileDataOffset()
        tiledata = []
        for idx in range(getNumTiles()):
            for y in range(getTilesHeight()):
                for x in range(getTilesWidth()):
                    tiledata.append(fileContent[offset])
                    offset += 2
        return bytearray(tiledata)
    raiseException()


def getMapWidth():
    if fileContent[3] == 0x05:
        return getLoHiValue(16)
    if fileContent[3] == 0x07:
        offset = getMapConfigOffset()
        return getLoHiValue(offset)
    raiseException()


def getMapHeight():
    if fileContent[3] == 0x05:
        return getLoHiValue(18)
    if fileContent[3] == 0x07:
        offset = getMapConfigOffset() + 2
        return getLoHiValue(offset)
    raiseException()


def getMapData():
    offset = getMapDataOffset()
    mapdata = []
    for y in range(getMapHeight()):
        for x in range(getMapWidth()):
            mapdata.append(fileContent[offset])
            offset += 2
    return bytearray(mapdata)
