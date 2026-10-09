#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "options.h"

LsOptions current_options = {0, 0};

void parse_options(int argc, char *argv[]) {
    int opt;
    // getopt sẽ duyệt qua các cờ -a và -A
    while ((opt = getopt(argc, argv, "aA")) != -1) {
        switch (opt) {
            case 'a':
                current_options.show_all = 1;
                break;
            case 'A':
                current_options.show_almost_all = 1;
                break;
            default:
                exit(1); // Thoát nếu người dùng nhập sai cờ
        }
    }
}
