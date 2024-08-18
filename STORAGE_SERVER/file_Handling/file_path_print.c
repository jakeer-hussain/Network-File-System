#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

void printAccessStatus(mode_t mode) {
    printf("Read Access: %s\n", (mode & R_OK) ? "Yes" : "No");
    printf("Write Access: %s\n", (mode & W_OK) ? "Yes" : "No");
}

void listFilesRecursively(const char *basePath) {
    char path[1000];
    struct dirent *entry;
    struct stat info;

    DIR *dir = opendir(basePath);

    // Check if the directory is opened successfully
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    // Read the directory entries
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(path, sizeof(path), "%s/%s", basePath, entry->d_name);

            // Get information about the file
            if (stat(path, &info) != 0) {
                perror("Error getting file information");
            } else {
                // Print the file or directory information
                printf("Path: %s\n", path);
                printf("Permissions: %o\n", info.st_mode & 0777);
                printf("Type: %s\n", (S_ISDIR(info.st_mode) ? "Directory" : "File"));

                // Check and print read and write access
                printf("Access:\n");
                printAccessStatus(info.st_mode);

                printf("\n");
            }

            // If it's a directory, recursively list its contents
            if (S_ISDIR(info.st_mode)) {
                listFilesRecursively(path);
            }
        }
    }

    // Close the directory
    closedir(dir);
}

int main() {
    char buffer[1024];
    getcwd(buffer, sizeof(buffer));
    const char *directoryPath = buffer; // Change this to the desired directory path

    // Print the files and directories in the directory recursively
    listFilesRecursively(directoryPath);

    return 0;
}
