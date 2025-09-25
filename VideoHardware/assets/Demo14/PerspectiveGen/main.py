# pip install pyquaternion
# pip install graphics.py
import random
# pip install pypng
import png
# pip install Pillow
#from PIL import Image, ImageDraw
import PIL.Image
import PIL.ImageDraw

from pyquaternion import Quaternion
import math

from graphics import *

renderFlagTitle = False
#renderFlagTitle = True

renderFlagLandscape = False
#renderFlagLandscape = True

calculateRadar = False


def insideScreen(p):
    if 0 <= p.x < 328 and 0 <= p.y < 224:
        return True
    else:
        return False


def drawSprite(width, height, x, y, scale):
    width = width / 4
    height = height / 4
    #    width = width / 2
    #    height = height / 2
    realWidth = width * scale
    realHeight = height * scale
    sprite = Rectangle(Point(x - (realWidth / 2), y - (realHeight / 2)),
                       Point(x + (realWidth / 2), y + (realHeight / 2)))
    sprite.setFill(color_rgb(min(abs(int(x)), 255), min(abs(int(y)), 255), 64))
    sprite.draw(win)


maxHardwareScale = 0

#    print(x,y,scale)

binaryDataFile = open("../../../bin/Demo14LargeTables.bin", "wb")
dataOffsetsFile = open("../../../tmp/Demo14LargeTablesIncludeFrames.a", "w")

# First write out the perspective table in the first 64K of memory
for zValue in range(256):
    for xValue in range(256):
        if xValue >= 128:
            x = -(256 - xValue)
        else:
            x = xValue
        realZ = 16 + zValue
        realX = ((x * (64)) / realZ)
        if realX < -96 or realX > 96:
            binaryDataFile.write(bytes([128]))
        else:
            binaryDataFile.write(bytes([int(round(realX)) & 0xff]))

# Write an identifier that the C64 can check for in the external RAM
dataOffsetsFile.write(
    "gameDataIdentifier = $" + hex(binaryDataFile.tell() & 0xffffff)[2:] + "\n")
binaryDataFile.write("MW2000-AfterBurner".encode())


# Check size over 64K by adding lots of extra blank data at the start
# binaryDataFile.write(bytearray(32768))

