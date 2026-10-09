#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "options.h"

LsOptions current_options = {0, 0, 0};

void parse_options(int argc, char *argv[]) {
    int opt;
    // Thêm 'l' vào chuỗi cờ của getopt
    while ((opt = getopt(argc, argv, "aAl")) != -1) {
        switch (opt) {
            case 'a': current_options.show_all = 1; break;
            case 'A': current_options.show_almost_all = 1; break;
            case 'l': current_options.show_long = 1; break;
            default: exit(1);
        }
    }
}
