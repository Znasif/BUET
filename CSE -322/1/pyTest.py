import socket
import pickle

class SimpleClient:
    data= ""

    def convMe(self, my_str):
        return bytes(my_str, 'utf-8')

    def revMe(self, buf):
        return buf.decode('utf-8')

    def __init__(self):
            s=socket.socket()
            host=socket.gethostname()
            s.connect((host,5555))
            l= input("->")
            while(l):
                f=s.makefile('wb', 1024)
                pickle.dump(l,f,-1)
                if (l == "Close Server"):
                    s.close()
                    f.close()
                    break
                f.close()
                print(l)

                f=s.makefile('rb', 1024)
                msg=pickle.load(f)
                f.close()

                print("server -> "+msg)
                l = input("->")

def Main():
    sc = SimpleClient()

if __name__ == '__main__':
    Main()
