#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define buffer_size 300

//Data handling functions
void freeSpace(char **array);
char ** getComandAndArguments(char * string, char ** command);
int handleCommand(char * call);

//Console functions
void invalidCommand(char * command);
void console();

#endif /* SERVER_H */
