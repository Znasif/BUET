import numpy as np
temp = np.eye(4)
with open("matrix.txt", 'w') as f:
    n = int(input())
    for i in range(n):
        a = np.random.rand(4, 4)*10
        print(np.matmul(temp, a))
        temp = a
        b = ""
        for j in a:
            for k in j:
                b += str(k)+" "
            b += '\n'
        f.writelines(b)
    f.close()
