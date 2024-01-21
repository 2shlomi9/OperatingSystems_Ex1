#include <stdio.h>
#include <stdlib.h>
#include "polyencoder.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s src_file dst_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *srcFileName = argv[1];
    char *dstFileName = argv[2];

    FILE *srcFile = fopen(srcFileName, "r");
    if (srcFile == NULL) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    FILE *dstFile = fopen(dstFileName, "w");
    if (dstFile == NULL) {
        perror("Error opening destination file");
        fclose(srcFile);
        exit(EXIT_FAILURE);
    }


    void *codec = createCodec("defghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abc");

    if (codec == NULL) {
        fprintf(stderr, "Invalid key. Exiting.\n");
        fclose(srcFile);
        fclose(dstFile);
        exit(EXIT_FAILURE);
    }


    char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), srcFile)) > 0) {
        char decodedBuffer[1024];
        int decodedBytes = decode(buffer, decodedBuffer, bytesRead, codec);
        fwrite(decodedBuffer, 1, decodedBytes, dstFile);
    }


    freecodec(codec);
    fclose(srcFile);
    fclose(dstFile);

    return 0;
}