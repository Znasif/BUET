import os
from tkinter import filedialog
import tkinter
import sys
root = tkinter.Tk()
s= filedialog.askdirectory()

#cwd = os.getcwd()
p="TestServer.java"

#if not os.path.exists(cwd+"\\"+s):
#    os.makedirs(cwd + "\\"+s)

if not os.path.exists(s):
    os.makedirs(s)

#with open(cwd+s) as f:
os.chdir(s)

cwd = os.getcwd()
print(os.path.getsize(p))
a = open(p,"rb")
b= a.read()
f = open("new"+p,"wb")
f.write(b)
if b!="QUIT":
    print("Not Quit!")
f = open("new"+p,"rb")
c = f.read()
print(sys.getsizeof(b))
print(sys.getsizeof(c))
print(os.path.getsize("new"+p))
print(cwd)
a=""
q=-1
n=len(cwd)
while(q!=-n-1 and str(cwd)[q]!="\\"):
    a+=str(cwd)[q]
    q-=1
a=str(cwd)[0:q]
print(a)