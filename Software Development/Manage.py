from Visual import Visual
from Process import Process


if __name__ == "__main__":
    title = ["Numbered.png", "Enhanced.png"]
    a = Visual.image_open(title[0])
    b = Visual.image_open(title[1])
    d = Visual.image_open(title[1], 1)
    c = Visual.get_nums(a, b)
    #d, _ = Process.get_contour(b, 1)

    Visual.image_write(title[0],c)
    #Visual.image_write(title[1],d)
