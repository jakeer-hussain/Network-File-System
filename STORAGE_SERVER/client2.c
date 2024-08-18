#include "headers.h"

void print(getfiles files_array, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%s\n", files_array[i].file_path);
    }
}
int main()
{

    printfiles files_array[MAX_FILES_DATA];
    getfiles pointer_files = files_array;

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

    if (strcmp(Input.Operation, "READ") == 0 || strcmp(Input.Operation, "WRITE") == 0)
    {
        printf("Came Under here : \n");
        int *point = &buf;
        recv(details.Comm_socket, point, sizeof(int), 0);
        printf("[ %d ( <- ) ::: %d \n]", details.port, buf);
        close(details.Comm_socket);
        printf("Disconnected from the naming server.\n");
    }

    else if (strcmp(Input.Operation, "CREATE") == 0 || strcmp(Input.Operation, "DELETE") == 0)
    {
        char bu[100];
        bzero(bu, sizeof(bu));
        recv(details.Comm_socket, bu, sizeof(bu), 0);
        printf("----\n");
        printf("%s\n", bu);
        printf("Disconnected from the naming server.\n");
    }

    else if (strcmp(Input.Operation, "GET") == 0)
    {
        int size;
        int *ptr_size = &size;
        recv(details.Comm_socket, ptr_size, sizeof(size), 0);
        send(details.Comm_socket, "Hello", 5, 0);
        recv(details.Comm_socket, pointer_files, sizeof(files_array) * MAX_FILES_DATA, 0);
        print(files_array, size);
        close(details.Comm_socket);
        printf("Disconnected from the naming server.\n");
    }

    else if(strcmp(Input.Operation, "COPY") == 0)
    {
        char buff[Kilo_byte] = {'\0'};
        printf("Enter the Source Path and Destination Path: ");
        scanf("%s",buff);
        char buff2[3] = {'\0'};
        recv(details.Comm_socket, buff2, sizeof(buff2), 0);
        send(details.Comm_socket, buff, strlen(buff), 0);

    }

    // This second half is Code for the communication with the Storage server

    if (strcmp(Input.Operation, "READ") == 0 || strcmp(Input.Operation, "WRITE") == 0)
    {
        Connection_Details details2;
        // details2.port = atoi(buffer);
        details2.port = buf;
        strcpy(details2.IP_address, "127.0.0.1");
        details2 = Establish_Connection_as_Client(details2);
        printf("--- Established Connection with storage server---\n");

        bzero(buffer, 1024);
        strcpy(buffer, Input.Operation);
        strcat(buffer, "#");
        strcat(buffer, Input.path);

        if (strcmp(Input.Operation, "WRITE") == 0)
        {
            strcat(buffer, "#");

            char mini_buffer[Kilo_byte * 10];
            printf("Now You are connected to the appropriate storage server\n");
            printf("Enter the Data What you want to write without spaces : \n");
            printf("----------------------------------------------\n");
            scanf("%s", mini_buffer);
            printf("----------------------------------------------\n");
            strcat(buffer, mini_buffer);
        }
        send(details2.Comm_socket, buffer, strlen(buffer), 0);

        bzero(buffer, 1024);
        recv(details2.Comm_socket, buffer, sizeof(buffer), 0);
        printf("[ %d ( <- ) ::: %s \n]", details2.port, buffer);

        close(details2.Comm_socket);
        printf("Disconnected from the storage server.\n");
    }
    return 0;
}