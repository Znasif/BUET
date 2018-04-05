from Visual import Visual
from Process import Process


if __name__ == "__main__":
    title = ["Numbered.png", "Enhanced.png"]
    a = Visual.image_open(title[0])
    b = Visual.image_open(title[1])
    d = Visual.image_open(title[1], 1)
    d = Process.get_subplots(a, b)
    for i in d:
        Visual.plot("OK", i)

