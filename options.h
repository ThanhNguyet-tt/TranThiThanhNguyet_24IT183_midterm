#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct {
    int show_all; int show_almost_all; int show_long; int show_numeric;
    int show_inode; int show_blocks; int show_type_indicator;
    int sort_none; int reverse_sort; int sort_size; int sort_time;
    int human_readable; int kibibytes;
    int time_ctime; int time_atime;
    int print_question; int print_raw;
    int dir_as_file; int recursive;
} LsOptions;

extern LsOptions current_options;
void parse_options(int argc, char *argv[]);

#endif
