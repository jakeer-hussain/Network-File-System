#include "headers.h"

int main()
{

    char *ip = "127.0.0.1";
    int port = 5000;        //naming server port

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    printf("TCP server socket created.\n");

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf("Connected to the server.\n");

    bzero(buffer, 1024);
    strcpy(buffer, "HELLO, THIS IS CLIENT.");           // This is the string we are sending
    printf("Client: %s\n", buffer);
    send(sock, buffer, strlen(buffer), 0);              // Data is sent to the Naming Server

    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("Server : %s\n", buffer);

    close(sock);
    printf("\nDisconnected from the server.\n");

    return 0;
}