radarWidth = 46
radarHeight = 46
shiftHalf = False
def commonCalculateLandscape():
    global win, currentLandscapeLookup, x, realZ, realX, hardwareScale, maxHardwareScale, frameXMod, calculateRadar
    if calculateRadar:
        img = PIL.Image.new('RGB', (radarWidth, radarHeight), (255, 0, 255))
        draw = PIL.ImageDraw.Draw(img)
        coordx1 = math.cos(math.radians(rotationZ)) * -(radarWidth/2)
        coordy1 = math.sin(math.radians(rotationZ)) * -(radarWidth/2)
        coordx2 = math.cos(math.radians(rotationZ)) * (radarWidth/2)
        coordy2 = math.sin(math.radians(rotationZ)) * (radarWidth/2)
        if shiftHalf:
            coordx1 += 0.5
            coordy1 += 0.5
            coordx2 += 0.5
            coordy2 += 0.5
        draw.line((coordx1 + (radarWidth/2), coordy1 + (radarHeight/2), coordx2 + (radarWidth/2), coordy2 + (radarHeight/2)), fill=(255,255,255))
        # Let's draw some horizon ground markers too
        realX = -(radarWidth/4)
        realY = (radarWidth/7)
        coordx1 = (math.cos(math.radians(rotationZ)) * realX) - (math.sin(math.radians(rotationZ)) * realY)
        coordy1 = (math.sin(math.radians(rotationZ)) * realX) + (math.cos(math.radians(rotationZ)) * realY)
        realX = (radarWidth/4)
        realY = (radarWidth/7)
        coordx2 = (math.cos(math.radians(rotationZ)) * realX) - (math.sin(math.radians(rotationZ)) * realY)
        coordy2 = (math.sin(math.radians(rotationZ)) * realX) + (math.cos(math.radians(rotationZ)) * realY)
        if shiftHalf:
            coordx1 += 0.5
            coordy1 += 0.5
            coordx2 += 0.5
            coordy2 += 0.5
        draw.line((coordx1 + (radarWidth/2), coordy1 + (radarHeight/2), coordx2 + (radarWidth/2), coordy2 + (radarHeight/2)), fill=(255,255,255))

        realX = -(radarWidth/4)
        realY = (radarWidth/5)
        coordx1 = (math.cos(math.radians(rotationZ)) * realX) - (math.sin(math.radians(rotationZ)) * realY)
        coordy1 = (math.sin(math.radians(rotationZ)) * realX) + (math.cos(math.radians(rotationZ)) * realY)
        realX = (radarWidth/4)
        realY = (radarWidth/5)
        coordx2 = (math.cos(math.radians(rotationZ)) * realX) - (math.sin(math.radians(rotationZ)) * realY)
        coordy2 = (math.sin(math.radians(rotationZ)) * realX) + (math.cos(math.radians(rotationZ)) * realY)
        if shiftHalf:
            coordx1 += 0.5
            coordy1 += 0.5
            coordx2 += 0.5
            coordy2 += 0.5
        draw.line((coordx1 + (radarWidth/2), coordy1 + (radarHeight/2), coordx2 + (radarWidth/2), coordy2 + (radarHeight/2)), fill=(255,255,255))

        realX = -(radarWidth/4)
        realY = (radarWidth/4)
        coordx1 = (math.cos(math.radians(rotationZ)) * realX) - (math.sin(math.radians(rotationZ)) * realY)
        coordy1 = (math.sin(math.radians(rotationZ)) * realX) + (math.cos(math.radians(rotationZ)) * realY)
        realX = (radarWidth/4)
        realY = (radarWidth/4)
        coordx2 = (math.cos(math.radians(rotationZ)) * realX) - (math.sin(math.radians(rotationZ)) * realY)
        coordy2 = (math.sin(math.radians(rotationZ)) * realX) + (math.cos(math.radians(rotationZ)) * realY)
        if shiftHalf:
            coordx1 += 0.5
            coordy1 += 0.5
            coordx2 += 0.5
            coordy2 += 0.5
        draw.line((coordx1 + (radarWidth/2), coordy1 + (radarHeight/2), coordx2 + (radarWidth/2), coordy2 + (radarHeight/2)), fill=(255,255,255))

    #math.cos(math.radians(rotationZ))
    if renderFlagLandscape:
        win = GraphWin(width=512, height=512)
        win.setCoords(0, 256, 256, 0)
    currentLandscapeLookup = bytearray()
    for zPos in range(7, -1, -1):
        #        currentLandscapeLookup += bytearray([binaryDataFile.tell() & 0xff, (binaryDataFile.tell() >> 8) & 0xff])
        currentLandscapeLookup += bytearray(
            [binaryDataFile.tell() & 0xff, (binaryDataFile.tell() >> 8) & 0xff, (binaryDataFile.tell() >> 16) & 0xff])
        currentLandscape = bytearray()
        numSprites = 0
        print("*************** frame *****************" + str(zPos))
        if renderFlagLandscape:
            win.flush()
        spriteRowCoordinate = 0
        for z in range(0, 64, 8):
            spriteRowCoordinate2 = 0
            for x in range(-7, 8):
                #            for x in range(-3 , 4):
                #                print(x,z , zPos)
                realZ = 16 + zPos + z
                # Adds a slight skew so that sprites are not precisely the same Z position, creates nice visual variation
                realZ += x / 2
                x = x + (frameXMod / 8)
                # Adds horizontal frame movement based on rotation angle
                if rotationZ >= 45 and rotationZ <= 120:
                    realX = 128 + (((x - (zPos / 8)) * 750) / realZ)
                elif rotationZ >= 240 and rotationZ <= 315:
                    realX = 128 + (((x + (zPos / 8)) * 750) / realZ)
                else:
                    realX = 128 + ((x * 750) / realZ)

                realX2 = (150 / realZ)
                realY = 128 + ((1600 * landscapeHeight / 128) / realZ)

                # Apply rotation
                realX = realX - 128
                realY = realY - 128

                newRealX = (math.cos(math.radians(rotationZ)) * realX) - (math.sin(math.radians(rotationZ)) * realY)
                newRealY = (math.sin(math.radians(rotationZ)) * realX) + (math.cos(math.radians(rotationZ)) * realY)

                realX = 128 + newRealX
                realY = 128 + newRealY

                if realX >= marginXLeft and realX <= 255 - marginXRight and realY >= marginYTop and realY <= 255 - marginYBottom:
                    theScale = (16.0 + 7) / realZ
                    hardwareScale = int(48 / theScale)
                    #                    hardwareScale = int(32 / theScale)
                    if hardwareScale >= 16 and hardwareScale <= 210:
                        # Note: sbc #48 is here now
                        adjustedX = int(realX) - 48
                        adjustedX *= 2
                        adjustedXMSB = 0
                        if adjustedX & 0x100:
                            adjustedXMSB = 0x10
                        adjustedX2 = int(realX + realX2) - 48
                        adjustedX2 *= 2
                        adjustedX2MSB = 0
                        if adjustedX2 & 0x100:
                            adjustedX2MSB = 0x10
                        currentLandscape += bytearray(
                            [int(spriteRowCoordinate + spriteRowCoordinate2) & 0xff, int(hardwareScale) & 0xff,
                             int(realY) & 0xff, adjustedX & 0xff, adjustedXMSB, adjustedX2 & 0xff, adjustedX2MSB])
                        numSprites = numSprites + 1

                        if hardwareScale > maxHardwareScale:
                            maxHardwareScale = hardwareScale

                        if renderFlagLandscape:
                            drawSprite(119, 85, realX, realY, theScale)
                spriteRowCoordinate2 = spriteRowCoordinate2 + 1
            spriteRowCoordinate = spriteRowCoordinate + 16

        finalFrame = bytearray([int(numSprites) & 0xff]) + currentLandscape
        binaryDataFile.write(finalFrame)

        if renderFlagLandscape:
            Rectangle(Point(48, 64), Point(208, 192)).draw(win)
            Line(Point(0, 128), Point(256, 128)).draw(win)

    if renderFlagLandscape:
        #        win.getMouse()
        win.close()

    if calculateRadar:
        img.save("../Radar/radar"+str(int(rotationZ))+".png" , 'png')

    return currentLandscapeLookup


