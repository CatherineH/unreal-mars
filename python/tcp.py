__author__ = 'c2hollow'

from socket import *
import thread
from get_tile import get_raw, get_raw_colors, compress_data

BUFF = 1024
HOST = '127.0.0.1'
PORT = 8890


def handler(clientsock, addr):
    while 1:
        data = clientsock.recv(BUFF)
        print("received data: "+repr(data))
        loc = data.split("_")

        if len(loc) >= 3:
            x = int(loc[1])
            loc[2] = loc[2].split("\x00")[0]
            y = int(loc[2])
            print(loc[0])
            if loc[0] == 'points':
                return_data = "".join(compress_data(get_raw(x, y)))
            elif loc[0] == "r":
                print("sending red data")
                print(len(get_raw_colors(x, y, 0)))
                return_data = "".join(get_raw_colors(x, y, 0))
            elif loc[0] == "g":
                print("sending blue data")
                return_data = "".join(get_raw_colors(x, y, 1))
            elif loc[0] == "b":
                print("sending green data")
                return_data = "".join(get_raw_colors(x, y, 2))
            print("size of data sent: "+str(len(return_data)))
            print("last value is: "+str(ord(return_data[-1])))
            clientsock.sendall(return_data)

        if not data: break

if __name__=='__main__':
    ADDR = (HOST, PORT)
    serversock = socket(AF_INET, SOCK_STREAM)
    serversock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    serversock.bind(ADDR)
    serversock.listen(5)
    while 1:
        print 'waiting for connection...'
        clientsock, addr = serversock.accept()
        print '...connected from:', addr
        thread.start_new_thread(handler, (clientsock, addr))

