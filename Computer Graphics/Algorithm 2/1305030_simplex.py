from scipy.optimize import linprog

c = [-1, 4]
A = [[-3, 1], [1, 2]]
b = [6, 4]
x0_bnds = (None, None)
x1_bnds = (-3, None)
res = linprog(c, A, b, bounds=[x0_bnds, x1_bnds])

print(res)
