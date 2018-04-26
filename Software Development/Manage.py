from Visual import Visual
from Process import Process


if __name__ == "__main__":
    title = ["Numbered.png", "Enhanced.png", "nums.jpg", "plot.jpg", "port.jpg", "see.jpg", "subsection.jpg",
             "testplot.jpg"]

    a = Visual.image_open(title[3], 1)
    b = Visual.image_open(title[3])
    b, _ = Process.get_contour(b, 1, a)
    Visual.image_write(title[3]+"subs", b)
