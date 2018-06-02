from Visual import Visual
from Process import Process


def make_subplots(t):
    """
    Stores a sub-categorized image in the title+"sub.jpg" file
    :param t: file name
    :return: NIL
    """
    a = Visual.image_open(t)
    b = Visual.image_open(t, 1)
    a, b = Process.get_contour(a, 1, b)
    Visual.image_write(t.split('.')[0] + "_subs", a)


def separate_plots(t):
    """
    Separate the image into two different images
    :param t: file name
    :return: NIL
    """
    a = Visual.image_open(t)
    a, b = Process.get_split(a)
    Visual.image_write(t.split('.')[0] + "_split_to_plot", a)
    Visual.image_write(t.split('.')[0] + "_split_to_num", b)
    c = Visual.get_overlay(a, b)
    Visual.image_write(t.split('.')[0] + "_overlay", c)

if __name__ == "__main__":
    title = ["Numbered.png", "Enhanced.png", "nums.jpg", "plot.jpg", "port.jpg", "see.jpg", "subsection.jpg",
             "testplot.jpg", "trial.jpg"]
    temps = ["Template/Symbols.png", "Template/SymbolSet.png", "Template/Crops"]

    make_subplots(title[4])
    # separate_plots(title[4])
