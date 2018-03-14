import threading
import socket
import pickle


class TestServer:

    def __init__(self):
        id = 1
        try:
            ss = socket.socket()
            host = socket.gethostname()
            ss.bind((host, 5555))
            print("Server has been started successfully.")
            ss.listen(3)
            while (True):
                c, addr = ss.accept()
                print(list(addr)[0])
                wt = WorkerThread(c, id)
                id+=1
                wt.start()
        except:
            print("Problem in ServerSocket operation. Exiting main.")


class WorkerThread(threading.Thread):
    def __init__(self, soc, id):
        threading.Thread.__init__(self)
        self.id = id
        self.soc = soc

    def convMe(self, my_str):
        return bytes(my_str, 'utf-8')

    def revMe(self, buf):
        return buf.decode('utf-8')

    def run(self):
        print("Your id is: " + str(self.id))
        try:
            f=self.soc.makefile('rb', 1024)
            data = pickle.load(f)
            f.close()
            while(data):
                print(str(self.id)+"->"+data)
                data=data.upper()

                f=self.soc.makefile('wb', 1024)
                pickle.dump(data,f,-1)
                f.close()

                f = self.soc.makefile('rb', 1024)
                data=pickle.load(f)
                f.close()
                print(data)
                if(data== "Close Server"):
                    self.soc.close()
                    break
        except:
            print("Error")
        print("Client "+str(self.id)+" is Done")

def Main():
    pyServer=TestServer()

if __name__ == '__main__':
    Main()