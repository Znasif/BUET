import tkinter as tk
import pyServer
import tkinter.messagebox as tkmsg
import os
from tkinter import filedialog

class serverStage(tk.Tk):

    def __init__(self, myServer):
        tk.Tk.__init__(self)
        self.place=os.getcwd()
        self.myServer=myServer
        self.title("Server")
        self.configureStart()

    def configureStart(self):
        l1 = tk.Label(self, text="Root Directory")
        l2 = tk.Label(self, text="Allowed extensions")
        l3 = tk.Label(self, text="SID List")
        l4 = tk.Label(self, text="Max Number of Files")
        l5 = tk.Label(self, text="Max File size in Bytes")
        e1 = tk.Button(self,text="Select Root Directory", command=self.selectRoot)
        e2 = tk.Entry(self)
        e3 = tk.Entry(self)
        e4 = tk.Entry(self)
        e5 = tk.Entry(self)
        i = tk.IntVar()
        c1 = tk.Checkbutton(self, text="Allow Folders", variable=i)
        b = tk.Button(self,text="Set Variables", command=lambda: self.dataSets(self.place,e2.get(),e3.get(),e4.get(),e5.get(),i.get()))
        l1.grid(row=0)
        l2.grid(row=1)
        l3.grid(row=2)
        l4.grid(row=3)
        l5.grid(row=4)
        e1.grid(row=0,column=1)
        e2.grid(row=1,column=1)
        e3.grid(row=2,column=1)
        e4.grid(row=3,column=1)
        e5.grid(row=4,column=1)
        c1.grid(columnspan=2)
        b.grid(columnspan=2)

    def dataSets(self, a, b, c, d, e, f):
        self.myServer.configureServer(a, b.split(","), c.split(","), int(d), int(e), f)
        self.varSet()

    def multiUser(self, uid):
        c= "User : "+str(uid[0])+" IP : "+str(uid[1])
        b = tkmsg.askyesno("Allow New User", c)
        print(b)
        return b

    def selectRoot(self):
        self.place =filedialog.askdirectory()

    def sameFile(self, fileName):
        return tkmsg.askyesno("Replace "+fileName+" ?")

    def varSet(self):
        tkmsg.showinfo("Success", "Server Successfully configured!")