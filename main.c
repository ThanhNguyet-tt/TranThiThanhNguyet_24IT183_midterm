
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "ls.h"
#include "options.h"
#include "display.h"

// So sanh ten duong dan theo thu tu tu dien

int compare_paths(const void *a, const void *b)
{
    const char *path1 = *(const char **)a;
    const char *path2 = *(const char **)b;

    struct stat st1, st2;

    int result = strcoll(path1, path2);

    // Lay thong tin hai file
    if (lstat(path1, &st1) == 0 &&
        lstat(path2, &st2) == 0)
    {
        // Sap xep theo kich thuoc: -S
        if (current_options.sort_size)
        {
            if (st1.st_size > st2.st_size)
                result = -1;
            else if (st1.st_size < st2.st_size)
                result = 1;
        }
        // Sap xep theo thoi gian: -t
        else if (current_options.sort_time)
        {
            if (st1.st_mtime > st2.st_mtime)
                result = -1;
            else if (st1.st_mtime < st2.st_mtime)
                result = 1;
        }
    }

    // Dao nguoc thu tu: -r
if (current_options.reverse_sort)    {
        result = -result;
    }

    return result;
}

int main(int argc, char *argv[])
{
    int error_status = 0;
    int printed = 0;

    parse_options(argc, argv);

    // Neu khong co doi so
    if (optind == argc)
    {
        list_directory(".");
        return 0;
    }

    // Tao mang chua cac doi so
    int count = argc - optind;

    char **paths = malloc(count * sizeof(char *));

    if (paths == NULL)
    {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < count; i++)
    {
        paths[i] = argv[optind + i];
    }

    // Sap xep ten neu khong co -f
    if (!current_options.sort_none)
    {
        qsort(paths, count, sizeof(char *), compare_paths);
    }

    // Luot 1: Hien thi file truoc
    for (int i = 0; i < count; i++)
    {
        struct stat st;

        if (lstat(paths[i], &st) == -1)
        {
            perror(paths[i]);
            error_status = 1;
            continue;
        }

        if (!S_ISDIR(st.st_mode) ||
            current_options.dir_as_file)
        {
            print_item("", paths[i]);
            printed = 1;
        }
    }

    // Luot 2: Hien thi thu muc
    if (!current_options.dir_as_file)
    {
        for (int i = 0; i < count; i++)
        {
            struct stat st;

            if (lstat(paths[i], &st) == -1)
            {
                continue;
            }

            if (S_ISDIR(st.st_mode))
            {
                if (printed)
                {
                    printf("\n");
                }

                if (count > 1 ||
                    current_options.recursive)
                {
                    printf("%s:\n", paths[i]);
                }

                list_directory(paths[i]);
                printed = 1;
            }
        }
    }

    free(paths);

    return error_status;
}
