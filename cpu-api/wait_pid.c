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
        printf("Child process\n");
        exit(0);
    } else {
        // Parent process
        // 使用 waitpid() 等待特定子进程的退出
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process exited abnormally\n");
        }

        printf("Parent process\n");
    }

    return 0;
}

/* 

waitpid()在以下情况下特别有用：

- 等待特定的子进程：通过指定子进程的PID，你可以选择性地等待某个具体的子进程。
- 非阻塞等待：通过设置 WNOHANG 选项，可以使waitpid()变为非阻塞，即如果没有子进程退出，它会立即返回而不会阻塞父进程。

[root@us-arm cpu-api]# gcc wait_pid.c -o wait_pid.out
[root@us-arm cpu-api]# ./wait_pid.out 
Child process
Child process exited with status 0
Parent process
 */