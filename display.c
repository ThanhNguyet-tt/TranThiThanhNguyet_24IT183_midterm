#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include "display.h"
#include "options.h"

// Tao duong dan day du
static int make_full_path(char *buffer, size_t size,
const char *path, const char *name) {
int len;


if (path == NULL || path[0] == '\0') {
    len = snprintf(buffer, size, "%s", name);
} else {
    len = snprintf(buffer, size, "%s/%s", path, name);
}

return len >= 0 && (size_t)len < size;


}

// Hien thi kich thuoc theo dang de doc
static void print_human_size(unsigned long long size) {
const char *units[] = {"B", "K", "M", "G", "T", "P"};
double value = (double)size;
int unit = 0;


while (value >= 1024.0 && unit < 5) {
    value /= 1024.0;
    unit++;
}

if (unit == 0) {
    printf("%llu", size);
} else {
    printf("%.1f%s", value, units[unit]);
}


}

// Hien thi ten file, xu ly ky tu khong in duoc
static void print_name(const char *name) {
const unsigned char *p = (const unsigned char *)name;


while (*p != '\0') {
    if (current_options.print_question && !isprint(*p)) {
        putchar('?');
    } else {
        putchar(*p);
    }

    p++;
}


}

// Hien thi ky hieu loai file khi co -F
static void print_file_type(mode_t mode) {
if (!current_options.show_type_indicator) {
return;
}


if (S_ISDIR(mode)) {
    putchar('/');
} else if (S_ISLNK(mode)) {
    putchar('@');
} else if (S_ISFIFO(mode)) {
    putchar('|');
} else if (S_ISSOCK(mode)) {
    putchar('=');
} else if (mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
    putchar('*');
}


}

// Hien thi kich thuoc
static void print_size(unsigned long long size) {
if (current_options.human_readable) {
print_human_size(size);
} else {
printf("%llu", size);
}
}

void print_item(const char *path, const char *name) {
char full_path[4096];


if (!make_full_path(full_path, sizeof(full_path), path, name)) {
    fprintf(stderr, "myls: path too long\n");
    return;
}

struct stat st;

// Lay thong tin file, khong di theo symbolic link
if (lstat(full_path, &st) == -1) {
    perror(full_path);
    return;
}

// In so inode neu co -i
if (current_options.show_inode) {
    printf("%llu ",
           (unsigned long long)st.st_ino);
}

// In so block neu co -s
// In so block neu co -s
if (current_options.show_blocks) {
    unsigned long long bytes =
        (unsigned long long)st.st_blocks * 512ULL;

    if (current_options.human_readable) {
        print_human_size(bytes);
    } else {
        unsigned long long block_size = 512ULL;

        if (current_options.kibibytes) {
            block_size = 1024ULL;
        } else {
            const char *env = getenv("BLOCKSIZE");

            if (env != NULL && env[0] != '\0') {
                char *end;
                unsigned long long value = strtoull(env, &end, 10);
                unsigned long long multiplier = 1;

                if (*end == 'K' || *end == 'k') {
                    multiplier = 1024ULL;
                    end++;
                } else if (*end == 'M' || *end == 'm') {
                    multiplier = 1024ULL * 1024ULL;
                    end++;
                } else if (*end == 'G' || *end == 'g') {
                    multiplier = 1024ULL * 1024ULL * 1024ULL;
                    end++;
                }

                if (*end == '\0' && value > 0 &&
                    value <= ULLONG_MAX / multiplier) {
                    block_size = value * multiplier;
                }
            }
        }

        printf("%llu", (bytes + block_size - 1) / block_size);
    }

    putchar(' ');
}

// Neu khong co -l thi chi in ten file
if (!current_options.show_long) {
    print_name(name);
    print_file_type(st.st_mode);
    putchar('\n');
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

// Xu ly SUID, SGID va sticky bit
if (st.st_mode & S_ISUID) {
    mode[3] = (st.st_mode & S_IXUSR) ? 's' : 'S';
}

if (st.st_mode & S_ISGID) {
    mode[6] = (st.st_mode & S_IXGRP) ? 's' : 'S';
}

if (st.st_mode & S_ISVTX) {
    mode[9] = (st.st_mode & S_IXOTH) ? 't' : 'T';
}

// Lay thong tin chu so huu va nhom
struct passwd *user = getpwuid(st.st_uid);
struct group *group = getgrgid(st.st_gid);

// -n hien thi UID va GID thay vi ten
if (current_options.show_numeric || user == NULL) {
    printf("%s %lu %u %u ",
           mode,
           (unsigned long)st.st_nlink,
           (unsigned int)st.st_uid,
           (unsigned int)st.st_gid);
} else {
    const char *user_name = user->pw_name;
    const char *group_name =
        group ? group->gr_name : NULL;

    printf("%s %lu %-8s %-8s ",
           mode,
           (unsigned long)st.st_nlink,
           user_name,
           group_name ? group_name : "unknown");
}

// Hien thi kich thuoc file
if (S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode)) {
    printf("%3u, %3u ",
           major(st.st_rdev),
           minor(st.st_rdev));
} else {
    print_size((unsigned long long)st.st_size);
    putchar(' ');
}

// Chon thoi gian theo -c, -u hoac mac dinh
time_t file_time;

if (current_options.time_ctime) {
    file_time = st.st_ctime;
} else if (current_options.time_atime) {
    file_time = st.st_atime;
} else {
    file_time = st.st_mtime;
}

char time_buffer[32];
struct tm *time_info = localtime(&file_time);

if (time_info != NULL) {
    strftime(time_buffer, sizeof(time_buffer),
             "%b %e %H:%M", time_info);
} else {
    strcpy(time_buffer, "unknown");
}

printf("%s ", time_buffer);

// Hien thi ten va ky hieu loai file
print_name(name);
print_file_type(st.st_mode);

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
        printf(" -> ");
        print_name(link_target);
    }
}

putchar('\n');


}
