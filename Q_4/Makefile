
CC = gcc
CFLAGS = -Wall

all: myzip myunzip

myzip: myzip.o 
	$(CC) $(CFLAGS) -o myzip myzip.o 

myunzip: myunzip.o 
	$(CC) $(CFLAGS) -o myunzip myunzip.o 

clean:
	rm -f myzip myunzip *.o
	
