#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char encodingMap[62];
} Codec;



void *createCodec(char key[62]) {
    Codec *codec = (Codec *)malloc(sizeof(Codec));
    if (codec == NULL) {
        perror("Failed to allocate memory for codec");
        return NULL;
    }

    int i, j;
    for (i = 0; i < 62; i++) {
        for (j = i + 1; j < 62; j++) {
            if (key[i] == key[j]) {
                free(codec);
                return NULL;
            }
        }
        codec->encodingMap[i] = key[i];
    }

    return codec;
}

int encode(char *textin, char *textout, int len, void *codec) {
    if (textin == NULL || textout == NULL || codec == NULL) {
        return -1;
    }

    Codec *polyCodec = (Codec *)codec;
    int i;
    for (i = 0; i < len; i++) {
        if (textin[i] >= 'a' && textin[i] <= 'z') {
            textout[i] = polyCodec->encodingMap[textin[i] - 'a'];
        } else if (textin[i] >= 'A' && textin[i] <= 'Z') {
            textout[i] = polyCodec->encodingMap[textin[i] - 'A' + 26];
        } else if (textin[i] >= '0' && textin[i] <= '9') {
            textout[i] = polyCodec->encodingMap[textin[i] - '0' + 52];
        } else {
            textout[i] = textin[i];
        }
    }

    textout[len] = '\0'; 

    return len;
}

int decode(char *textin, char *textout, int len, void *codec) {
    if (textin == NULL || textout == NULL || codec == NULL) {
        return -1;
    }

    Codec *polyCodec = (Codec *)codec;
    int i;
    for (i = 0; i < len; i++) {
        char currentChar = textin[i];

        int index;
        
        if (currentChar >= 'a' && currentChar <= 'z') {
            index = strchr(polyCodec->encodingMap, currentChar) - polyCodec->encodingMap;
        } else if (currentChar >= 'A' && currentChar <= 'Z') {
            index = strchr(polyCodec->encodingMap + 26, currentChar) - polyCodec->encodingMap;
        } else if (currentChar >= '0' && currentChar <= '9') {
            index = strchr(polyCodec->encodingMap + 52, currentChar) - polyCodec->encodingMap;
        } else {
            index = -1;
        }

        if (index >= 0) {
            textout[i] = 'a' + index;
        } else {
            textout[i] = currentChar;
        }
    }

    textout[len] = '\0'; 

    return len;
}

void freecodec(void *codec) {
    free(codec);
}

