###############################################################################
# server-python.py
# Name: 
# NetId:  
###############################################################################

import sys
import socket

RECV_BUFFER_SIZE = 2048
QUEUE_LENGTH = 10

def server(server_port):
    ServerSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    ServerSocket.bind(('', int(server_port)))
    ServerSocket.listen(QUEUE_LENGTH)
    while True:
        ClientSocket, ClientAddr = ServerSocket.accept()
        while True:
            RecvBuffer = ClientSocket.recv(RECV_BUFFER_SIZE)
            if not RecvBuffer:
                break
            sys.stdout.buffer.write(RecvBuffer)
            sys.stdout.flush()
        ClientSocket.close()
    ServerSocket.close()
    pass

def main():
    """Parse command-line argument and call server function """
    if len(sys.argv) != 2:
        sys.exit("Usage: python server-python.py [Server Port]")
    server_port = int(sys.argv[1])
    server(server_port)

if __name__ == "__main__":
    main()
