#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "options.h"

LsOptions current_options = {0};

void parse_options(int argc, char *argv[]) {
    int opt;
    current_options.print_question = isatty(STDOUT_FILENO); // Mặc định dùng -q nếu in ra terminal

    while ((opt = getopt(argc, argv, "aAlnisFfrStkhcuqwdR")) != -1) {
        switch (opt) {
            case 'a': current_options.show_all = 1; break;
            case 'A': current_options.show_almost_all = 1; break;
            case 'l': current_options.show_long = 1; break;
            case 'n': current_options.show_numeric = 1; current_options.show_long = 1; break;
            case 'i': current_options.show_inode = 1; break;
            case 's': current_options.show_blocks = 1; break;
            case 'F': current_options.show_type_indicator = 1; break;
            case 'f': current_options.sort_none = 1; current_options.show_all = 1; break;
            case 'r': current_options.reverse_sort = 1; break;
            case 'S': current_options.sort_size = 1; break;
            case 't': current_options.sort_time = 1; break;
            case 'k': current_options.kibibytes = 1; break;
            case 'h': current_options.human_readable = 1; break;
            case 'c': current_options.time_ctime = 1; current_options.time_atime = 0; break;
            case 'u': current_options.time_atime = 1; current_options.time_ctime = 0; break;
            case 'q': current_options.print_question = 1; current_options.print_raw = 0; break;
            case 'w': current_options.print_raw = 1; current_options.print_question = 0; break;
            case 'd': current_options.dir_as_file = 1; break;
            case 'R': current_options.recursive = 1; break;
            default: exit(1);
        }
    }
}