def commonCalculateRunway():
    global win, currentLandscapeLookup, x, realZ, realX, hardwareScale, maxHardwareScale
    currentLandscapeLookup = bytearray()
    for zPos in range(7, -1, -1):
        currentLandscapeLookup += bytearray(
            [binaryDataFile.tell() & 0xff, (binaryDataFile.tell() >> 8) & 0xff, (binaryDataFile.tell() >> 16) & 0xff])
        print("*************** runway frame *****************" + str(zPos))
        runwayToRowFrame = bytearray(256)
        z = 64-8+16
        while z >= 0:
#            realZ = 16 + zPos + z
            realZ = 1 + z

            realY = ((1600 * landscapeHeight / 128) / realZ)
            # Adjust for screen coordinates, not shifted sprite coordinates
            realY = realY * 2

            theScale = (70.0 * 16) / realZ
#            theScale = theScale * 2
            realY = int(realY)
#            print("realZ = " + str(realZ) + " realY = " + str(realY) + " theScale = " + str(theScale))
            if theScale >= 1:
                if realY <= 255:
                    if theScale > 127:
                        theScale = 127

                    theScale = int(theScale) & 0x7f
                    # For the stripes
                    if int(realZ - zPos) & 0x04:
                        theScale = theScale | 0x80
                    runwayToRowFrame[realY] = theScale

            z = z - 0.01

        startOffset = 0
        while runwayToRowFrame[0] == 0:
            del runwayToRowFrame[0]
            startOffset = startOffset + 1

        while runwayToRowFrame[len(runwayToRowFrame)-1] == 0:
            runwayToRowFrame.pop()

        print("startOffset = " + str(startOffset))
        print("len runwayToRowFrame = " + str(len(runwayToRowFrame)))
        for x in runwayToRowFrame:
            print("$" + hex(x)[2:] + " , ", end='')
        print()

        # Horizon offset row and the length of this chunk
        finalFrame = bytearray([int(startOffset) & 0xff, int(len(runwayToRowFrame)) & 0xff]) + runwayToRowFrame
        binaryDataFile.write(finalFrame)

    return currentLandscapeLookup


marginXLeft = 16
marginXRight = 32   # Because the canyon has large objects we expand the right hand edge a bit
marginYTop = 48
marginYBottom = 48
rotationZ = 0
frameXMod = 0


