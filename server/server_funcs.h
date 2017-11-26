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
#include "../comun_info.h"

#define MAX_CLI 20
#define MAX 50
#define buffer_size 300



typedef struct client {
  int PID;
  int FD;
  char username[MAX];
  Player *player;

} Client;

typedef struct RCVED_DATA {
  Client clients[MAX_CLI];
  int nClients;
}ClientsData;

//Data handling functions
void freeSpace(char **array);
char ** getComandAndArguments(char * string, char ** command);
int handleCommand(char * call);

//Console functions
void invalidCommand(char * command);
void console();

#endif /* SERVER_H */
