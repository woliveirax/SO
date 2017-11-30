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
#include "../comun_info.h"

#define MAX_CLI 20
#define MAX 50
#define buffer_size 300

typedef struct Bomb {
  int nBombs;
  int posx,posy;
  int explosionSize; //Tamanho explosao
}Bomb;

typedef struct player {
  int PID;
  int posx,posy;
  int score;
  Bomb nuke;
  Bomb grenade;
} Player;

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
