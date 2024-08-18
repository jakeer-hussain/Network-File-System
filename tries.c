#include "headers.h"

// A better hash function for a string with linear probing
int hash(const char *str)
{
    int hashValue = 0;
    while (*str)
    {
        hashValue = (hashValue * 31) + *str++;
    }
    return hashValue % MAX_DIR;
}

// Function to create a new trie node
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

// Function to insert a file or directory path into the trie using linear probing
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

// Function to delete a file or directory path from the trie using linear probing
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

// Driver program
int main()
{
    // Create the root of the trie
    // struct TrieNode *root = createNode("root", 0, ACCESIBLE);

    // // Insert some file and directory paths into the trie
    // insert(root, "documents/file1.txt", 1, ACCESIBLE);
    // insert(root, "documents/file2.txt", 2, NOT_ACCESIBLE);
    // insert(root, "documents/subfolder/file3.txt", 3, ACCESIBLE);
    // insert(root, "pictures/photo.jpg", 4, NOT_ACCESIBLE);
    // insert(root, "music/song1.mp3", 2, NOT_ACCESIBLE);

    // // Search for paths in the trie
    // printf("Search results:\n");
    // printf("Does \"documents/file1.txt\" exist? %s\n", search(root, "documents/file1.txt") ? "Yes" : "No");
    // printf("Does \"documents/file3.txt\" exist? %s\n", search(root, "documents/file3.txt") ? "Yes" : "No");
    // printf("Does \"documents/subfolder/file3.txt\" exist? %s\n", search(root, "documents/subfolder/file3.txt") ? "Yes" : "No");
    // printf("Does \"music/song1.mp3\" exist? %s\n", search(root, "music/song1.mp3") ? "Yes" : "No");

    // // Delete paths from the trie
    // if (deletePath(root, "documents/file1.txt"))
    // {
    //     printf("Path \"documents/file1.txt\" deleted successfully.\n");
    // }
    // else
    // {
    //     printf("Path \"documents/file1.txt\" not found in the trie.\n");
    // }
    // // Delete paths from the trie
    // if (deletePath(root, "documents/subfolder/file3.txt"))
    // {
    //     printf("Path \"documents/subfolder/file3.txt\" deleted successfully.\n");
    // }
    // else
    // {
    //     printf("Path \"documents/subfolder/file3.txt\" not found in the trie.\n");
    // }

    // // Search again after deletion
    // printf("\nSearch results after deletion:\n");
    // printf("Does \"documents/file1.txt\" exist? %s\n", search(root, "documents/file1.txt") ? "Yes" : "No");
    // printf("Does \"documents/file2.txt\" exist? %s\n", search(root, "documents/file2.txt") ? "Yes" : "No");
    // printf("Does \"documents/subfolder/file3.txt\" exist? %s\n", search(root, "documents/subfolder/file3.txt") ? "Yes" : "No");

    // Free the memory used by the trie
    // free(root);

    return 0;
}
