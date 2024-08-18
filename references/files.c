#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Current working directory: %s\n", cwd);

    return 0;
}
