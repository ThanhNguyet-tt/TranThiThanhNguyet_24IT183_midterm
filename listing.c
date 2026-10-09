#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "ls.h"

void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("myls");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            printf("%s  ", entry->d_name);
        }
    }
    printf("\n");

    closedir(dir);
}
