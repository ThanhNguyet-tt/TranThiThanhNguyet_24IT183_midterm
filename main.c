#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include "ls.h"
#include "options.h"

int main(int argc, char *argv[]) {
    // 1. Phân tích các cờ trước (-a, -A)
    parse_options(argc, argv);

    // 2. Chạy thư mục. optind là chỉ số mảng sau khi đọc xong các option
    if (optind == argc) {
        list_directory("."); // Nếu không truyền thư mục, mặc định là hiện tại
    } else {
        for (int i = optind; i < argc; i++) {
            if (argc - optind > 1) {
                printf("%s:\n", argv[i]);
            }
            list_directory(argv[i]);
            if (i < argc - 1) printf("\n");
        }
    }
    return 0;
}
