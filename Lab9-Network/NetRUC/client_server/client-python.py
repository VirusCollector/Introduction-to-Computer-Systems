###############################################################################
# client-python.py
# Name: 
# NetId:  
###############################################################################

import sys
import socket

SEND_BUFFER_SIZE = 2048

def client(server_ip, server_port):
    ClientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        server_addr = (server_ip, int(server_port))
        ClientSocket.connect(server_addr)
        while True:
            SendBuffer = sys.stdin.buffer.read(SEND_BUFFER_SIZE)
            if not SendBuffer:
                break
            ClientSocket.send(SendBuffer)
    except socket.error as e:
        print("Connect Failed!", e)
        ClientSocket.close()
        return -1
    finally:
        ClientSocket.close()
    pass


def main():
    """Parse command-line arguments and call client function """
    if len(sys.argv) != 3:
        sys.exit("Usage: python client-python.py [Server IP] [Server Port] < [message]")
    server_ip = sys.argv[1]
    server_port = int(sys.argv[2])
    client(server_ip, server_port)

if __name__ == "__main__":
    main()
