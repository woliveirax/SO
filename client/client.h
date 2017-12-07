#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#define FILE_USERS "../Data/users.data"
#define MAX 50

//estrutura com informaçao relativa a cada cliente;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct Client_Data
{
  pthread_t RECEIVE_THREAD;
  pthread_cond_t AWAITED_REPLY_LOGIN;
  pthread_mutex_t LOCK_LOGIN;
  int LOGIN_CONFIRMATION;
  int FD_SERVER_PIPE;
  int FD_CLIENT_PIPE;
  char CLIENT_PIPE[MAX];
  int PID;

}Client_data;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Client_options(Client_data *info);
int main(int argc, char * argv[]);

#endif /* CLIENT_H */
