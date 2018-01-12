#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include <ncurses.h>

#include "../comun_info.h"
#include "server_structures.h"

#define MAX 50
#define buffer_size 300

//Pipe Functions
void * pipeMain(void * rcvData);

//Data handling functions
void freeSpace(char **array);
char ** getComandAndArguments(char * string, char ** command, int * argc);
void handleCommand(char * str, ClientsData * Data);

//Console functions
void invalidCommand(char * command);
void console(ClientsData * Data);

#endif /* SERVER_H */
