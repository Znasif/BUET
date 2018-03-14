from tkinter import *
from tkinter import filedialog
import tkinter.messagebox as tkmsg
class clientUI(Tk):

    #def deliverToServer(self):

    def printPort(self):
        print(self.e2.get())

    def __init__(self):
        Tk.__init__(self)
        self.title("Client")
        self.a = Frame()
        self.a.pack(side="top", fill="both", expand=True)
        self.a.grid_rowconfigure(0, weight=1)
        self.a.grid_columnconfigure(0, weight=1)
        self.startSc()
        self.mainloop()

    def startSc(self):
        self.s = Frame(self.a)
        s1 = Label(self.s, text="Open")
        b1 = Button(self.s, text="Select", command=self.callFile)
        s1.pack()
        b1.pack()
        self.s.pack()
    def callFile(self):
        print(filedialog.askopenfilename())
    def storeFrame(self):
        print("Here")
        self.t=Frame(self.a)
        self.l1 = Label(self.t, text="Student ID ")
        self.l2 = Label(self.t, text="Port Number")
        self.e1 = Entry(self.t)
        self.e2 = Entry(self.t)
        self.b1 = Button(self.t, text="Login", command=self.printPort)
        self.c1 = Checkbutton(self.t, text="Keep me logged in")
        #self.d1 = filedialog.askopenfilenames()
        self.l1.grid(row=0)
        self.l2.grid(row=1)
        self.e1.grid(row=0, column=1)
        self.e2.grid(row=1, column=1)
        self.b1.grid(columnspan=2)
        self.c1.grid(columnspan=2)
        self.t.pack()
        #self.t.tkraise()
        print("Ereh")

a=clientUI()