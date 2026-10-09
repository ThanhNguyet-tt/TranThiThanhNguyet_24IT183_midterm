#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include "display.h"
#include "options.h"

void print_item(const char *dir_path, const char *filename) {
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, filename);

    struct stat st;
    if (lstat(full_path, &st) == -1) {
        perror("lstat");
        return;
    }

    // Nếu có cờ -i: In số Inode đầu tiên
    if (current_options.show_inode) {
        printf("%7lu ", (unsigned long)st.st_ino);
    }

    // Nếu có cờ -s: In số Blocks (Hệ thống Linux thường chia 2 để ra dung lượng 1K blocks)
    if (current_options.show_blocks) {
        printf("%4lld ", (long long)(st.st_blocks / 2));
    }

    // Nếu có cờ -l hoặc -n
    if (current_options.show_long) {
        char mode[11] = "----------";
        if (S_ISDIR(st.st_mode)) mode[0] = 'd';
        else if (S_ISLNK(st.st_mode)) mode[0] = 'l';
        else if (S_ISCHR(st.st_mode)) mode[0] = 'c';
        else if (S_ISBLK(st.st_mode)) mode[0] = 'b';

        if (st.st_mode & S_IRUSR) mode[1] = 'r';
        if (st.st_mode & S_IWUSR) mode[2] = 'w';
        if (st.st_mode & S_IXUSR) mode[3] = 'x';
        if (st.st_mode & S_IRGRP) mode[4] = 'r';
        if (st.st_mode & S_IWGRP) mode[5] = 'w';
        if (st.st_mode & S_IXGRP) mode[6] = 'x';
        if (st.st_mode & S_IROTH) mode[7] = 'r';
        if (st.st_mode & S_IWOTH) mode[8] = 'w';
        if (st.st_mode & S_IXOTH) mode[9] = 'x';

        // Xử lý riêng cho cờ -n: Chuyển User/Group thành con số
        char user_str[256], group_str[256];
        if (current_options.show_numeric) {
            snprintf(user_str, sizeof(user_str), "%u", st.st_uid);
            snprintf(group_str, sizeof(group_str), "%u", st.st_gid);
        } else {
            struct passwd *pw = getpwuid(st.st_uid);
            struct group  *gr = getgrgid(st.st_gid);
            snprintf(user_str, sizeof(user_str), "%s", pw ? pw->pw_name : "unknown");
            snprintf(group_str, sizeof(group_str), "%s", gr ? gr->gr_name : "unknown");
        }

        char time_buf[256];
        struct tm *tm_info = localtime(&st.st_mtime);
        strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm_info);

        printf("%s %lu %s %s %8lld %s %s\n",
               mode, (unsigned long)st.st_nlink, user_str, group_str,
               (long long)st.st_size, time_buf, filename);
    } else {
        // Nếu không có cờ -l hoặc -n thì chỉ in tên
        printf("%s  ", filename);
    }
}
