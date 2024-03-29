#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipefdone[2], pipefdtwo[2];
    if (pipe(pipefdone) == -1 || pipe(pipefdtwo) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t gpg_pid = fork();
    if (gpg_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (gpg_pid == 0) {
        // Child 1 (gpg)
        close(pipefdone[0]);
        close(pipefdtwo[0]);
        close(pipefdtwo[1]);

        dup2(pipefdone[1], STDOUT_FILENO);  // Redirect stdout to the pipe
        close(pipefdone[1]);
        char *arglist[] = {"gpg", "-d", argv[1], NULL};
        execvp(arglist[0], arglist);
        perror("execvp gpg");
        exit(EXIT_FAILURE);
    }

    

    pid_t gunzip_pid = fork();
    if (gunzip_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (gunzip_pid == 0) {
        // Child 2 (gunzip)
        close(pipefdone[1]);
        close(pipefdtwo[0]);

        dup2(pipefdone[0], STDIN_FILENO);   // Redirect stdin to the first pipe
        close(pipefdone[0]);

        dup2(pipefdtwo[1], STDOUT_FILENO);  // Redirect stdout to the second pipe
        close(pipefdtwo[1]);
        char *arglist[] = {"gunzip", "-d", NULL};
        execvp(arglist[0], arglist);
        perror("execvp gunzip");
        exit(EXIT_FAILURE);
    }

    pid_t tar_pid = fork();
    if (tar_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (tar_pid == 0) {
        close(pipefdtwo[1]);
        close(pipefdone[0]);
        close(pipefdone[1]);

        dup2(pipefdtwo[0], STDIN_FILENO);   // Redirect stdin to the second pipe
        close(pipefdtwo[0]);
        char *arglist[] = {"tar", "-xv", NULL}; // Extract from stdin
        execvp(arglist[0], arglist);
        perror("execvp tar");
        exit(EXIT_FAILURE);
    }

    close(pipefdone[0]);
    close(pipefdone[1]);
    close(pipefdtwo[0]);
    close(pipefdtwo[1]);
    waitpid(gpg_pid, NULL, 0);
    waitpid(gunzip_pid, NULL, 0);
    waitpid(tar_pid, NULL, 0);

    printf("Decompression and extraction completed successfully.\n");
    return 0;
}
