#include <stdio.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include "display.h"
#include "options.h"

void print_item(const char *dir_path, const char *filename) {
    char full_path[1024], disp_name[512];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, filename);
    strncpy(disp_name, filename, sizeof(disp_name));

    // -q: Đổi ký tự lỗi thành '?'
    if (current_options.print_question && !current_options.print_raw) {
        for (int i = 0; disp_name[i] != '\0'; i++) {
            if (disp_name[i] < 32 || disp_name[i] == 127) disp_name[i] = '?';
        }
    }

    struct stat st;
    if (lstat(full_path, &st) == -1) return;

    char ind = '\0';
    if (current_options.show_type_indicator) {
        if (S_ISDIR(st.st_mode)) ind = '/';
        else if (S_ISLNK(st.st_mode)) ind = '@';
        else if (S_ISSOCK(st.st_mode)) ind = '=';
        else if (S_ISFIFO(st.st_mode)) ind = '|';
        else if (st.st_mode & 0111) ind = '*';
    }

    if (current_options.show_inode) printf("%7lu ", (unsigned long)st.st_ino);

    if (current_options.show_blocks) {
        long long blk = current_options.kibibytes ? (st.st_blocks / 2) : st.st_blocks;
        printf("%4lld ", blk);
    }

    if (current_options.show_long) {
        char mode[11] = "----------";
        if (S_ISDIR(st.st_mode)) mode[0] = 'd'; else if (S_ISLNK(st.st_mode)) mode[0] = 'l';
        
        if (st.st_mode & S_IRUSR) mode[1] = 'r'; if (st.st_mode & S_IWUSR) mode[2] = 'w'; if (st.st_mode & S_IXUSR) mode[3] = 'x';
        if (st.st_mode & S_IRGRP) mode[4] = 'r'; if (st.st_mode & S_IWGRP) mode[5] = 'w'; if (st.st_mode & S_IXGRP) mode[6] = 'x';
        if (st.st_mode & S_IROTH) mode[7] = 'r'; if (st.st_mode & S_IWOTH) mode[8] = 'w'; if (st.st_mode & S_IXOTH) mode[9] = 'x';

        char u_str[256], g_str[256];
        if (current_options.show_numeric) {
            snprintf(u_str, 256, "%u", st.st_uid); snprintf(g_str, 256, "%u", st.st_gid);
        } else {
            struct passwd *pw = getpwuid(st.st_uid); struct group *gr = getgrgid(st.st_gid);
            snprintf(u_str, 256, "%s", pw ? pw->pw_name : "unknown"); snprintf(g_str, 256, "%s", gr ? gr->gr_name : "unknown");
        }

        time_t target_t = current_options.time_atime ? st.st_atime : (current_options.time_ctime ? st.st_ctime : st.st_mtime);
        char t_buf[256]; strftime(t_buf, 256, "%b %d %H:%M", localtime(&target_t));

        printf("%s %lu %s %s ", mode, (unsigned long)st.st_nlink, u_str, g_str);
        
        // -h: Human readable
        if (current_options.human_readable) {
            double s = st.st_size; int i = 0; const char* units[] = {"B", "K", "M", "G", "T"};
            while (s >= 1024 && i < 4) { s /= 1024; i++; }
            if (i == 0) printf("%5lld ", (long long)st.st_size); else printf("%4.1f%s ", s, units[i]);
        } else {
            printf("%8lld ", (long long)st.st_size);
        }

        printf("%s %s%c\n", t_buf, disp_name, ind ? ind : ' ');
    } else {
        printf("%s%c  ", disp_name, ind ? ind : ' ');
    }
}
