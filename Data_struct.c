#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 128

// Structure to represent a file entry
struct FileEntry {
    int portNumber;
    int permissions;
};

// Trie node structure
struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    struct FileEntry* fileEntry;
};

// Function to create a new file entry
struct FileEntry* createFileEntry(int portNumber, int permissions) {
    struct FileEntry* newEntry = (struct FileEntry*)malloc(sizeof(struct FileEntry));
    if (newEntry != NULL) {
        newEntry->portNumber = portNumber;
        newEntry->permissions = permissions;
    }
    return newEntry;
}

// Function to create a new trie node
struct TrieNode* createTrieNode() {
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (newNode != NULL) {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            newNode->children[i] = NULL;
        }
        newNode->fileEntry = NULL;
    }
    return newNode;
}

// Function to insert a file entry into the trie
void insert(struct TrieNode* root, const char* filePath, int portNumber, int permissions) {
    struct TrieNode* current = root;
    for (int i = 0; filePath[i] != '\0'; ++i) {
        int index = (int)filePath[i];
        if (current->children[index] == NULL) {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
        current->fileEntry = createFileEntry(portNumber, permissions);
    }

    // if (current->fileEntry == NULL) {
    // }
}

// Function to search for a file entry in the trie
struct FileEntry* search(struct TrieNode* root, const char* filePath) {
    struct TrieNode* current = root;
    for (int i = 0; filePath[i] != '\0'; ++i) {
        int index = (int)filePath[i];
        if (current->children[index] == NULL) {
            return NULL; // File path not found
        }
        current = current->children[index];
    }

    return current->fileEntry;
}

// Function to delete a file entry from the trie
void delete(struct TrieNode* root, const char* filePath) {
    struct TrieNode* current = root;
    struct TrieNode* parent = NULL;

    for (int i = 0; filePath[i] != '\0'; ++i) {
        int index = (int)filePath[i];
        if (current->children[index] == NULL) {
            return; // File path not found
        }
        parent = current;
        current = current->children[index];
    }

    // Delete the file entry
    free(current->fileEntry);
    current->fileEntry = NULL;

    // Perform cleanup (remove unnecessary nodes)
    if (parent != NULL) {
        int index = (int)filePath[strlen(filePath) - 1];
        parent->children[index] = NULL;
    }
}

// Function to print the contents of the trie
void print(struct TrieNode* root, char* currentPath, int level) {
    // if(root->children[35] == '#')
    // {
    //     printf("File: %s, Port: %d, Permissions: %d\n", currentPath, root->fileEntry->portNumber, root->fileEntry->permissions);
    // }

    if (root->fileEntry != NULL) {
        currentPath[level] = '\0';
    }

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (root->children[i] != NULL) {
            if(i == 35)
            {
                printf("File: %s, Port: %d, Permissions: %d\n", currentPath, root->fileEntry->portNumber, root->fileEntry->permissions);
            }
            currentPath[level] = (char)i;
            print(root->children[i], currentPath, level + 1);
        }
    }
}

// Function to free the memory used by the trie
void freeTrie(struct TrieNode* root) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        freeTrie(root->children[i]);
    }

    free(root->fileEntry);
    free(root);
}

int main() {
    // Example usage
    struct TrieNode* trieRoot = createTrieNode();

    insert(trieRoot, "/balu/a1.txt#", 8080, 755);
    insert(trieRoot, "/bunny/a2.txt#", 8080, 755);
    insert(trieRoot, "/balu/b1.txt#", 8082, 700);

    printf("Initial Trie:\n");
    char currentPath[100];
    print(trieRoot, currentPath, 0);

    const char* searchPath = "/b";
    struct FileEntry* result = search(trieRoot, searchPath);
    if (result != NULL) {
        printf("\nSearch result for %s:\n", searchPath);
        printf("File: %s, Port: %d, Permissions: %d\n\n", searchPath, result->portNumber, result->permissions);
    } else {
        printf("\nFile %s not found.\n", searchPath);
    }
    
    print(trieRoot, currentPath, 0);

    freeTrie(trieRoot);

    return 0;
}
