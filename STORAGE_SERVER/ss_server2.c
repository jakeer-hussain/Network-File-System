#include "headers.h"

void *client_connect(void *arg)
{
    Connection_Details details;
    char Data[Kilo_byte * 2];
    details.port = 3001;
    strcpy(details.IP_address, "127.0.0.1");

    details = Establish_Connection_as_Server(details);
    printf("--- Established Connection ---\n");

    while(1)
    {
        details = Accept_Connection(details);
        printf("--- New Client Arrived ---\n");
        bzero(Data, Kilo_byte*2);
        recv(details.Comm_socket, Data, sizeof(Data), 0);
        printf("[ %d ( <- ) ::: %s \n]", details.port, Data);

        bzero(Data, Kilo_byte * 2);
        strcpy(Data, "hey hello");
        send(details.Comm_socket, Data, strlen(Data), 0);

        close(details.Client_socket);
    }
}
void *naming_server(void *arg)
{
    Connection_Details naming_server;
    naming_server.port = 4001;
    strcpy(naming_server.IP_address,"127.0.0.1");
    
    naming_server = Establish_Connection_as_Client(naming_server);
    
    char buffer[Kilo_byte];
    while (1)
    {
        bzero(buffer, 1024);
        strcpy(buffer, "HELLO Naming Server, this is Storage Server Acting as a client.");
        
        send(naming_server.Comm_socket, buffer, strlen(buffer), 0);
        bzero(buffer, 1024);
        recv(naming_server.Comm_socket, buffer, sizeof(buffer), 0);
        printf("[ %d ( <- ) ::: %s \n]", naming_server.port, buffer);
        
        if (strcmp(buffer, EXIT_CODE) == 0)     //exit
        {
            break;
        }
    }

    close(naming_server.Comm_socket);
    printf("Disconnected from the server.\n");
}
int main()
{

    pthread_t ns, client;
    pthread_create(&ns, NULL, naming_server, NULL);
    pthread_create(&client, NULL, client_connect, NULL);

    pthread_join(ns, NULL);
    pthread_join(client, NULL);
    return 0;
}