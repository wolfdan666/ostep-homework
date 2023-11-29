#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process
        // 使用 execl() 调用 /bin/ls
        execl("/bin/ls", "ls", "-l", (char *)NULL);
        perror("Exec failed");
        exit(1);
    } else {
        // Parent process
        // 等待子进程结束
        wait(NULL);
        printf("Parent process\n");
    }

    return 0;
}

/* 

[root@us-arm cpu-api]# gcc exec_ls.c -o exec_ls.out 
[root@us-arm cpu-api]# ./exec_ls.out 
total 84
-rw-r--r-- 1 root root  4954 Nov 28 14:19 README-fork.md
-rw-r--r-- 1 root root  4996 Nov 28 14:19 README-generator.md
-rw-r--r-- 1 root root   448 Nov 28 14:19 README.md
-rw-r--r-- 1 root root   534 Nov 29 13:00 exec_ls.c
-rwxr-xr-x 1 root root  9448 Nov 29 13:00 exec_ls.out
-rwxr-xr-x 1 root root 12111 Nov 28 14:19 fork.py
-rwxr-xr-x 1 root root 19859 Nov 28 14:19 generator.py
-rw-r--r-- 1 root root  1076 Nov 29 12:56 ipc_pipe.c
-rwxr-xr-x 1 root root  9592 Nov 29 12:55 ipc_pipe.out
Parent process
 */