#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "ls.h"
#include "options.h"

void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("myls");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Xử lý logic file ẩn (bắt đầu bằng dấu chấm)
        if (entry->d_name[0] == '.') {
            // Nếu không có -a và không có -A thì bỏ qua file ẩn
            if (!current_options.show_all && !current_options.show_almost_all) {
                continue;
            }
            // Nếu có -A nhưng không có -a, thì bỏ qua "." và ".."
            if (current_options.show_almost_all && !current_options.show_all) {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                    continue;
                }
            }
        }
        printf("%s  ", entry->d_name);
    }
    printf("\n");

    closedir(dir);
}
