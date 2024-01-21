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

    textout[len] = '\0'; // Null-terminate the encoded string

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

    textout[len] = '\0'; // Null-terminate the decoded string

    return len;
}

void freecodec(void *codec) {
    free(codec);
}

int main() {
    void *codec = createCodec("defghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abc");

    char txtsrc[] = "shlomi 7ka sali 7ka matan melamed 7ka ben 7ka harel";
    char txtencode[53]; // Include space for null terminator

    encode(txtsrc, txtencode, 52, codec);
    printf("encode %s:\n%s\n", txtsrc, txtencode);

    char txtdecode[53]; // Include space for null terminator
    decode(txtencode, txtdecode, 52, codec);
    printf("decode %s:\n%s\n", txtencode, txtdecode);

    freecodec(codec);

    return 0;
}
