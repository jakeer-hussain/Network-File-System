#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#define PATH_MAX 270
void deleteFolder(const char *folderPath);

int main() {
    char input[100];

    // Get user input
    printf("Enter the folder name: ");
    scanf("%s", input);
int len = strlen(input);
    if (len >= 4 && strcmp(input + len - 4, ".txt") == 0) {
        // Delete the file
        if (remove(input) == 0) {
            printf("File deleted successfully.\n");
        } else {
            printf("Unable to delete the file.\n");
        }
    } 
    // Delete the folder and its contents
    else{    
    	deleteFolder(input);
    }

    return 0;
}

void deleteFolder(const char *folderPath) {
    DIR *dir;
    struct dirent *entry;
    struct stat statBuf;

    // Open the directory
    dir = opendir(folderPath);

    if (dir == NULL) {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    // Read the directory
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            // Create the full path of the entry
            char entryPath[PATH_MAX];
            snprintf(entryPath, sizeof(entryPath), "%s/%s", folderPath, entry->d_name);

            // Get information about the entry
            if (lstat(entryPath, &statBuf) == -1) {
                perror("Unable to get file status");
                closedir(dir);
                exit(EXIT_FAILURE);
            }

            // Recursively delete subdirectories
            if (S_ISDIR(statBuf.st_mode)) {
                deleteFolder(entryPath);
            } else {
                // Delete files
                if (remove(entryPath) != 0) {
                    perror("Unable to delete file");
                    closedir(dir);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    // Close the directory
    closedir(dir);

    // Delete the empty folder
    if (rmdir(folderPath) != 0) {
        perror("Unable to delete folder");
        exit(EXIT_FAILURE);
    }
}

