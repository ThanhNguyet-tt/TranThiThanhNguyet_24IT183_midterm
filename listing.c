#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ls.h"
#include "options.h"
#include "display.h"
#include "sorting.h"
#include "listing.h"

// Tinh tong so block cua cac muc trong thu muc
static unsigned long long calculate_total(
    const char *path,
    char **entries,
    size_t count
) {
    unsigned long long total = 0;

    for (size_t i = 0; i < count; i++) {
        char full_path[4096];

        int len = snprintf(
            full_path,
            sizeof(full_path),
            "%s/%s",
            path,
            entries[i]
        );

        if (len < 0 || (size_t)len >= sizeof(full_path)) {
            continue;
        }

        struct stat st;

        if (lstat(full_path, &st) == 0) {
            total += (unsigned long long)st.st_blocks;
        }
    }

    return total;
}
void list_directory(const char *path) {
// Neu doi tuong can liet ke la file, hien thi truc tiep

if (current_options.dir_as_file) {
    print_item("", path);
    return;
}

// Mo thu muc
DIR *dir = opendir(path);

if (dir == NULL) {
    perror("myls");
    return;
}

// Cap phat bo nho de luu ten cac muc trong thu muc
size_t capacity = 64;
size_t count = 0;

char **entries = malloc(capacity * sizeof(*entries));

if (entries == NULL) {
    perror("malloc");
    closedir(dir);
    return;
}

struct dirent *entry;

// Doc tung muc trong thu muc
while ((entry = readdir(dir)) != NULL) {
    // Xu ly cac file an bat dau bang dau cham
    if (entry->d_name[0] == '.') {
        // Khong hien thi file an neu khong co -a hoac -A
        if (!current_options.show_all &&
            !current_options.show_almost_all) {
            continue;
        }

        // -A hien thi file an nhung bo qua . va ..
        if (current_options.show_almost_all &&
            !current_options.show_all &&
            (strcmp(entry->d_name, ".") == 0 ||
             strcmp(entry->d_name, "..") == 0)) {
            continue;
        }
    }

    // Neu mang da day thi tang kich thuoc
    if (count == capacity) {
        size_t new_capacity = capacity * 2;

        char **temp = realloc(
            entries,
            new_capacity * sizeof(*entries)
        );

        if (temp == NULL) {
            perror("realloc");

            for (size_t i = 0; i < count; i++) {
                free(entries[i]);
            }

            free(entries);
            closedir(dir);
            return;
        }

        entries = temp;
        capacity = new_capacity;
    }

    // Luu ten muc vao mang
    entries[count] = strdup(entry->d_name);

    if (entries[count] == NULL) {
        perror("strdup");

        for (size_t i = 0; i < count; i++) {
            free(entries[i]);
        }

        free(entries);
        closedir(dir);
        return;
    }

    count++;
}

// Dong thu muc sau khi doc xong
closedir(dir);

// Sap xep cac muc neu khong su dung -f
if (!current_options.sort_none) {
    current_dir_for_sort = path;

    qsort(
        entries,
        count,
        sizeof(*entries),
        compare_entries
    );
}

// In tong block khi dung -l hoac -s
if (current_options.show_long ||
    (current_options.show_blocks &&
     isatty(STDOUT_FILENO))) {

    printf("total %llu\n",
           calculate_total(path, entries, count));
}

// Hien thi cac muc trong thu muc
for (size_t i = 0; i < count; i++) {
    print_item(path, entries[i]);
}

// Xuong dong sau khi hien thi danh sach
if (!current_options.show_long && count > 0) {
    printf("\n");
}

// Duyet cac thu muc con neu co tuy chon -R
if (current_options.recursive) {
    for (size_t i = 0; i < count; i++) {
        // Bo qua thu muc hien tai va thu muc cha
        if (strcmp(entries[i], ".") == 0 ||
            strcmp(entries[i], "..") == 0) {
            continue;
        }

        // Tao duong dan day du
        char f_path[4096];

        int len = snprintf(
            f_path,
            sizeof(f_path),
            "%s/%s",
            path,
            entries[i]
        );

        // Kiem tra duong dan co qua dai khong
        if (len < 0 || (size_t)len >= sizeof(f_path)) {
            fprintf(stderr, "myls: path too long\n");
            continue;
        }

        struct stat st;

        // Chi de quy neu muc la thu muc that
        if (lstat(f_path, &st) == 0 &&
            S_ISDIR(st.st_mode)) {
            printf("\n%s:\n", f_path);

            list_directory(f_path);
        }
    }
}

// Giai phong bo nho
for (size_t i = 0; i < count; i++) {
    free(entries[i]);
}

free(entries);

}
