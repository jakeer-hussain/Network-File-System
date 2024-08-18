#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {

    const char *file_path = "/home/doraemon/Documents/Operating Systems/test/hello.txt";
    const char *data = "Hello World";

    // Open the file for writing, creating it if it doesn't exist
    int file_descriptor = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (file_descriptor == -1) {
        perror("Error opening file");
        return 0;
    }

    // Write data to the file
    ssize_t bytes_written = write(file_descriptor, data, strlen(data));

    // Close the file
    close(file_descriptor);

    if (bytes_written == -1) {
        perror("Error writing to file");
        return 0;
    } else {
        printf("Data written successfully to %s\n", file_path);
        return 1;
    }
}
