#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

#include "display.h"
#include "options.h"

void print_item(const char *path, const char *name) {
char full_path[4096];


// Tao duong dan day du den file
int len = snprintf(full_path, sizeof(full_path), "%s/%s", path, name);

if (len < 0 || (size_t)len >= sizeof(full_path)) {
    fprintf(stderr, "myls: path too long\n");
    return;
}

struct stat st;

// Lay thong tin file
if (lstat(full_path, &st) == -1) {
    // Truong hop doi tuong la file duoc truyen truc tiep
    if (lstat(path, &st) == -1) {
        perror("myls");
        return;
    }
}

// Neu khong su dung -l, chi hien thi ten
if (!current_options.show_long) {
    printf("%s   ", name);
    return;
}

// Tao chuoi quyen truy cap
char mode[11] = "----------";

if (S_ISDIR(st.st_mode)) {
    mode[0] = 'd';
} else if (S_ISLNK(st.st_mode)) {
    mode[0] = 'l';
} else if (S_ISCHR(st.st_mode)) {
    mode[0] = 'c';
} else if (S_ISBLK(st.st_mode)) {
    mode[0] = 'b';
} else if (S_ISFIFO(st.st_mode)) {
    mode[0] = 'p';
} else if (S_ISSOCK(st.st_mode)) {
    mode[0] = 's';
}

// Quyen cua chu so huu
if (st.st_mode & S_IRUSR) mode[1] = 'r';
if (st.st_mode & S_IWUSR) mode[2] = 'w';
if (st.st_mode & S_IXUSR) mode[3] = 'x';

// Quyen cua nhom
if (st.st_mode & S_IRGRP) mode[4] = 'r';
if (st.st_mode & S_IWGRP) mode[5] = 'w';
if (st.st_mode & S_IXGRP) mode[6] = 'x';

// Quyen cua nguoi dung khac
if (st.st_mode & S_IROTH) mode[7] = 'r';
if (st.st_mode & S_IWOTH) mode[8] = 'w';
if (st.st_mode & S_IXOTH) mode[9] = 'x';

// Xu ly quyen SUID, SGID va sticky bit
if (st.st_mode & S_ISUID) {
    mode[3] = (st.st_mode & S_IXUSR) ? 's' : 'S';
}

if (st.st_mode & S_ISGID) {
    mode[6] = (st.st_mode & S_IXGRP) ? 's' : 'S';
}

if (st.st_mode & S_ISVTX) {
    mode[9] = (st.st_mode & S_IXOTH) ? 't' : 'T';
}

// Lay ten chu so huu va ten nhom
struct passwd *user = getpwuid(st.st_uid);
struct group *group = getgrgid(st.st_gid);

const char *user_name = user ? user->pw_name : "unknown";
const char *group_name = group ? group->gr_name : "unknown";

// Lay thoi gian sua doi cuoi cung
char time_buffer[32];
struct tm *time_info = localtime(&st.st_mtime);

if (time_info != NULL) {
    strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M",
             time_info);
} else {
    strcpy(time_buffer, "unknown");
}

// Hien thi thong tin chi tiet
printf("%s %lu %-8s %-8s %8ld %s ",
       mode,
       (unsigned long)st.st_nlink,
       user_name,
       group_name,
       (long)st.st_size,
       time_buffer);

// Hien thi ten file
printf("%s", name);

// Neu la symbolic link thi hien thi dich den
if (S_ISLNK(st.st_mode)) {
    char link_target[4096];

    ssize_t link_len = readlink(
        full_path,
        link_target,
        sizeof(link_target) - 1
    );

    if (link_len >= 0) {
        link_target[link_len] = '\0';
        printf(" -> %s", link_target);
    }
}

printf("\n");


}

