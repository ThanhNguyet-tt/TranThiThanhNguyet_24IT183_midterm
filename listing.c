
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

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

// Giai phong mang chua ten cac muc
static void free_entries(char **entries, size_t count) {
    for (size_t i = 0; i < count; i++) {
        free(entries[i]);
    }

    free(entries);
}

// Liet ke noi dung thu muc
// Tra ve 0 neu thanh cong, 1 neu co loi
int list_directory(const char *path) {
    int error_status = 0;

    // Neu su dung -d thi hien thi thu muc nhu file
    if (current_options.dir_as_file) {
        print_item("", path);
        return 0;
    }

    // Mo thu muc
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror(path);
        return 1;
    }

    // Cap phat bo nho de luu ten cac muc
    size_t capacity = 64;
    size_t count = 0;

    char **entries = malloc(capacity * sizeof(*entries));

    if (entries == NULL) {
        perror("malloc");
        closedir(dir);
        return 1;
    }

    struct dirent *entry;

    // Doc tung muc trong thu muc
    while (1) {
        errno = 0;
        entry = readdir(dir);

        if (entry == NULL) {
            // Phan biet het thu muc va loi doc thu muc
            if (errno != 0) {
                perror(path);
                error_status = 1;
            }
            break;
        }

        // Xu ly cac file an
        if (entry->d_name[0] == '.') {
            if (!current_options.show_all &&
                !current_options.show_almost_all) {
                continue;
            }

            // -A bo qua . va ..
            if (current_options.show_almost_all &&
                !current_options.show_all &&
                (strcmp(entry->d_name, ".") == 0 ||
                 strcmp(entry->d_name, "..") == 0)) {
                continue;
            }
        }

        // Tang kich thuoc mang neu da day
        if (count == capacity) {
            size_t new_capacity = capacity * 2;

            char **temp = realloc(
                entries,
                new_capacity * sizeof(*entries)
            );

            if (temp == NULL) {
                perror("realloc");
                free_entries(entries, count);
                closedir(dir);
                return 1;
            }

            entries = temp;
            capacity = new_capacity;
        }

        // Luu ten muc vao mang
        entries[count] = strdup(entry->d_name);

        if (entries[count] == NULL) {
            perror("strdup");
            free_entries(entries, count);
            closedir(dir);
            return 1;
        }

        count++;
    }

    // Dong thu muc sau khi doc
    if (closedir(dir) == -1) {
        perror(path);
        error_status = 1;
    }

    // Sap xep neu khong su dung -f
    if (!current_options.sort_none) {
        current_dir_for_sort = path;

        qsort(
            entries,
            count,
            sizeof(*entries),
            compare_entries
        );
    }

    // In tong block khi dung -l hoac -s voi terminal
    if (current_options.show_long ||
        (current_options.show_blocks &&
         isatty(STDOUT_FILENO))) {

unsigned long long total_blocks =
    calculate_total(path, entries, count);

// Chuyen tu block 512 byte sang block 1024 byte
unsigned long long total_kb = (total_blocks + 1) / 2;

printf("total %llu\n", total_kb);    }

    // Hien thi cac muc trong thu muc
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
            fprintf(stderr, "myls: path too long\n");
            error_status = 1;
            continue;
        }

        struct stat st;

        if (lstat(full_path, &st) == -1) {
            perror(full_path);
            error_status = 1;
            continue;
        }

        print_item(path, entries[i]);
    }

    // Duyet cac thu muc con khi su dung -R
    if (current_options.recursive) {
        for (size_t i = 0; i < count; i++) {
            // Khong de quy vao . va ..
            if (strcmp(entries[i], ".") == 0 ||
                strcmp(entries[i], "..") == 0) {
                continue;
            }

            char f_path[4096];

            int len = snprintf(
                f_path,
                sizeof(f_path),
                "%s/%s",
                path,
                entries[i]
            );

            if (len < 0 || (size_t)len >= sizeof(f_path)) {
                fprintf(stderr, "myls: path too long\n");
                error_status = 1;
                continue;
            }

            struct stat st;

            if (lstat(f_path, &st) == -1) {
                perror(f_path);
                error_status = 1;
                continue;
            }

            // Chi de quy vao thu muc that
            if (S_ISDIR(st.st_mode)) {
                printf("\n%s:\n", f_path);

                if (list_directory(f_path) != 0) {
                    error_status = 1;
                }
            }
        }
    } else if (!current_options.show_long && count > 0) {
        // Giu cach xuong dong cua code cu khi khong de quy
        printf("\n");
    }

    // Giai phong bo nho
    free_entries(entries, count);

    return error_status;
}

