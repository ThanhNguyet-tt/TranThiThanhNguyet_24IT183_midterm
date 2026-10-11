# Midterm Project – Implement `ls(1)`

## 1. Student Information

- **Name:** Tran Thi Thanh Nguyet
- **Student ID:** 24IT183
- **University:** Vietnam-Korea University of Information and Communication Technology (VKU)

## 2. Project Description

This project implements a simplified version of the UNIX `ls(1)` command in C, based on the provided NetBSD manual page.

The program uses POSIX system calls and library functions such as `opendir()`, `readdir()`, `lstat()`, and `closedir()` to access file and directory information.

The source code is divided into multiple modules for easier maintenance and testing.

## 3. Implemented Features

| Category | Options | Description |
|---|---|---|
| Visibility | `-a`, `-A` | Display hidden files |
| Directory | `-d`, `-R` | Display directory itself or recursively list directories |
| Long format | `-l`, `-n` | Display detailed file information |
| File information | `-i`, `-s`, `-h`, `-k`, `-F` | Display inode, blocks, readable sizes and file type indicators |
| Sorting | `-f`, `-r`, `-S`, `-t` | Control file sorting |
| Time | `-c`, `-u` | Select file timestamps |
| Character display | `-q`, `-w` | Control display of non-printable characters |

## 4. Project Structure

```text
main.c          Main program
ls.h            Shared declarations
listing.c       Directory listing and recursion
listing.h       Directory listing declarations
options.c       Command-line option processing
options.h       Option definitions
display.c       File information formatting
display.h       Display declarations
sorting.c       Directory entry sorting
sorting.h       Sorting declarations
Makefile        Build instructions
README.md       Project documentation
.gitignore      Ignored generated files
```

## 5. Compilation

Requirements:

- Linux or WSL Ubuntu
- GCC compiler
- GNU Make

Compile the project:

```bash
make clean
make
```

The executable file `myls` will be generated.

## 6. Usage

Basic syntax:

```bash
./myls [options] [file ...]
```

Examples:

```bash
./myls
./myls -la
./myls -R testdir
./myls -lR testdir
./myls -i .
./myls -S .
./myls -t .
./myls -lh .
./myls -d .
```

## 7. Testing

The program has been tested with regular files, directories, hidden files, nested directories, multiple operands, sorting options, and nonexistent paths.

Examples:

```bash
./myls -a .
./myls -A .
./myls -r .
./myls -S .
./myls -t .
./myls -R .
./myls nonexistent_file
echo $?
```

The program returns a nonzero exit status when a nonexistent file operand is encountered.

## 8. GitHub Repository

https://github.com/ThanhNguyet-tt/TranThiThanhNguyet_24IT183_midterm
