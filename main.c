#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include "ls.h"
#include "options.h"

int main(int argc, char *argv[]) {
    parse_options(argc, argv);

    if (optind == argc) {
        list_directory(".");
    } else {
        for (int i = optind; i < argc; i++) {
            struct stat st;
            // FIX 2: Kiểm tra đường dẫn hợp lệ trước khi làm gì khác
            if (lstat(argv[i], &st) == -1) {
                perror(argv[i]); // In thẳng lỗi báo không tồn tại
                continue;
            }
            
            if (argc - optind > 1 && S_ISDIR(st.st_mode) && !current_options.dir_as_file) {
                printf("%s:\n", argv[i]);
            }
            list_directory(argv[i]);
            if (i < argc - 1) printf("\n");
        }
    }
    return 0;
}
