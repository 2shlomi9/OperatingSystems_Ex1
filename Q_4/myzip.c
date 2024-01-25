#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s directory\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipefdone[2], pipefdtwo[2];
    if (pipe(pipefdone) == -1 || pipe(pipefdtwo) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t tar_pid = fork();
    if (tar_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (tar_pid == 0) {
        // Child 1 (tar)
        close(1); // Close stdout
        dup2(pipefdone[1], 1); // Redirect stdout to pipefdone[1]
        close(pipefdone[0]); // Close unused read end
        close(pipefdtwo[0]); // Close unused pipes from other processes
        close(pipefdtwo[1]);

        char *arglist[] = {"tar", "-cvf", "-", argv[1], NULL};
        execvp(arglist[0], arglist);
        perror("execvp tar");
        exit(EXIT_FAILURE);
    }

    pid_t gzip_pid = fork();
    if (gzip_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (gzip_pid == 0) {
        // Child 2 (gzip)
        close(0); // Close stdin
        close(1); // Close stdout
        dup2(pipefdone[0], 0);  // Redirect stdin to pipefdone[0]
        dup2(pipefdtwo[1], 1); // Redirect stdout to pipefdtwo[1]
        close(pipefdone[1]); // Close unused write end
        close(pipefdtwo[0]); // Close unused read end

        char *arglist[] = {"gzip", "-", NULL};
        execvp(arglist[0], arglist);
        perror("execvp gzip");
        exit(EXIT_FAILURE);
    }

    pid_t gpg_pid = fork();
    if (gpg_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (gpg_pid == 0) {
        // Child 3 (gpg)
        close(0); // Close stdin
        dup2(pipefdtwo[0], 0); // Redirect stdin to pipefdtwo[0]
        close(pipefdone[0]); // Close unused pipes from other processes
        close(pipefdone[1]);
        close(pipefdtwo[1]); // Close unused write end

        char *arglist[] = {"gpg", "-c", "-", NULL};
        execvp(arglist[0], arglist);
        perror("execvp gpg");
        exit(EXIT_FAILURE);
    }

    // Parent process
    close(pipefdone[0]);
    close(pipefdone[1]);
    close(pipefdtwo[0]);
    close(pipefdtwo[1]);

    waitpid(tar_pid, NULL, 0);
    waitpid(gzip_pid, NULL, 0);
    waitpid(gpg_pid, NULL, 0);

    printf("Compression completed successfully.\n");
    return 0;
}
