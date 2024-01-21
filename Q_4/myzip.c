#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void executeCommand(const char *command, const char *arg1, const char *arg2) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        execlp(command, command, arg1, arg2, NULL);
        perror("execlp");
        _exit(EXIT_FAILURE);
    }
    

    // Parent process
    int status;
    waitpid(pid, &status, 0);

    if (!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS) {
        fprintf(stderr, "Error: %s process failed\n", command);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s directory\n", argv[0]);
        exit(EXIT_FAILURE);
    }

   
    char tempTarFile[] = "/tmp/myzip_tempXXXXXX";
    int tarFd = mkstemp(tempTarFile);
    if (tarFd == -1) {
        perror("mkstemp");
        exit(EXIT_FAILURE);
    }

    
    executeCommand("tar", "-cf", tempTarFile);

    
    executeCommand("compress", tempTarFile, NULL);

    
    executeCommand("gpg --output output.gpg --encrypt", tempTarFile, NULL);

    printf("myzip completed successfully.\n");

    // Clean up temporary file
    unlink(tempTarFile);

    return 0;
}