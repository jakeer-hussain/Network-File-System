#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 3) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept a connection from a client
    int client_addr_len = sizeof(client_addr);
    if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&client_addr_len)) == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Set timeout for recv using select
    fd_set read_fds;
    struct timeval timeout;
    FD_ZERO(&read_fds);
    FD_SET(client_socket, &read_fds);

    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    int ready = select(client_socket + 1, &read_fds, NULL, NULL, &timeout);

    if (ready == -1) {
        perror("Select failed");
        exit(EXIT_FAILURE);
    } else if (ready == 0) {
        // No response within 5 seconds
        printf("No Response: Dead\n");
    } else {
        // Receive data from the client
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            perror("Recv failed");
            exit(EXIT_FAILURE);
        }

        printf("Received from client: %s\n", buffer);
    }

    // Close the sockets
    close(client_socket);
    close(server_socket);

    return 0;
}
