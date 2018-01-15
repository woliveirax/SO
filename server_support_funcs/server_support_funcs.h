#ifndef SERVER_SUPPORT_FUNCS_H
#define SERVER_SUPPORT_FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ncurses.h>
#include "../comun_info.h"
#include "../server_support_funcs/server_support_funcs.h"
#include "../server/server_structures.h"

pthread_mutex_t map_token;
pthread_mutex_t map_send;

ClientsData * global_clients;
gameInfo * global_map;
Enemy *enemy;
int mapImported;

bool game;

int help(int argc, char *argv[]);
void loadMap(int argc, char * argv[]);
int addUser (int argc, char *argv[]);
void showCurrentUsers(int argc, char * argv[], ClientsData Data);
void serverShutdown();
int kickUser(int argc, char * argv[],ClientsData * Data);
void clearScreen();

#endif
