#ifndef SERVER_SUPPORT_FUNCS_H
#define SERVER_SUPPORT_FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "../comun_info.h"

#define file "../Data/users.data"
#define buf_size 1024
#define MAX 50


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


int help(int argc, char *argv[]);
int addUser (int argc, char *argv[]);
void showCurrentUsers(int argc,ClientsData Data);
void serverShutdown(int argc, ClientsData * Data);
int kickUser(int argc, char * argv[],ClientsData * Data);


#endif
