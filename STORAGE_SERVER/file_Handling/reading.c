#include <stdio.h>
#include <stdlib.h>

int main() {
    // Get input for the file path from the user
    char filePath[200];
    printf("Enter the file path: ");
    if (scanf("%199s", filePath) != 1) {
        fprintf(stderr, "Error reading file path.\n");
        return 1;
    }

    // Open the file for reading
    FILE* file = fopen(filePath, "r");
    if (!file) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    // Read and print the content of the file
    int character;
    while ((character = fgetc(file)) != EOF) {
        putchar(character);
    }

    // Close the file
    fclose(file);

    return 0;
}