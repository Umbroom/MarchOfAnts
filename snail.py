file_name = 'hoge'
extension = '.png'


from PIL import Image


def extract(r, g, b):
    if (r<120 and g<120 and b<120):
        a = r+g+b
        a = 300 - a//3

        if(a>255):
            a = 255
        return (0, 0, 0, a)
    else:
        return (0, 0, 0, 0)


percent = 0

img = Image.open(file_name + extension)
width, height = img.size

for y in range(height):
    for x in range(width):
        r, g, b, a = img.getpixel((x, y))
        img.putpixel((x, y), extract(r, g, b))

    pct = int(y / height*100)
    if(percent<pct):
        percent = pct
        print(str(percent) + '%')

img.save(file_name + '2' + extension)

print('----complete.')