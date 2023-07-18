/*****************************************************************************
 * client-c.c                                                                 
 * Name:	
 * NetId:	
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#define SEND_BUFFER_SIZE 2048


/* TODO: client()
 * Open socket and send message from stdin.
 * Return 0 on success, non-zero on failure
*/
int client(char* server_ip, char* server_port)
{
    int ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
    char SendBuffer[SEND_BUFFER_SIZE];
    ssize_t BufferSize = 0;
    if (ClientSocket < 0)
    {
        perror("Failed to create socket");
        return -1;
    }
    struct sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(atoi(server_port));
    ServerAddr.sin_addr.s_addr = inet_addr(server_ip);
    if (connect(ClientSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)) < 0)
    {
        perror("Failed to connect to server");
        close(ClientSocket);
        return -1;
    }
    while ((BufferSize = read(STDIN_FILENO, SendBuffer, SEND_BUFFER_SIZE)) > 0)
    {
        if (send(ClientSocket, SendBuffer, BufferSize, 0) < 0)
        {
            perror("Failed to send message");
            close(ClientSocket);
            return -1;
        }
    }
    close(ClientSocket);
    return 0;
}

/*
 * main()
 * Parse command-line arguments and call client function
*/
int main(int argc, char **argv) 
{
  char *server_ip;
  char *server_port;

  if (argc != 3) 
  {
    fprintf(stderr, "Usage: ./client-c [server IP] [server port] < [message]\n");
    exit(EXIT_FAILURE);
  }

  server_ip = argv[1];
  server_port = argv[2];
  return client(server_ip, server_port);
}
