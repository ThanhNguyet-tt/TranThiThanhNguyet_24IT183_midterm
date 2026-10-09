#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct {
    int show_all;        // -a
    int show_almost_all; // -A
    int show_long;       // -l
    int show_numeric;    // -n
    int show_inode;      // -i
    int show_blocks;     // -s
} LsOptions;

extern LsOptions current_options;
void parse_options(int argc, char *argv[]);

#endif
