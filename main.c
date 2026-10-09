#include <stdio.h>
#include "ls.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        list_directory(".");
    } else {
        for (int i = 1; i < argc; i++) {
            printf("%s:\n", argv[i]);
            list_directory(argv[i]);
            if (i < argc - 1) printf("\n");
        }
    }
    return 0;
}
