#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>

// Here we are considering read and write case where client has to send message to naming server and then he needs to connect
// with the storage server on the basis of PORT number that is been sent by the Naming Server

int main()
{
    char *ip = "127.0.0.1";
    int port = 5000;            //Naming Sever port
    int port2;                  // We are getting this from the NS

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    printf(":::5000::: TCP server socket created.\n");

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf(":::5000::: Connected to the Naming server.\n");

    bzero(buffer, 1024);
    strcpy(buffer, ":::5000::: HELLO NS, THIS IS CLIENT.");           // This is the string we are sending
    // sleep(2);
    send(sock, buffer, strlen(buffer), 0);              // Data is sent to the Naming Server
    printf(":::5000:::: [->]Client: %s\n", buffer);

    bzero(buffer, 1024);
    // sleep(2);
    recv(sock, buffer, sizeof(buffer), 0);
    printf(":::5000::: [<-]recieved data from naming server: %s\n",buffer);
    port2 = 3000;

    close(sock);    
    printf("\n:::5000::: Disconnected from the Naming server.\n");

    struct sockaddr_in addr2;               //new address
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    printf(":::3000::: TCP new socket is created");

    memset(&addr2, '\0', sizeof(addr2));
    addr2.sin_family = AF_INET;
    addr2.sin_port = port2;
    addr2.sin_addr.s_addr = inet_addr(ip);
    
    connect(sock, (struct sockaddr *)&addr2, sizeof(addr2));
    printf(":::3000::: Connected to the Storage Server.\n");

    strcpy(buffer, ":::3000::: HELLO SS, THIS IS CLIENT.");           // This is the string we are sending
    // sleep(2);
    send(sock, buffer, strlen(buffer), 0);  
    printf(":::3000::: [->]Client: %s\n", buffer);
    
    bzero(buffer, 1024);
    // sleep(2);
    recv(sock, buffer, sizeof(buffer), 0);
    printf(":::3000::: [<-]Client: %s\n", buffer);

    close(sock);
    printf("\n:::3000::: Disconnected from the Storage server.\n");

    return 0;        
}