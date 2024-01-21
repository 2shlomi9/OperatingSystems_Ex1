
CC = gcc
CFLAGS = -Wall

all: encode decode

encode: encode.o polyencoder.o
	$(CC) $(CFLAGS) -o encode encode.o polyencoder.o

decode: decode.o polyencoder.o
	$(CC) $(CFLAGS) -o decode decode.o polyencoder.o

polyencoder.o: polyencoder.c polyencoder.h
	$(CC) $(CFLAGS) -c polyencoder.c

clean:
	rm -f encode decode *.o
