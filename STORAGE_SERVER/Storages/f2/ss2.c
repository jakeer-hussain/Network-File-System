#include "headers.h"
serverfiles file_array[MAX_FILES_IN_SS];
storage_files Ptr_Files = file_array;

char* Reading (char* path)
{
    // printf("@@@@   %s    @@@@\n", path);
    char *read_text = (char*) malloc (sizeof(char) * 1000);

    int file_descriptor = open(path, O_RDONLY);

    read(file_descriptor, read_text, 1000 );

    printf("Data Going To send : %s\n", read_text);

    return read_text;
}

void writing (char* path, char* text)
{
    // printf("@@@   %s   |||    %s    @@@", path, text);
    int file_descriptor = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    
    write(file_descriptor, text, strlen(text));

    close(file_descriptor);
}

void creation(char* token){

    size_t path_length = strlen(token);

    // Check if the path ends with ".txt"
    if (path_length >= 4 && strcmp(token + path_length - 4, ".txt") == 0) {
        // Create a file
        FILE* file = fopen(token, "w");
        if (file != NULL) {
            printf("File created successfully.\n");
            fclose(file);
        } else {
            fprintf(stderr, "Error creating file.\n");
        }
    } else {
        // Create a folder
        int status = mkdir(token, 0755); // Set folder permissions as needed
        if (status == 0) {
            printf("Folder created successfully.\n");
        } else {
            fprintf(stderr, "Error creating folder.\n");
        }
    }

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
            char entryPath[MAX_PATH];
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
void deletion (char* token)
{
    printf("\n\nDELETION DONE : %s\n\n", token);
    int len = strlen(token);
    if (len >= 4 && strcmp(token + len - 4, ".txt") == 0) {
        // Delete the file
        if (remove(token) == 0) {
            printf("File deleted successfully.\n");
        } else {
            printf("Unable to delete the file.\n");
        }
    } 
    // Delete the folder and its contents
    else{    
    	deleteFolder(token);
    }
}

void *client_connect(void *arg)
{
    Connection_Details details;
    char Data[Kilo_byte * 2];
    details.port = 3000;
    strcpy(details.IP_address, "127.0.0.1");

    details = Establish_Connection_as_Server(details);
    printf("--- Established Connection ---\n");

    while (1)
    {
        char response[1000];
        details = Accept_Connection(details);
        printf("--- New Client Arrived ---\n");
        bzero(Data, Kilo_byte * 2);
        recv(details.Comm_socket, Data, sizeof(Data), 0);
        printf("[ %d ( <- ) ::: %s \n]", details.port, Data);

        char* token1 = strtok(Data, "#");
        if(strncmp(token1, "READ", 4) == 0)
        {
            // Reading should be done Here by tokenizing
            token1 = strtok(NULL, "#");
            char *path = (char*) malloc (sizeof(char)*1000);
            strcpy(path, token1);
            strcpy(response, Reading(path));
        }
        else if(strncmp(token1, "WRITE", 5) == 0)
        {
            // Writing Should be done Here By tokenizing
            token1 = strtok(NULL, "#");
            char *path = (char*) malloc (sizeof(char)*1000);
            strcpy(path, token1);
            char *writable = (char*) malloc (sizeof(char) * Kilo_byte * 10);
            token1 = strtok(NULL, "#");
            strcpy(writable, token1);
            writing(path, writable);
            strcpy(response, "Writing Done Successfully");
        }
        else if(strncmp(token1, "COPY_READ", 9) == 0)
        {
            token1 = strtok(NULL, "#");
            token1 = strtok(NULL, "#");
            char srcpath[Kilo_byte] = {'\0'};
            char srcpath2[Kilo_byte] = {'\0'};
            strcpy(srcpath,token1);
            strcpy(srcpath2,token1);
            char* ptr = strtok(srcpath,"/");
            while(ptr!=NULL){
                ptr = strtok(NULL,"/");
                bzero(srcpath,sizeof(srcpath));
                strcpy(srcpath,ptr);
            }
            char *read_Cont = Reading(srcpath);

            char Response[10000]; 
            sprintf(Response, "%s#%s", srcpath, read_Cont);
            
            token1 = strtok(NULL, "#");
            Connection_Details src_copy;
            src_copy.port = 7000;
            strcpy(src_copy.IP_address,"127.0.0.1");
            src_copy = Establish_Connection_as_Server(src_copy);
            src_copy = Accept_Connection(src_copy);
            
            send(src_copy.Comm_socket, Response, strlen(Response), 0);

            close(src_copy.Comm_socket);
        }

        else if(strncmp(token1, "COPY_WRITE", 10) == 0)
        {
            token1 = strtok(NULL, "#");
            token1 = strtok(NULL, "#");
            token1 = strtok(NULL, "#");

            Connection_Details des_com;
            des_com.port = 7000;
            strcpy(des_com.IP_address,"127.0.0.1");
            des_com = Establish_Connection_as_Client(des_com);
            
            char recieved[10000];
            recv(des_com.Comm_socket, recieved, sizeof(recieved), 0);

            char* tokk = strtok(recieved, "#");
            char completepath[1000] = {'\0'};
            strcpy(completepath, token1);
            strcat(completepath, tokk);

            creation(completepath);

            tokk = strtok(NULL, "#");
            char sendx[10000] = {'\0'};
            strcpy(sendx,tokk);
            writing(completepath, sendx);

            close(des_com.Comm_socket);
        }

        send(details.Comm_socket, response, strlen(response), 0);

        close(details.Client_socket);
    }
}
void *naming_server(void *arg)
{
    Connection_Details details;
    details.port = 4001;
    strcpy(details.IP_address, "127.0.0.1");

    details = Establish_Connection_as_Client(details);
    printf("--- Connected to the Naming Server ---\n");

    char buffer[1024];
    // int rand = 5;
    // int* ptr = &rand;
    // send(details.Comm_socket, ptr, sizeof(int), 0);
    // bzero(buffer,sizeof(buffer));
    // recv(details.Comm_socket, buffer, sizeof(buffer), 0);
    int a; // = send(details.Comm_socket, file_array, sizeof(file_array)*MAX_FILES_IN_SS, 0);

    while (1)
    {
        bzero(buffer, 1024);
        a = recv(details.Comm_socket, buffer, sizeof(buffer), 0);

        char* token = strtok(buffer, " ");
        if(strncmp(token, "CREATE", 6) == 0)
        {
            token = strtok(NULL, " ");
            printf("String = %s\n", token);
            creation(token);
        }
        else if(strncmp(token, "DELETE", 6) == 0)
        {
            token = strtok(NULL, " ");
            printf("String = %s\n", token);
            deletion(token);
        }

        printf("[ %d ( <<< ) ::::: %s \n]", details.port, buffer);

        if (strcmp(buffer, EXIT_CODE) == 0) // exit
        {
            break;
        }
        sleep(1);

        strcpy(buffer,TESTING);
        send(details.Comm_socket, buffer, strlen(buffer), 0);
        // break;
    }

    close(details.Comm_socket);
    printf("--- Disconnected from the server ---\n");
}


int main()
{
    strcpy(file_array[0].path,"/a/a.txt");
    file_array[0].permissions = READ_WRITE;
    file_array[0].size = 10;
    strcpy(file_array[0].path,"/b/b.txt");
    file_array[0].permissions = READ_PERMIT;
    file_array[0].size = 7;
    strcpy(file_array[0].path,"/c/c.txt");
    file_array[0].permissions = WRITE_PERMIT;
    file_array[0].size = 21;


    pthread_t ns, client;
    pthread_create(&ns, NULL, naming_server, NULL);
    pthread_create(&client, NULL, client_connect, NULL);

    pthread_join(client, NULL);
    pthread_join(ns, NULL);
    return 0;
}
