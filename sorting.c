#include <string.h>
#include <sys/stat.h>
#include <strings.h>
#include "sorting.h"
#include "options.h"

const char *current_dir_for_sort = ".";

int compare_entries(const void *a, const void *b) {
    if (current_options.sort_none) return 0; // -f (Không sắp xếp)

    const char *nameA = *(const char **)a;
    const char *nameB = *(const char **)b;
    int result = 0;

    if (current_options.sort_size || current_options.sort_time) {
        char pathA[1024], pathB[1024];
        snprintf(pathA, sizeof(pathA), "%s/%s", current_dir_for_sort, nameA);
        snprintf(pathB, sizeof(pathB), "%s/%s", current_dir_for_sort, nameB);

        struct stat stA, stB;
        lstat(pathA, &stA); lstat(pathB, &stB);

        if (current_options.sort_size) {
            if (stA.st_size > stB.st_size) result = -1;
            else if (stA.st_size < stB.st_size) result = 1;
        } else if (current_options.sort_time) {
            time_t tA = current_options.time_atime ? stA.st_atime : (current_options.time_ctime ? stA.st_ctime : stA.st_mtime);
            time_t tB = current_options.time_atime ? stB.st_atime : (current_options.time_ctime ? stB.st_ctime : stB.st_mtime);
            if (tA > tB) result = -1;
            else if (tA < tB) result = 1;
        }
    }

    if (result == 0) result = strcasecmp(nameA, nameB); // Mặc định xếp theo Alphabet
    if (current_options.reverse_sort) result = -result;   // -r (Đảo ngược)
    return result;
}
