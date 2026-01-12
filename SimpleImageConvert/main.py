# pip install pillow
import sys

from PIL import Image


def print_hi(name):
    print(f'Hi, {name}')  # Press Ctrl+F8 to toggle the breakpoint.


# Press the green button in the gutter to run the script.
if __name__ == '__main__':

    if len(sys.argv) < 3:
        print("<input palette image> <input image to convert> <output binary file>")
        print("Output format : bytes lo/hi width, then lo/hi height, then pixel data")
        exit(-1)

    # First calculate the colout to index dictionary using the input palette
    image = Image.open(sys.argv[1])
    image.load()
    image = image.convert("RGB")
    width, height = image.size
    dRGBToIndex = {}
    for y in range(height):
        for x in range(width):
            pixel = image.getpixel([x, y])
            if not dRGBToIndex.__contains__(pixel):
                dRGBToIndex[pixel] = len(dRGBToIndex)

    print("input palette", dRGBToIndex)

    image = Image.open(sys.argv[2])
    image.load()
    image = image.convert("RGB")
    print("size ", image.size)

    width, height = image.size
    output = open(sys.argv[3], "wb")
    output.write(bytes([width & 0xff, width >> 8]))
    output.write(bytes([height & 0xff, height >> 8]))

    for y in range(height):
        for x in range(width):
            pixel = image.getpixel([x, y])
            index = dRGBToIndex[pixel]
            output.write(bytes([index]))
