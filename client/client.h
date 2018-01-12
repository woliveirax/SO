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
#include <ncurses.h>
#include <signal.h>


#define FILE_USERS "../Data/users.data"
#define MAX 50

//estrutura com informaçao relativa a cada cliente;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct Client_Data
{
  pthread_t RECEIVE_THREAD;           //ID da thread a receber
  pthread_cond_t AWAITED_REPLY_LOGIN; //Condicao da thread
  pthread_mutex_t LOCK_LOGIN;         //mecanismo de espera de resposta
  int LOGIN_CONFIRMATION;             //Resposta do servidor
  int FD_SERVER_PIPE;                 //FD do servidor
  int FD_CLIENT_PIPE;                 //FD do cliente
  char CLIENT_PIPE[MAX];              //Nome do pipe do cliente
  int PID;
  WINDOW *MAPVIEWER;                  // Janela para ver Mapa;
  WINDOW *TIMEGAMEVIEWER;             // janela para ver o tempo de jogo
  WINDOW *LOGOGAME;                   // janela onde estará o nome do jogo
  WINDOW *INFOGAME;                   // janela para ver informaçao relativa ao jogo
  WINDOW *CHATVIEWER;                  // janela para ver informaçao digitada pelo utilizador;
  WINDOW *CHATLIMVIEWER;
  WINDOW *CHATWRITER;                 // Janela para enviar mensagem para CHAT pelo utilizador;

}Client_data;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Client_options(Client_data *info);
int main(int argc, char * argv[]);

#endif /* CLIENT_H */
