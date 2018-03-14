import tkinter as tk
import pyClient
import tkinter.messagebox as tkmsg
from tkinter import filedialog
import os
import zipfile

class clientStage(tk.Tk):

    def __init__(self, myClient):
        tk.Tk.__init__(self)
        self.myClient=myClient
        self.title("Client")
        self.scene=tk.Frame(self)
        self.scene.pack(side="top", fill="both", expand=True)
        self.scene.grid_rowconfigure(0, weight=1)
        self.scene.grid_columnconfigure(0, weight=1)
        self.s = tk.Frame(self.scene)
        self.t = tk.Frame(self.scene)
        self.sceneStart()
        self.selectStart()


    def showFrame(self, p):
        p.tkraise()

    def sceneStart(self):
        l1 = tk.Label(self.s, text="Student ID")
        l2 = tk.Label(self.s, text="IP Address")
        l3 = tk.Label(self.s, text="Port")
        e1 = tk.Entry(self.s)
        e2 = tk.Entry(self.s)
        e3 = tk.Entry(self.s)
        b = tk.Button(self.s, text="Send Request", command= lambda: self.userInfo(e1.get(),e2.get(),e3.get()))
        l1.grid(row=0, column=0)
        l2.grid(row=1, column=0)
        l3.grid(row=2, column=0)
        e1.grid(row=0, column=1)
        e2.grid(row=1, column=1)
        e3.grid(row=2, column=1)
        b.grid(columnspan=2)
        self.s.grid(row=0, column=0, sticky="nsew")

    def userInfo(self, a, b, c):
        self.myClient.userInfo(a, b, c)

    def selectStart(self):
        l1 = tk.Label(self.t, text="Select")
        b1 = tk.Button(self.t, text="Select File(s)", command=self.selectFile)
        b2 = tk.Button(self.t, text="Select Folder", command=self.selectFolder)
        l1.grid(row=0)
        b1.grid(row=0, column=1)
        b2.grid(row=0, column=2)
        self.t.grid (row=0, column=0, sticky="nsew")

    def selectFile(self):
        try:
            flag=0
            if(self.myClient.conData[1]>1):
                self.l = list(filedialog.askopenfilenames())
                n = len(self.l)
                if(self.myClient.conData[1]<n):
                    self.warnFile()
                    n=0
                self.sizeS=[0]*n
                for i in range(n):
                    self.sizeS[i]=os.path.getsize(self.l[i])
                    if(self.sizeS[i]>self.myClient.conData[2]):
                        self.warnFile()
                        break
                    for j in range(len(self.myClient.conData[0])):
                        if(str(self.l[i]).endswith(str(self.myClient.conData[0][j]))):
                            flag = 1
                            break
                    if(flag==0):
                        self.warnFile()
                        break
            else:
                self.l = filedialog.askopenfilename()
                self.sizeS=os.path.getsize(self.l)
                if (self.sizeS > self.myClient.conData[2]):
                    self.warnFile()
                    print("1")
                for j in range(len(self.myClient.conData[0])):
                    if (str(self.l).endswith(str(self.myClient.conData[0][j]))):
                        flag = 1
                        self.l=[self.l]
                        self.sizeS=[self.sizeS]
                        break
                if (flag == 0):
                    self.warnFile()
            if(flag==1):
                n=len(self.l)
                self.nameS=[""]*n
                for i in range(n):
                    self.l[i],self.nameS[i]=os.path.split(self.l[i])
                #print([self.l,self.nameS,self.sizeS])
                self.myClient.sendFileS([self.l,self.nameS,self.sizeS], 0)
        except:
            print("Error in SelectFiles")


    def selectFolder(self):
        try:
            self.l = filedialog.askdirectory()
            self.sizeS = 0
            flag = 0
            if(self.myClient.conData[3]==0):
                self.warnFile()
                return
            zipf = zipfile.ZipFile ('Python.zip', 'w', zipfile.ZIP_DEFLATED)
            num=0
            for root, dirs, files in os.walk (self.l):
                for file in files:
                    num+=1
                    self.sizeS+=os.path.getsize(self.l+"\\"+file)
                    if (self.sizeS > self.myClient.conData[2] or self.myClient.conData[1]<num):
                        self.warnFile ()
                        break
                    else:
                        for j in range (len (self.myClient.conData[0])):
                            if (str (str(file)).endswith (str (self.myClient.conData[0][j]))):
                                flag = 1
                                zipf.write (os.path.join (root, file),os.path.relpath(os.path.join(root, file), os.path.join(self.l, '..')))
                                break
                        if (flag == 0):
                            self.warnFile ()
                            break
                if(flag==0):
                    zipf.close()
                    os.remove ("Python.zip")
                    break
                else:
                    zipf.close()
                    self.l=os.path.dirname(os.path.realpath(__file__))
                    self.nameS = ["Python.zip"]
                    self.sizeS=[ os.path.getsize(self.l+"\\Python.zip") ]
                    self.l=[self.l]
                    # print([self.l,self.nameS,self.sizeS])
                    self.myClient.sendFileS ([self.l, self.nameS, self.sizeS], 1)
        except:
            print("Error in Folder Select")

    def warnFile(self):
        tkmsg.showerror("Error", "The file you have selected does not conform with server specifications")
        self.showFrame(self.t)

    def warnUser(self):
        tkmsg.showerror("Error", "The server does not recognise your credential")
        self.showFrame(self.s)

    def suCCess(self):
        tkmsg.showinfo("Upload Successful", "Server confirmed receipt of uploaded File")