# Create the file assets\Demo14\runway.png

# pip install pypng
import png

width = 1024
height = 1024
img = []

# Light runway
for y in range(128):
    row = ()
    xpos = 0
    for x in range(width):
        if xpos > 256 - (y) and xpos < 256 + (y):
            if xpos > 256 - ((6.8*y)/8) and xpos < 256 - ((6*y)/8):
                row = row + (255, 255, 255)
            else:
                if xpos > 256 - ((3 * y) / 8) and xpos < 256 - ((2.4 * y) / 8):
                    row = row + (255, 255, 255)
                else:
                    if xpos > 256 + ((2.4 * y) / 8) and xpos < 256 + ((3 * y) / 8):
                        row = row + (255, 255, 255)
                    else:
                        if xpos > 256 + ((6 * y) / 8) and xpos < 256 + ((6.8 * y) / 8):
                            row = row + (255, 255, 255)
                        else:
                            row = row + (128, 128, 128)
        else:
            row = row + (255, 0, 255)
        xpos = xpos + 1
    img.append(row)

# Dark runway
for y in range(128):
    row = ()
    xpos = 0
    for x in range(width):
        if xpos > 256 - (y) and xpos < 256 + (y):
            if xpos > 256 - ((7*y)/8) and xpos < 256 + ((7*y)/8):
                row = row + (96, 96, 96)
            else:
                row = row + (64, 64, 64)
        else:
            row = row + (255, 0, 255)
        xpos = xpos + 1
    img.append(row)

for y in range(1024-256):
    row = ()
    for x in range(width):
        row = row + (255, 0, 255)
    img.append(row)

with open('../runway.png', 'wb') as f:
    w = png.Writer(width, height, greyscale=False)
    w.write(f, img)
