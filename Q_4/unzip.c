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

  
    pid_t gpg_pid = fork();
    if (gpg_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (gpg_pid == 0) {
        // Child 1 (gpg)
        close(1); // Close stdout
        dup2(pipefdone[0], 1); // Redirect stdout to pipefdone[1]
        char *arglist[] = {"gpg", "--decrypt", argv[1], NULL};
        execvp(arglist[0], arglist);
        perror("execvp gpg");
        exit(EXIT_FAILURE);
    }
        waitpid(gpg_pid, NULL, 0);


    pid_t gunzip_pid = fork();
    if (gunzip_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (gunzip_pid == 0) {
        // Child 2 (gunzip)
        close(0); // Close stdin
        close(1); // Close stdout
        dup2(pipefdone[1], 0);  // Redirect stdin to pipefdone[0]
        dup2(pipefdtwo[0], 1); // Redirect stdout to pipefdtwo[1]
        char *filename = strtok(argv[1], ".gpg");  // extracts "folder.tar.gz" from "folder.tar.gz.gpg"
        char *temp_filename = strcat(filename, ".tar.gz");  // adds ".tar.gz" for gunzip
        printf("%s",temp_filename);
        char *arglist[] = {"gunzip", "-c", NULL};  
        execvp(arglist[0], arglist);
        perror("execvp gunzip");
        exit(EXIT_FAILURE);
    }

    pid_t tar_pid = fork();
    if (tar_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (tar_pid == 0) {
        // Child 3 (tar)
        close(0); // Close stdin
        dup2(pipefdtwo[1], 0); // Redirect stdin to pipefdtwo[0]


        char *arglist[] = {"tar", "-xvf", "-", NULL};
        execvp(arglist[0], arglist);
        perror("execvp tar");
        exit(EXIT_FAILURE);
    }

    // Parent process
    close(pipefdone[0]);
    close(pipefdone[1]);
    close(pipefdtwo[0]);
    close(pipefdtwo[1]);
    waitpid(gunzip_pid, NULL, 0);
    waitpid(tar_pid, NULL, 0);



    printf("Decompression completed successfully.\n");
    return 0;
}
