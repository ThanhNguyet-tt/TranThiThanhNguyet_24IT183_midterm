#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "ls.h"
#include "options.h"
#include "display.h"
#include "sorting.h"

void list_directory(const char *path) {
    if (current_options.dir_as_file) {
        print_item(".", path);
        if (!current_options.show_long) printf("\n");
        return;
    }

    DIR *dir = opendir(path);
    if (dir == NULL) { perror("myls"); return; }

    // BẮT ĐẦU FIX: Sử dụng cấp phát động mở rộng tự động bằng realloc
    int capacity = 1024;
    char **entries = malloc(capacity * sizeof(char *));
    if (!entries) { perror("malloc"); closedir(dir); return; }

    int count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            if (!current_options.show_all && !current_options.show_almost_all) continue;
            if (current_options.show_almost_all && !current_options.show_all) {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            }
        }
        
        // Kiem tra gioi han va mo rong bo nho nếu vượt quá capacity
        if (count >= capacity) {
            capacity *= 2;
            char **temp = realloc(entries, capacity * sizeof(char *));
            if (!temp) { perror("realloc"); break; }
            entries = temp;
        }
        entries[count++] = strdup(entry->d_name);
    }
    closedir(dir);

    // Sap xep
    current_dir_for_sort = path;
    qsort(entries, count, sizeof(char *), compare_entries);

    // In danh sach
    for (int i = 0; i < count; i++) print_item(path, entries[i]);
    if (!current_options.show_long && count > 0) printf("\n");

    // De quy -R
    if (current_options.recursive) {
        for (int i = 0; i < count; i++) {
            if (strcmp(entries[i], ".") != 0 && strcmp(entries[i], "..") != 0) {
                char f_path[1024]; snprintf(f_path, sizeof(f_path), "%s/%s", path, entries[i]);
                struct stat st;
                if (lstat(f_path, &st) == 0 && S_ISDIR(st.st_mode)) {
                    printf("\n%s:\n", f_path);
                    list_directory(f_path);
                }
            }
        }
    }

    // FIX: Giải phóng bộ nhớ chuỗi strdup() cực kỳ an toàn
    for (int i = 0; i < count; i++) free(entries[i]);
    free(entries);
}
