//create newfile or newfolder in a directory
// #include <stdio.h>
// #include<stdlib.h>
// #include <sys/stat.h>
// #include<string.h>
// #include <unistd.h>
// #include <fcntl.h>
// int main()
// {
//     char filename[100] = {'\0'};
//     char buffer[100] = {'\0'};
//     fgets(filename, sizeof(filename), stdin);
//     filename[strcspn(filename, "\n")] = '\0';
//     fgets(buffer, sizeof(buffer), stdin);
//     buffer[strcspn(buffer, "\n")] = '\0';
//     char path[200];
//     snprintf(path, sizeof(path), "%s/%s", buffer, filename);
//     // int file_descriptor = creat(path, 0644);
//     // close(file_descriptor);
//     // mkdir(path,0755);
//     return 0;
// }





// write to a file
// #include <stdio.h>
// #include<stdlib.h>
// #include<string.h>
// #include <unistd.h>
// #include <fcntl.h>
// void writetofile(char* filename,char* buffer){
//     FILE* file;
//     file = fopen(filename,"w");
//     if(file == NULL){
//         printf("Unable to open file\n");
//         return;
//     }
//     fprintf(file,"%s",buffer);
//     fclose(file);
//     printf("Data has been written to the file.\n");
//     return;
// }
// int main()
// {
//     char filename[100] = {'\0'};
//     char buffer[1024] = {'\0'};
//     fgets(filename, sizeof(filename), stdin);
//     filename[strcspn(filename, "\n")] = '\0';
//     fgets(buffer, sizeof(buffer), stdin);
//     buffer[strcspn(buffer, "\n")] = '\0';
//     writetofile(filename,buffer);
//     return 0;
// }


// delete a file 
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>

// int main() {
//     const char *filename = "/home/jakeer/Videos/c.txt";  // Replace with the name of the file you want to delete

//     if (remove(filename) == 0) {   // remove for file & rmdir for directory
//         printf("File deleted successfully.\n");
//     } else {
//         perror("Error deleting the file");
//         return 1;
//     }

//     return 0;
// }


// printing all files and folders of a directory
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <dirent.h>

// void listFilesAndFolders(const char *dirpath) {
//     DIR *dir;
//     struct dirent *entry;

//     dir = opendir(dirpath);

//     if (dir == NULL) {
//         perror("Error opening the directory");
//         return;
//     }
//     char **fileNames = NULL;
//     int numFiles = 0;
//     while ((entry = readdir(dir)) != NULL) {
//         if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
//             printf("%s%s\n", prefix, entry->d_name);

//             // Check if it's a directory (folder) and recursively list its contents
//             if (entry->d_type == DT_DIR) {
//                 char path[PATH_MAX];
//                 snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);
//                 listFilesAndFolders(path, strcat(prefix, "  "));  // Indent subfolder contents
//             }
//         }
//     }

//     closedir(dir);
// }

// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         printf("Usage: %s <directory>\n", argv[0]);
//         return 1;
//     }

//     const char *dirname = argv[1];
//     listFilesAndFolders(dirname);

//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

void printFileDetails(struct dirent *entry, const char *dirname) {
    struct stat fileStat;
    char path[1024];

    snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

    if (stat(path, &fileStat) < 0) {
        perror("Error getting file details");
        return;
    }

    struct passwd *pw = getpwuid(fileStat.st_uid);
    struct group *gr = getgrgid(fileStat.st_gid);

    // printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    // printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    // printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    // printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    // printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    // printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    // printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    // printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    // printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    // printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf(" %ld", (long)fileStat.st_nlink);
    printf(" %s %s", pw->pw_name, gr->gr_name);
    // printf(" %5ld", (long)fileStat.st_size);

    // char timestamp[20];
    // strftime(timestamp, sizeof(timestamp), "%b %d %H:%M", localtime(&fileStat.st_mtime));
    // printf(" %s", timestamp);

    printf(" %s\n", entry->d_name);
}

void listFilesAndFolders(const char *dirname) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dirname);

    if (dir == NULL) {
        perror("Error opening the directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printFileDetails(entry, dirname);
        }
    }

    closedir(dir);
}

int main() {

    char buffer[100];
    getcwd(buffer, sizeof(buffer));
    const char *dirname = buffer;
    listFilesAndFolders(dirname);

    return 0;
}