# Debug
landscapeHeight = 64
landscapeHeight = 128
#currentLandscapeLookup = commonCalculateRunway()


# Debug rotated landscape frames
#calculateRadar = True
#frameXMod = 0
#marginYBottom = 32
#rotationZ = 45.0
#currentLandscapeLookup = commonCalculateLandscape()



# Canyon sequence
frameXMod = 0
landscapeHeight = 64
marginYBottom = 48
while frameXMod < 8:
    # This creates labels like: landscapeScrollX000Frames = <offset>
    currentLandscapeLookup = commonCalculateLandscape()
    dataOffsetsFile.write(
        "landscapeScrollX" + "{:03d}".format(int(frameXMod)) + "Frames = $" + hex(
            binaryDataFile.tell() & 0xffffff)[2:] + "\n")
    binaryDataFile.write(currentLandscapeLookup)
    frameXMod = frameXMod + 1


marginXRight = 48


# Takeoff (and landing) frames
frameXMod = 0
landscapeHeight = 64
marginYBottom = 48

# Landing and takeoff sequence
while landscapeHeight <= 128:
    # This creates labels like: landscapeHeight128Frames = <offset>
    currentLandscapeLookup = commonCalculateLandscape()
    dataOffsetsFile.write(
        "landscapeHeight" + "{:03d}".format(int(landscapeHeight)) + "Frames = $" + hex(
            binaryDataFile.tell() & 0xffffff)[2:] + "\n")
    binaryDataFile.write(currentLandscapeLookup)


    # This creates labels like: runwayForHeight128Frames = <offset>
    currentLandscapeLookup = commonCalculateRunway()
    dataOffsetsFile.write(
        "runwayForHeight" + "{:03d}".format(int(landscapeHeight)) + "Frames = $" + hex(
            binaryDataFile.tell() & 0xffffff)[2:] + "\n")
    binaryDataFile.write(currentLandscapeLookup)

    landscapeHeight = landscapeHeight + 1



# Rotated landscape frames
frameXMod = 0
marginYBottom = 32

# Rotation
calculateRadar = True
while rotationZ < 360.0:
    currentLandscapeLookup = commonCalculateLandscape()

    # This creates labels like: landscape175Frames = <offset>
    dataOffsetsFile.write(
        "landscape" + "{:03d}".format(int(rotationZ)) + "Frames = $" + hex(binaryDataFile.tell() & 0xffffff)[2:] + "\n")
    binaryDataFile.write(currentLandscapeLookup)

    rotationZ = rotationZ + 5

# displays table label names in order
rotationZ = 180 - 5
while rotationZ >= 0:
    print("landscape" + "{:03d}".format(int(rotationZ)) + "Frames , ", end='')
    rotationZ = rotationZ - 5

print("")

rotationZ = 360 - 5
while rotationZ >= 180:
    print("landscape" + "{:03d}".format(int(rotationZ)) + "Frames , ", end='')
    rotationZ = rotationZ - 5

print("")

calculateRadar = False
landscapeHeight = 0
while landscapeHeight < 8:
    print("landscapeScrollX" + "{:03d}".format(int(landscapeHeight)) + "Frames , ", end='')
    landscapeHeight = landscapeHeight + 1

print("")

landscapeHeight = 64
while landscapeHeight <= 128:
    print("landscapeHeight" + "{:03d}".format(int(landscapeHeight)) + "Frames , ", end='')
    landscapeHeight = landscapeHeight + 1

print("")

landscapeHeight = 64
while landscapeHeight <= 128:
    print("runwayForHeight" + "{:03d}".format(int(landscapeHeight)) + "Frames , ", end='')
    landscapeHeight = landscapeHeight + 1

print("")

# Draw a grid of balls
dataOffsetsFile.write("titleScreenAnimation1 = $" + hex(binaryDataFile.tell() & 0xffffff)[2:] + "\n")

# 12 x 11
grid = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0],
        [0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0],
        [0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], ]

points = []

for y in range(11):
    for x in range(12):
        point = grid[y][x], [x - 5.5, 0.0, y - 5]
        points.append(point)

writeEvery = 5
writeEveryCount = 0

axis = [1, 0, 0]
angle = 0
# camera = [0, 1, -5]  # At 13.7 MHz
camera = [0, 2, -5]  # At 12.096 MHz
win = GraphWin(width=328, height=224, autoflush=False)
win.setCoords(0, 224, 328, 0)
Line(Point(164, 0), Point(164, 224)).draw(win)
Line(Point(0, 112), Point(328, 112)).draw(win)

