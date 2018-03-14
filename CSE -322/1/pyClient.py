import socket
import clientUI
import sys
import pickle
import pyServer
import os


class client:
    def __init__(self):
        self.clientSocket = socket.socket ()
        self.chost = socket.gethostbyname (socket.gethostname ())
        self.myUI = clientUI.clientStage (self)
        self.myUI.showFrame (self.myUI.s)
        self.connected=0
        self.myUI.mainloop ()

    def sendFileS(self, fileInfos, folder):
        try:
            filePath, fileName, fileSize = fileInfos[0], fileInfos[1], fileInfos[2]
            f = self.clientSocket.makefile ('wb', 512)
            pickle.dump ([fileName, fileSize, folder], f, -1)
            f.close ()

            print ("Client sends")

            p = len (fileName)
            for i in range (p):
                os.chdir (filePath[i])
                print(filePath[i], fileName[i])
                with open (fileName[i], 'rb') as f:
                    bytesToSend = f.read (512)
                    self.clientSocket.send (bytesToSend)
                    print ("Client sends")
                    lenToSend = len(bytesToSend)
                    while (self.sReply () == 0):
                        self.clientSocket.send (bytesToSend)
                        print ("Client sends")
                    print (lenToSend, fileSize[i])
                    while lenToSend<fileSize[i]:
                        bytesToSend = f.read (512)
                        self.clientSocket.send (bytesToSend)
                        lenToSend+=len(bytesToSend)
                        print (lenToSend, fileSize[i])
                        while (self.sReply () == 0):
                            self.clientSocket.send (bytesToSend)
                            print ("Client sends")
            self.myUI.suCCess ()
            if(folder==1):
                os.remove("Python.zip")
        except:
            print ("Error : writeMe")

    def sReply(self):
        try:
            f = self.clientSocket.makefile ("rb", 512)
            msg = pickle.load (f)
            print ("Client receives "+str(msg))
            f.close ()
            if (msg == 0):
                return 1
            return 0
        except:
            print ("Error : sReply")
            return 0

    def setConstraint(self, data):
        self.conData = []
        for i in range (4):
            self.conData.append (data[i])
            # print(data[i])

    def userInfo(self, sid, sip, port):
        self.sid = sid
        self.sip = sip
        data = [sid, self.chost]
        try:
            if(self.connected==0):
                self.clientSocket.connect ((sip, int (port)))
                self.connected=1
            f = self.clientSocket.makefile ('wb', 512)
            pickle.dump (data, f, -1)
            f.close ()

            print ("Client sends")

            f = self.clientSocket.makefile ('rb', 512)
            data = pickle.load (f)
            f.close ()

            print ("Client receives")

            if (data == 0):
                self.myUI.warnUser ()
            else:
                f = self.clientSocket.makefile ('rb', 512)
                data = pickle.load (f)
                f.close ()

                print ("Client receives")

                self.setConstraint (data)
                self.myUI.showFrame (self.myUI.t)
        except:
            print("Error connecting to Server")


def Main():
    c = client ()


if __name__ == "__main__":
    Main ()
