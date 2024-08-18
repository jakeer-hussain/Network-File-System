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

int hash(const char *str)
{
    int hashValue = 0;
    while (*str)
    {
        hashValue = (hashValue * 31) + *str++;
    }
    return hashValue % MAX_DIR;
}

struct TrieNode *createNode(const char *name, int port_num, int accessible)
{
    struct TrieNode *newNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (newNode)
    {
        strncpy(newNode->name, name, sizeof(newNode->name) - 1);
        newNode->name[sizeof(newNode->name) - 1] = '\0';
        newNode->port_num = port_num;
        newNode->accessible = accessible;
        for (int i = 0; i < MAX_DIR; i++)
        {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

void insert(struct TrieNode *root, const char *path, int port_num, int accessible)
{
    struct TrieNode *current = root;
    char *dummy = strdup(path);
    char *token = strtok(dummy, "/");
    accessible = -accessible;
    while (token != NULL)
    {
        int index = hash(token);
        // Linear probing to find the next available index
        while (current->children[index] != NULL)
        {
            if (strncmp(current->children[index]->name, token, strlen(current->children[index]->name)) == 0)
            {
                break;
            }
            index = (index + 1) % MAX_DIR;
        }
        if (!current->children[index])
        {
            current->children[index] = createNode(token, port_num, accessible);
        }
        current = current->children[index];
        token = strtok(NULL, "/");
    }
    current->accessible = -accessible;

    // Free the memory allocated for dummy
    free(dummy);
}

// Function to search for a file or directory path in the trie
bool search(struct TrieNode *root, const char *path)
{
    struct TrieNode *current = root;
    char *dummy = strdup(path);
    char *token = strtok(dummy, "/");
    while (token != NULL)
    {
        int index = hash(token);
        if (!current->children[index])
        {
            free(dummy);
            return false; // The path is not present in the trie
        }

        current = current->children[index];
        token = strtok(NULL, "/");
    }
    free(dummy);
    return current != NULL && current->accessible == ACCESIBLE; // Check if the end of the path is reached
}

bool deletePath(struct TrieNode *root, const char *path)
{
    struct TrieNode *current = root;
    char *dummy = strdup(path);
    char *token = strtok(dummy, "/");
    struct TrieNode *parent = NULL;
    int parent_index = -1;

    while (token != NULL)
    {
        int index = hash(token);
        // Linear probing to find the next available index
        while (current->children[index] != NULL)
        {
            if (strncmp(current->children[index]->name, token, strlen(current->children[index]->name)) == 0)
            {
                break;
            }
            index = (index + 1) % MAX_DIR;
        }

        if (!current->children[index])
        {
            free(dummy);
            return false; // The path is not present in the trie
        }

        parent = current;
        parent_index = index;
        current = current->children[index];
        token = strtok(NULL, "/");
    }


    if (current->accessible > 0)
    {
        free(current);
        parent->children[parent_index] = NULL;
        free(dummy);
        return true;
    }

    free(dummy);
    return false;
}

