#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "ls.h"
#include "options.h"
#include "display.h"

void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("myls");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            if (!current_options.show_all && !current_options.show_almost_all) continue;
            if (current_options.show_almost_all && !current_options.show_all) {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            }
        }
        
        // Gọi thẳng print_item, hàm này tự biết cờ nào đang bật để in cho đúng
        print_item(path, entry->d_name);
    }
    
    if (!current_options.show_long) {
        printf("\n");
    }

    closedir(dir);
}
