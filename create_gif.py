import os

from PIL import Image

i = 0
images = list()
while True:
    path = os.path.join(
        os.path.dirname(__file__),
        f'out\\recording.{i}.png'
    )
    print(path)
    if not os.path.exists(path):
        break
    img = Image.open(path).convert('RGBA', dither=None, palette='WEB')
    images.append(img)
    i += 1

images[0].save(
    'out\\recording.gif',
    save_all=True,
    append_images=images[1:],
    optimize=True,
    duration=250,
    loop=0
)
