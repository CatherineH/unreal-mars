__author__ = 'c2hollow'

from socket import *
import thread
from get_tile import get_raw, compress_data

BUFF = 1024
HOST = '127.0.0.1'
PORT = 8890


def response(key):
    return 'Recieved'


def handler(clientsock, addr):
    while 1:
        data = clientsock.recv(BUFF)
        print("received data: "+repr(data))
        loc = data.split("_")
        if len(loc) >= 2:
            x = int(loc[0])
            loc[1] = loc[1].split("\x00")[0]
            y = int(loc[1])
            return_data = "".join(compress_data(get_raw(x, y)))
            print("size of data sent: "+str(len(return_data)))
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

