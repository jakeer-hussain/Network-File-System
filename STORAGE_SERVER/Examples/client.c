#include "headers.h"

int main()
{
    Connection_Details details;
    details.port = 5000;
    strcpy(details.IP_address, "127.0.0.1");
    details = Establish_Connection_as_Client(details);
    printf("--- Established Connection with naming server---\n");

    printf("--------------------------------\n");
    char buffer[Kilo_byte];
    client_input Input;
    printf("Enter the Operation : ");
    scanf("%s", Input.Operation);
    printf("Enter the File Path : ");
    scanf("%s", Input.path);
    printf("--------------------------------\n");
    ptrinput pointer = &Input;
    send(details.Comm_socket, pointer, sizeof(Input), 0);

    int buf;
    int *point = &buf;
    recv(details.Comm_socket, point, sizeof(int), 0);
    printf("[ %d ( <- ) ::: %d \n]", details.port, buf);
    close(details.Comm_socket);
    printf("Disconnected from the naming server.\n");

    if (strcmp(Input.Operation, "READ") == 0 || strcmp(Input.Operation, "WRITE") == 0)
    {
        Connection_Details details2;
        // details2.port = atoi(buffer);
        details2.port = 3000;
        strcpy(details2.IP_address, "127.0.0.1");
        details2 = Establish_Connection_as_Client(details2);
        printf("--- Established Connection with storage server---\n");

        bzero(buffer, 1024);
        strcpy(buffer, "HELLO Storage Server, this is client.");
        send(details2.Comm_socket, buffer, strlen(buffer), 0);

        bzero(buffer, 1024);
        recv(details2.Comm_socket, buffer, sizeof(buffer), 0);
        printf("[ %d ( <- ) ::: %s \n]", details2.port, buffer);

        close(details2.Comm_socket);
        printf("Disconnected from the storage server.\n");
    }
    return 0;
}