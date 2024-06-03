# Operating Systems (OSs) Course Assignment 1
### For Computer Science B.S.c at Ariel University

**By Shlomi Zecharia**

## Description

The task involves creating a multi-process compression and encryption tool similar to zip, using Linux system calls and execv.

First of all: set up a Unix-compatible environment on your computer. You can work with Mac or with WSU, but it's preferable to work with a virtual machine running Ubuntu.

### Bug Fixing Exercise (Q_1):

**bag_a.c :** a program that crashes due to division by zero.

For your assigned bug:
- Compile the program with and without debug flags.
- Run the program (let it crash).
- Demonstrate the steps required to obtain a core dump.
- Open the core dump using gdb and demonstrate the differences in output (using copy-paste or screenshots) between running with and without debugging.
- Demonstrate finding the relevant line of code where the crash occurred using the core dump.
- Open the core dump using a graphical debugger (e.g., ddd).
- Attached is a pdf file (debugger.pdf) that explains the use of the debugger

### Working with Libraries (Q_2):

**pythagorean.c :** a program that takes three side lengths from the user and checks if they form a Pythagorean triple.
- If it's a Pythagorean triple, the program should print the angles of the triangle in radians.
- If it's not a Pythagorean triple, the program should print an error message.
in this program we work with library math.h

## Building 
```
make all
```
## Running
```
./pifagor3
```

### Writing a library (Q_3):

We will write a program that produces a polyalphabetic encoder.

The encoder has 4 functions:

**createCodec(char key[62]):** Creates a data structure for the polyalphabetic encoding, containing the key according to which the encoding will be performed.

- This function initializes and returns a codec structure used for polyalphabetic encoding.
- It accepts a key of length 62 characters, which defines the mapping of each character to its corresponding cipher.
- The key consists of 26 lowercase letters, followed by 26 uppercase letters, and then 10 digits (from 1 to 0).
- For example, the following key generates a Caesar cipher with a shift of +3:
  defghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abc.
- If the provided key is invalid (e.g., contains duplicate characters), the function returns NULL;
  otherwise, it returns a pointer to the initialized codec structure.

**encode(char *textin, char *textout, int len, void *codec)**: A function that encrypts textin into textout using the codec (which will be started using createCodec)

- This function encodes the input text (textin) into the output text (textout) using the provided codec.
- The len parameter specifies the length of the input text.
- Returns the number of characters encoded or -1 in case of failure (e.g., if textin is NULL).


**decode(char *textin, char *textout, int len, void *codec)**: A function that decodes textin into textout using the codec (which will be started using createCodec)

- This function decodes the input text (textin) into the output text (textout) using the provided codec.
- The len parameter specifies the length of the input text.
- Returns the number of characters decoded or -1 in case of failure (e.g., if textin is NULL).



The folder contains the following files:

  
**polyencoder.c :** Contains the implementation of the polyalphabetic encoding functions

**polyencoder.h :** Header file declaring the functions and structures used in polyencoder.c

**encode.c :** Program for encoding a source file using polyalphabetic encoding

**decode.c :** Program for decoding an encoded source file

## Building 
```
make all
```

## Running

```
./encode srcfile destfile key

./decode srcfile destfile key

```

### Working with processes (Q_4):
In this part we creating two programs, myzip and myunzip. 
Will perform compression and decompression tasks similar to the functionalities of the zip and unzip commands.

We're utilizing the fork() system call to create a child process. 
This child process will handle the compression or decompression task while the parent process manages the overall flow of the program.
Additionally, we're using pipe() to create a unidirectional communication channel between processes/

The code will implement a sequence of operations similar to "tar | gz | gpg". Each process will be created using fork, and data transfer will be facilitated using pipes. 
The final result will be a file that underwent all these processes: it will first be untarred, then compressed, and finally encrypted.

**myzip.c :** A program that compresses a directory using the tar, gzip, and gpg commands.
- This program takes a directory as a command-line argument and compresses it into a single file using the tar, gzip, and gpg utilities.
- It creates two pipes for inter-process communication.
- It forks three child processes: one for tar, one for gzip, and one for gpg.
- The tar child process archives the specified directory and writes the output to a pipe.
- The gzip child process reads from the first pipe, compresses the data using gzip, and writes the compressed data to a second pipe.
- The gpg child process reads from the second pipe, encrypts the data using gpg, and writes the encrypted data to a file.
- The parent process waits for all child processes to finish, closes the pipes, and prints a success message.

**myunzip.c :** A program that decompresses and extracts a file using the gpg, gunzip, and tar commands.

- This program takes a compressed file as a command-line argument and decompresses it into its original form using the gpg, gunzip, and tar utilities.
- It creates two pipes for inter-process communication.
- It forks three child processes: one for gpg, one for gunzip, and one for tar.
- The gpg child process decrypts the input file and writes the decrypted data to a pipe.
- The gunzip child process reads from the first pipe, decompresses the data using gunzip, and writes the decompressed data to a second pipe.
- The tar child process reads from the second pipe, extracts the data using tar, and writes the extracted files to the current directory.
- The parent process waits for all child processes to finish, closes the pipes, and prints a success message.


Attached is a check folder on which we will run myzip.c
Attached is a file folder.tar.jz.gpg that will be activated with the help of myunzip.c

## Building 
```
make all
```
## Running
```
./myzip check
./myunzip folder.tar.gz.gpg
```
