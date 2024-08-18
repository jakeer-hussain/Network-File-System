#include "headers.h"

Connection_Details Establish_Connection_as_Server (Connection_Details Details)
{
    struct sockaddr_in server_addr;
    char buffer[Kilo_byte];
    Details.Server_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = Details.port;
    server_addr.sin_addr.s_addr = inet_addr(Details.IP_address);

    bind(Details.Server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(Details.Server_socket, 1);

    return Details;
}

Connection_Details Accept_Connection (Connection_Details Details)
{
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);
    Details.Client_socket = accept (Details.Server_socket, (struct sockaddr *) &client_addr, &addr_size);
    Details.Comm_socket = Details.Client_socket;
    return Details;
}

Connection_Details Establish_Connection_as_Client (Connection_Details Details)
{
    int port = Details.port;
    char *ip = Details.IP_address;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[Kilo_byte];

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    Details.Client_socket = sock;
    Details.Comm_socket = sock;

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));

    return Details;
}

