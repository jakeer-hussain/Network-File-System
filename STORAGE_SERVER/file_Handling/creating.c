#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main() {
    char path[200] = {'\0'};
    
    // User input for the path
    printf("Enter a path: ");
    fgets(path, sizeof(path), stdin);
    path[strcspn(path, "\n")] = '\0'; // Remove the newline character

    // Get the length of the path
    size_t path_length = strlen(path);

    // Check if the path ends with ".txt"
    if (path_length >= 4 && strcmp(path + path_length - 4, ".txt") == 0) {
        // Create a file
        FILE* file = fopen(path, "w");
        if (file != NULL) {
            printf("File created successfully.\n");
            fclose(file);
        } else {
            fprintf(stderr, "Error creating file.\n");
        }
    } else {
        // Create a folder
        int status = mkdir(path, 0755); // Set folder permissions as needed
        if (status == 0) {
            printf("Folder created successfully.\n");
        } else {
            fprintf(stderr, "Error creating folder.\n");
        }
    }

    return 0;
}
