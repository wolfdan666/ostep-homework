#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    pid_t child1_pid = fork();

    if (child1_pid == -1) {
        perror("Fork failed");
        return 1;
    }

    if (child1_pid == 0) {
        // First child process
        close(pipe_fd[0]);  // Close the read end of the pipe

        // Redirect stdout to the pipe
        dup2(pipe_fd[1], STDOUT_FILENO);

        // Execute a command (e.g., "ls")
        execlp("ls", "ls", (char *)NULL);

        perror("Exec failed");
        exit(1);
    } else {
        // Parent process
        pid_t child2_pid = fork();

        if (child2_pid == -1) {
            perror("Fork failed");
            return 1;
        }

        if (child2_pid == 0) {
            // Second child process
            close(pipe_fd[1]);  // Close the write end of the pipe

            // Redirect stdin to the pipe
            dup2(pipe_fd[0], STDIN_FILENO);

            // Execute another command (e.g., "wc -l")
            execlp("wc", "wc", "-l", (char *)NULL);

            perror("Exec failed");
            exit(1);
        } else {
            // Parent process
            close(pipe_fd[0]);  // Close unused read end of the pipe
            close(pipe_fd[1]);  // Close unused write end of the pipe

            // Wait for both child processes to finish
            waitpid(child1_pid, NULL, 0);
            waitpid(child2_pid, NULL, 0);
        }
    }

    return 0;
}


/* 
[root@us-arm cpu-api]# gcc two_child_pipe.c -o two_child_pipe.out
[root@us-arm cpu-api]# ./two_child_pipe.out 
15
[root@us-arm cpu-api]# ls |wc -l
15
 */