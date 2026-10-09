# Midterm Project: Implement ls(1)

## 1. Student Information
* **Name:** Tran Thi Thanh Nguyet
* **Student ID:** 24IT183
* **Class/University:** VKU

## 2. Project Description
This project implements a simplified version of the UNIX ls(1) command in C, using POSIX system calls (opendir, readdir, lstat).

## 3. Implemented Features
* **Visibility:** -a, -A, -d, -R
* **Format:** -l, -n, -i, -s, -k, -h, -F
* **Sorting:** -f (no sort), -r (reverse), -S (size), -t (time)
* **Time:** -c, -u
* **Characters:** -q, -w

## 4. Compilation & Usage
make clean
make
./myls -la
