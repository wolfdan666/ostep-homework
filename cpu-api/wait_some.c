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
        printf("Child [%d] process\n", getpid());
        exit(0);
    } else {
        // Parent process
        // 等待子进程结束
        int status;
        printf("wait return: [%d]\n", wait(&status));

        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process exited abnormally\n");
        }

        printf("Parent [%d] process\n", getpid());
    }

    return 0;
}

/* 
[root@us-arm cpu-api]# gcc wait_some.c -o wait_some.out
[root@us-arm cpu-api]# ./wait_some.out 
Child [3038984] process
wait return: [3038984]
Child process exited with status 0
Parent [3038983] process
 */