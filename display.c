#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include "display.h"

void print_long_format(const char *dir_path, const char *filename) {
    char full_path[1024];
    // Nối đường dẫn thư mục với tên file để lstat() đọc đúng đường dẫn
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, filename);

    struct stat st;
    // lstat() lấy thông tin file, nếu là symbolic link thì lấy của chính link đó
    if (lstat(full_path, &st) == -1) {
        perror("lstat");
        return;
    }

    // 1. Phân tích loại file và quyền (Permissions)
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

    // 2. Lấy thông tin User và Group
    struct passwd *pw = getpwuid(st.st_uid);
    struct group  *gr = getgrgid(st.st_gid);
    char *user_name = pw ? pw->pw_name : "unknown";
    char *group_name = gr ? gr->gr_name : "unknown";

    // 3. Format thời gian chỉnh sửa file
    char time_buf[256];
    struct tm *tm_info = localtime(&st.st_mtime);
    strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm_info);

    // 4. In ra theo chuẩn ls -l
    printf("%s %lu %s %s %8lld %s %s\n",
           mode,
           (unsigned long)st.st_nlink, // Số lượng hard links
           user_name,                  // Chủ sở hữu
           group_name,                 // Nhóm sở hữu
           (long long)st.st_size,      // Kích thước (bytes)
           time_buf,                   // Thời gian
           filename);                  // Tên file
}
