
#include <stdlib.h>
#include <stdio.h>

#include "main.h"
#include "core.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Error: Invalid amount of arguments. [%d]\n", argc);
        exit(EXIT_FAILURE);
    }

    char* fpath = argv[1];
    mainAlgorithm(fpath);

    return 0;
}
