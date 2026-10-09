#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct {
    int show_all;        // Cờ cho -a
    int show_almost_all; // Cờ cho -A
    int show_long;       // Cờ cho -l
} LsOptions;

extern LsOptions current_options;
void parse_options(int argc, char *argv[]);

#endif
