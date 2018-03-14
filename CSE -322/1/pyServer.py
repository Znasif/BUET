import socket
from threading import Thread
import pickle
import serverUI
import os
import zipfile

class Server:

    def __init__(self):
        ss = socket.socket()
        shost = socket.gethostbyname(socket.gethostname())
        print(shost)
        port=5555
        self.myUI = serverUI.serverStage (self)
        ss.bind((shost,port))
        ap=listenThread(ss,self)
        ap.start()
        self.myUI.mainloop ()

    def configureServer(self, a, b, c, d, e, f):
        self.root=a
        self.ext=b
        self.sid=[]
        for i in range(len(c)):
            if "-" not in c[i]:
                self.sid.append([int(c[i]),[]])
            else:
                p,q=c[i].split("-")
                p,q=int(p),int(q)
                for j in range(p,q+1):
                    self.sid.append([j,[]])
        self.mnf=d
        self.mffb=e
        self.fileS=f
        print(self.root,self.ext,self.sid,self.mnf,self.mffb,self.fileS)

class serverThread(Thread):

    def __init__(self, sa, server):
        Thread.__init__(self)
        self.sa=sa
        self.server = server

    def checkUser(self, uid):
        p = len(self.server.sid)
        for i in range(p):
            if (uid[0] == str(self.server.sid[i][0])):
                if uid[1] in self.server.sid[i][1]:
                    return 2
                else:
                    self.server.sid[i][1].append(uid[1])
                    return 1
        return 0

    def run(self):
        try:
            f=self.sa.makefile('rb', 1024)
            data = pickle.load(f)
            f.close()
            print ("Server receives")

            a1 = self.checkUser(data)
            print(a1)
            if(a1==2):
                if(self.server.myUI.multiUser(data)):
                    a1 = 1
                else:
                    a1 = 0

            f = self.sa.makefile('wb', 512)
            pickle.dump(a1, f, -1)
            f.close()

            print ("Server sends")

            if not os.path.exists(self.server.root):
                os.makedirs(self.server.root)

            os.chdir(self.server.root)

            self.sendConfig(a1)

            while(a1):
                try:
                    f = self.sa.makefile('rb', 1024)
                    self.fileInfos = pickle.load(f)
                    f.close()
                    print ("Server receives")

                    fileName = self.fileInfos[0]
                    fileSize = self.fileInfos[1]
                    folder = self.fileInfos[2]

                    n = len(fileName)

                    for i in range(n):
                        if(os.path.isfile(fileName[i])):
                            if(self.server.myUI.sameFile(fileName[i])==False):
                                fileName[i]="new_"+str(i)+fileName[i]
                        f = open(fileName[i], 'wb')
                        data = self.sa.recv(512)
                        print ("Server receives")
                        totalRecv=len(data)
                        f.write(data)
                        self.sReply()
                        print(totalRecv,fileSize[i])
                        while totalRecv < fileSize[i]:
                            data = self.sa.recv(512)
                            print ("Server receives")
                            totalRecv += len(data)
                            f.write(data)
                            self.sReply()
                        f.close()
                        if(folder==1):
                            with zipfile.ZipFile ('Python.zip', "r") as z:
                                z.extractall (self.server.root)
                            os.remove('Python.zip')
                except:
                    print("Error in file reception")

        except:
            print("Error")

    def sendConfig(self, a1):
        if(a1==1):
            f = self.sa.makefile('wb', 512)
            pickle.dump([self.server.ext,self.server.mnf,self.server.mffb,self.server.fileS], f, -1)
            f.close()
            print ("Server sends")

    def sReply(self):
        try:
            msg=0
            f = self.sa.makefile('wb', 512)
            pickle.dump(msg, f, -1)
            f.close()
            print("Server sends "+str(msg))
        except:
            print("Error : sReply")
            return 0

class listenThread(Thread):

    def __init__(self, ss, server):
        Thread.__init__(self)
        self.ss=ss
        self.server=server

    def run(self):
        self.ss.listen ()
        while (True):
            sa, addr = self.ss.accept ()
            th = serverThread (sa, self.server)
            th.start ()
        # except:
        print ("Server socket error")

def Main():
    s = Server()

if __name__ == "__main__":
    Main()