from Visual import Visual
from Process import Process


if __name__ == "__main__":
    title = ["Numbered.png", "Enhanced.png"]
    a = Visual.image_open(title[0])
    b = Visual.image_open(title[1])
    Visual.plot("Image", a)
