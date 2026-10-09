# Midterm Project: Implement ls(1)

## 1. Student Information
* **Name:** Tran Thi Thanh Nguyet
* **Student ID:** 24IT183
* **Class/University:** VKU

## 2. Project Description
This project implements a simplified, modular version of the UNIX `ls(1)` command written in C, adhering to POSIX system calls (`opendir`, `readdir`, `lstat`) to retrieve filesystem metadata, directory structures, and correct output formatting.

## 3. Features Implemented & Checked
* **Visibility:** `-a`, `-A`, `-d`, `-R`
* **Format:** `-l`, `-n`, `-i`, `-s`, `-k`, `-h`, `-F`
* **Sorting (Stable implemented):** `-f` (no sort), `-r` (reverse), `-S` (size), `-t` (time)
* **Time fields:** `-c` (change time), `-u` (access time)
* **Characters:** `-q`, `-w`
* **Memory Safety:** Implemented dynamic resizing with `realloc()` to prevent overflow in large directories. Safely null-terminates string buffers with `strncpy()`.
* **Robustness:** Gracefully handles invalid directories/paths prior to execution.

## 4. Compilation & Usage
To compile the project:
`make clean`
`make`

Running examples:
`./myls`                   # Basic
`./myls -la`               # Long + hidden
`./myls -lSr`              # Sort by size reversed
`./myls invalid_path`      # Handled safely without segfaults
