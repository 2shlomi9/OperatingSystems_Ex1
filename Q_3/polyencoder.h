#ifndef POLYENCODER_H
#define POLYENCODER_H

void *createCodec(char key[62]);
int encode(char *textin, char *textout, int len, void *codec);
int decode(char *textin, char *textout, int len, void *codec);
void freecodec(void *codec);

#endif 