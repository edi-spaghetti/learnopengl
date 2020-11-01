import sys
import random
from functools import reduce
from PIL import Image, ImageDraw, ImageFont, ImageOps

#  seed for testing to ensure same images come out each time
try:
    SEED = sys.argv[1]
except IndexError:
    SEED = 2
random.seed(SEED)

IMG_WIDTH = 418
IMG_HEIGHT = 418


class AnimatedTexture:

    def __init__(self):

        self.start = ord('!')
        self.stop = ord('_')
        self.length = 100  # num characters in a stream

        # 3 colours in rgb
        imgs = list()
        for i in range(3):
            imgs.append(self.letter_stream(i))

        # combine them all into one image
        img = reduce(self.combine, imgs)

        # add the border and save it out
        img = ImageOps.expand(img, border=41, fill='black')
        img.save('container2_animated_emission.png')

    def combine(self, a, b):
        """
        Combine the images
        Probably a better way to do this but blend was giving me issues
        """

        c = Image.new('RGB', (IMG_WIDTH, IMG_HEIGHT))
        a_data = a.getdata()
        b_data = b.getdata()

        c_data = list()
        for i in range(len(a_data)):
            a_rgb = a_data[i]
            b_rgb = b_data[i]

            c_rgb = [0, 0, 0]
            for k in range(3):
                c_rgb[k] = a_rgb[k] + b_rgb[k]

            c_data.append(tuple(c_rgb))

        c.putdata(c_data)
        return c

    # make the letters
    def letter_stream(self, index):

        img = Image.new('RGB', (IMG_WIDTH, IMG_HEIGHT), color='black')
        draw = ImageDraw.Draw(img)
        draw.fontmode = '1'  # turn off anti-aliasing

        x = -5
        MAX_GAP = 50
        while x < IMG_WIDTH:

            text_size = random.randint(12, 24)
            font = ImageFont.truetype('ocr-a-ext.ttf', text_size)

            # ensure some horizontal variation
            y = random.randint(-10, 0)

            # determine the stream duration
            stream_start = 128 + random.randint(-10, 127)
            stream_speed = random.randint(3, 5)

            # capture width of stream to ensure no overlapping
            max_width = 0

            for i in range(self.length):
                letter = chr(random.randint(self.start, self.stop))

                # calculate character y offset
                ascent, descent = font.getmetrics()
                (width, baseline), (offset_x, offset_y) = font.font.getsize(letter)
                height = offset_y + (ascent - offset_y) + descent
                char_y = y + height * i

                # bail out early if we're already way off the canvas
                if char_y > IMG_HEIGHT + height * 2:
                    break

                # update max width if character is a fatty
                if width > max_width:
                    max_width = width

                # calculate red values
                val = int(stream_start + i * stream_speed)
                if val > 255:
                    break
                colour = [0, 0, 0]
                colour[index] = val
                colour = tuple(colour)

                draw.text(
                    (x, char_y),
                    letter,
                    font=font,
                    fill=colour
                )

            # start a new stream with enough space so they don't overlap
            x = x + max_width + MAX_GAP * random.random()

        return img


if __name__ == '__main__':
    AnimatedTexture()
