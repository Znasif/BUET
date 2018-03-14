with open("hdtt4note.txt") as f:
    lines=f.readlines()
    for l in lines:
        s=l.split()
        print(s[-1])
    f.close()

with open("hdtt4list.txt") as f:
    lines=f.readlines()
    print((lines[0].split())[2])
    print((lines[0].split())[6])
    for l in lines[-9:-6:1]:
        s=l.split()
        print(s[-3],s[-1])
    f.close()
