#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "options.h"

LsOptions current_options = {0, 0, 0, 0, 0, 0};

void parse_options(int argc, char *argv[]) {
    int opt;
    // Thêm n, i, s vào getopt
    while ((opt = getopt(argc, argv, "aAlnis")) != -1) {
        switch (opt) {
            case 'a': current_options.show_all = 1; break;
            case 'A': current_options.show_almost_all = 1; break;
            case 'l': current_options.show_long = 1; break;
            case 'n': 
                current_options.show_numeric = 1; 
                current_options.show_long = 1; // -n mặc định kéo theo -l
                break;
            case 'i': current_options.show_inode = 1; break;
            case 's': current_options.show_blocks = 1; break;
            default: exit(1);
        }
    }
}
