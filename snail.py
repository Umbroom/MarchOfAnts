import sys
import numpy
from PIL import Image


file_name = sys.argv[1]
extension = '.' + sys.argv[2]


def extract(l):
    if (l<=i):
        a = i - l + 200

        if(a>255):
            a = 255
        output_image_data[block_r*x+p][block_r*y+q] = (0, a)


def dust(n):
    percent = 0
    dustpan = 0
    for x in range(width):
        for y in range(height):
            if(output_image_data[x][y] != (0, 0)):

                for i in range(3, 15, 2):
                    black_sum = 0
                    for p in range(i):
                        for q in range(i):
                            if(0 <= x+p-i//2 and x+p-i//2 < width and 0 <= y+q-i//2 and y+q-i//2 < height):
                                if(output_image_data[x+p-i//2][y+q-i//2]!=(0, 0)):
                                    black_sum += 1

                    if(black_sum <= 2*(i//2)):
                        dustpan += 1
                        output_image_data[x][y] = (0, 0)
                        break

            pct = int((x*(height-1)+y)/((width-1)*(height-1)) * 100)
            if(percent<pct):
                percent = pct
                print('dust(' + str(n) + '): ' + str(percent) + '%')

    print('dustpan(' + str(n) + ')>> ' + str(dustpan))
    if(dustpan != 0 and n < 10):
        dust(n+1)


block_r = 100

input_image = Image.open(file_name + extension).convert('L')
width, height = input_image.size
input_image_data = [[input_image.getpixel((x, y)) for y in range(height)] for x in range(width)]

output_image = Image.new('LA', (width, height))
output_image_data = [[(0, 0) for y in range(height)] for x in range(width)]

width_fraction = width//block_r
height_fraction = height//block_r
width_div = width//block_r if width_fraction == 0 else width//block_r + 1
height_div = height//block_r if height_fraction == 0 else height//block_r + 1

percent = 0

for x in range(width_div):
    for y in range(height_div):
        each_color_sums = [0 for i in range(256)]

        block_width = block_r if (width >= block_r * (x+1)) else width_fraction
        block_height = block_r if (height >= block_r * (y+1)) else height_fraction

        for p in range(block_width):
            for q in range(block_height):
                l = input_image_data[block_r*x+p][block_r*y+q]
                each_color_sums[l] += 1

        for i in range(numpy.argmax(each_color_sums) + 1):
            if(each_color_sums[numpy.argmax(each_color_sums) - i] <= (block_width*block_height)/200):
                break

        i = numpy.argmax(each_color_sums) - i

        for p in range(block_width):
            for q in range(block_height):
                l = input_image_data[block_r*x+p][block_r*y+q]
                extract(l)

        pct = int((height_div * x + y)/(width_div * height_div) * 100)
        if(percent<pct):
            percent = pct
            print(str(percent) + '%')

dust(1)

for x in range(width):
    for y in range(height):
        output_image.putpixel((x, y), output_image_data[x][y])

output_image.save(file_name + '2.png')

print('----complete.')