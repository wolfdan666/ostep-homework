#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process
        close(pipe_fd[0]); // 关闭管道的读端

        printf("hello\n");

        // 向管道写入一个字节，通知父进程可以继续执行
        write(pipe_fd[1], "1", 1);

        close(pipe_fd[1]); // 关闭管道的写端
    } else {
        // Parent process
        close(pipe_fd[1]); // 关闭管道的写端

        // 阻塞等待子进程写入字节，表示可以继续执行
        char buffer[1];
        read(pipe_fd[0], buffer, 1);

        printf("goodbye\n");

        close(pipe_fd[0]); // 关闭管道的读端
    }

    return 0;
}

/* 
[root@us-arm ostep]# cd ostep-homework/cpu-api/
[root@us-arm cpu-api]# gcc ipc_pipe.c -o ipc_pipe.out 
[root@us-arm cpu-api]# ./ipc_pipe.out 
hello
goodbye
 */