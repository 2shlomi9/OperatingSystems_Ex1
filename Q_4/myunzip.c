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

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t gpg_pid = fork();
    if (gpg_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (gpg_pid == 0) {
        // Child 1 (gpg)
        close(1);  // Close stdout
        dup2(pipefd[1], 1); // Redirect stdout to pipefd[1]
        close(pipefd[0]);  // Close unused pipe end
        char *arglist[] = {"gpg", "--decrypt", argv[1], NULL};
        execvp(arglist[0], arglist);
        perror("execvp gpg");
        exit(EXIT_FAILURE);
    }

    // Parent
    close(pipefd[1]);

    char tempfile[] = "/tmp/myunzip.XXXXXX";
    int tempfd = mkstemp(tempfile);
    if (tempfd == -1) {
        perror("mkstemp");
        exit(EXIT_FAILURE);
    }

    pid_t gunzip_pid = fork();
    if (gunzip_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (gunzip_pid == 0) {
        // Child 2 (gunzip)
        close(0);  // Close stdin
        dup2(pipefd[0], 0);  // Redirect stdin to pipefd[0]
        close(pipefd[1]);  // Close unused pipe end
        close(1);  // Close stdout
        dup2(tempfd, 1);  // Redirect stdout to temp file
        char *arglist[] = {"gunzip", "-", NULL};
        execvp(arglist[0], arglist);
        perror("execvp gunzip");
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    close(tempfd);  // Close temp file in parent

    waitpid(gpg_pid, NULL, 0);
    waitpid(gunzip_pid, NULL, 0);

    // Parent (after gunzip completes)
    pid_t tar_pid = fork();
    if (tar_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (tar_pid == 0) {
        // Child 3 (tar)
        char *arglist[] = {"tar", "-xvf", tempfile, NULL};
        execvp(arglist[0], arglist);
        perror("execvp tar");
        exit(EXIT_FAILURE);
    }

    waitpid(tar_pid, NULL, 0);
    unlink(tempfile);  // Delete temp file after tar completes

    printf("Decompression completed successfully.\n");
    return 0;
}