while angle < 360 * 9:
    # while angle < 360:
    theta = math.radians(angle)
    if angle == 0:
        axis = [0, 1, 0]
    if angle == 360:
        axis = [1, 0, 0]
    if angle == 360 * 2:
        axis = [1, 2, 3]
    if angle == 360 * 3:
        axis = [1, 0, 0]
    if angle >= (360 * 3) + 90:
        theta = math.radians(90)
    if angle == 360 * 5:
        axis = [1, 0, 0]
    if angle == 360 * 7:
        axis = [-3, -2, -1]
    if angle == 360 * 8:
        axis = [0, 1, 0]
    if angle >= 360 * 5:
        theta = math.radians(angle)
    if angle >= 360 * 8:
#        camera[1] += 0.01  # At 13.7 MHz
        camera[1] += 0.02  # At 12.096 MHz
        camera[2] -= 0.05
    if 10 <= angle < 360:
        camera[2] += 0.05
    if 400 <= angle < 500:
#        camera[1] -= 0.01  # At 13.7 MHz
        camera[1] -= 0.02  # At 12.096 MHz

    # For the explosion etc
    random.seed(1234)

    toSort = []
    for point in points:
        colour = point[0]
        origPos = point[1]

        v = Quaternion(axis=axis, angle=theta).rotate(origPos)

        explosionVector = [0, 0, 0]
        explosionVector[0] = origPos[0] * random.uniform(0.01, 0.04)
        explosionVector[1] = -origPos[2] * random.uniform(0.01, 0.04)
        explosionVector[2] = random.uniform(-0.05, -0.01)

        # Explode object
        if 360 * 4 <= angle < 360 * 5:
            deltaTime = angle - (360 * 4)
            v[0] += explosionVector[0] * deltaTime
            v[1] += explosionVector[1] * deltaTime
            # And move it behind the camera
            v[2] += -abs(explosionVector[2]) * deltaTime

        # Bring object back again
        if 360 * 5 <= angle < 360 * 6:
            deltaTime = angle - (360 * 5)
            deltaTime = 360 - deltaTime
            v[0] += explosionVector[0] * deltaTime
            v[1] += explosionVector[1] * deltaTime
            # And move it behind the camera
            v[2] += -abs(explosionVector[2]) * deltaTime

        v[0] = v[0] + camera[0]
        v[1] = v[1] + camera[1]
        v[2] = v[2] + camera[2]

        if v[2] > 1:
            screenX = 164 + (200.0 * v[0] / v[2])
            screenY = 112 + (200.0 * v[1] / v[2])

            scale = 3 / v[2]
            # From the sprite pixel size
            sizeX = 60.0 * scale
            sizeY = 59.0 * scale
            hardwareScale = 32.0 / scale
            if 0 < hardwareScale < 255 and sizeX <= 255 and sizeY <= 255:
                topLeft = Point(screenX - (sizeX / 2), screenY - (sizeY / 2))
                bottomRight = Point(screenX + (sizeX / 2), screenY + (sizeY / 2))
                if (insideScreen(topLeft) or insideScreen(bottomRight)
                        or insideScreen(Point(bottomRight.x, topLeft.y))
                        or insideScreen(Point(topLeft.x, bottomRight.y))):
                    toSort.append([hardwareScale, colour, topLeft, bottomRight, sizeX, sizeY])
                    if hardwareScale > maxHardwareScale:
                        maxHardwareScale = hardwareScale

    toSort.sort(key=lambda tup: tup[0])
    if writeEveryCount == 0:
        binaryDataFile.write(bytes([len(toSort)]))

    for element in toSort:
        msb = 0
        if int(element[2].x) & 0x100:
            msb |= 0x10
        if int(element[2].y) & 0x100:
            msb |= 0x20
        thisElement = bytes(
            [element[1], msb, int(element[2].x) & 0xff, int(element[4]), int(element[2].y) & 0xff, int(element[5]),
             int(element[0]) & 0xff])
        if writeEveryCount == 0:
            binaryDataFile.write(thisElement)
        if renderFlagTitle:
            sprite = Rectangle(element[2], element[3])
            sprite.setFill(color_rgb(min(abs(int(element[1] * 100)), 255), min(abs(int(element[1] * 100)), 255), 64))
            sprite.draw(win)

    if writeEveryCount == 0:
        nextPos = binaryDataFile.tell() & 0xffffff
        nextPos = nextPos + 3
        binaryDataFile.write(bytes([int(nextPos) & 0xff, int(nextPos >> 8) & 0xff, int(nextPos >> 16) & 0xff]))

    if renderFlagTitle:
        win.update()

    writeEveryCount += 1
    if writeEveryCount >= writeEvery:
        writeEveryCount = 0

    #    win.getMouse()
    for item in win.items[:]:
        item.undraw()
    #    win.update()
    #    win.close()
    angle += 1

# End of animation marker
binaryDataFile.write(bytes([255]))

# Round up to include all values...
dataOffsetsFile.write("maxHardwareScaleTabValue = " + str(int(maxHardwareScale + 1)) + "\n")

# Output horizon tables
ScreenLandscapeOriginYPos = 88
ScreenLandscapeNumRollSteps = 72
ScreenLandscapeNumStepsDelta = int(360 / ScreenLandscapeNumRollSteps)
dataOffsetsFile.write("landscapeRotationHorizonOffsets = $" + hex(binaryDataFile.tell() & 0xffffff)[2:] + "\n")


def Mode7RotationOffset():
    theOffsetX = int((220.0 * 256.0) + (184.0 * -256.0 * math.sin(math.radians(float(angle + 90)))) + (
            (ScreenLandscapeOriginYPos + 24.0 - screenYOffset) * -256.0 * math.sin(math.radians(float(angle)))))
    theOffsetY = int((128.0 * 256.0) + (184.0 * -256.0 * math.sin(math.radians(float(angle + 90 + 90)))) + (
            (ScreenLandscapeOriginYPos + 24.0 - screenYOffset) * 256.0 * math.sin(
        math.radians(float(angle + 90 + 90 + 90)))))
    thisElement = bytes(
        [theOffsetX & 0xff, (theOffsetX >> 8) & 0xff, (theOffsetX >> 16) & 0xff,
         theOffsetY & 0xff, (theOffsetY >> 8) & 0xff, (theOffsetY >> 16) & 0xff])
    #    thisElement = bytes([0,0,0,0,0,0])
    binaryDataFile.write(thisElement)


for horizonOffset in range(27):
    screenYOffset = (horizonOffset - 12) * 2 * 2
    for rollSteps in range(ScreenLandscapeNumRollSteps >> 1):
        angle = 180 - ((rollSteps + 1) * ScreenLandscapeNumStepsDelta)
        Mode7RotationOffset()
    for rollSteps in range(ScreenLandscapeNumRollSteps >> 1):
        angle = 360 - ((rollSteps + 1) * ScreenLandscapeNumStepsDelta)
        Mode7RotationOffset()

# Rotation tables
# Force data alignment to 0x100 bytes, makes random low byte addressing code faster
while binaryDataFile.tell() & 0xff:
    binaryDataFile.write(bytes([0]))
dataOffsetsFile.write("rotationVectorTable = $" + hex(binaryDataFile.tell() & 0xffffff)[2:] + "\n")


def MakeRotationVector():
    for i in range(256):
        x = i
        if x >= 128:
            x = -(256 - x)
        theX = float(x) * math.sin(math.radians(angle + 90))
        thisElement = bytes([int(theX) & 0xff])
        binaryDataFile.write(thisElement)
    for i in range(256):
        x = i
        if x >= 128:
            x = -(256 - x)
        theY = float(x) * math.sin(math.radians(angle))
        thisElement = bytes([int(theY) & 0xff])
        binaryDataFile.write(thisElement)


for rollSteps in range(ScreenLandscapeNumRollSteps >> 1):
    angle = 180 - ((rollSteps + 1) * ScreenLandscapeNumStepsDelta)
    MakeRotationVector()
for rollSteps in range(ScreenLandscapeNumRollSteps >> 1):
    angle = 360 - ((rollSteps + 1) * ScreenLandscapeNumStepsDelta)
    MakeRotationVector()

# Always the last line
dataOffsetsFile.write("EndDataMarker = $" + hex(binaryDataFile.tell() & 0xffffff)[2:] + "\n")
