#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "server_funcs.h"
#include "../server_support_funcs/server_support_funcs.h"

void HandleSignal(int s)
{
  if(s == SIGINT)
  {
    printf("Por favor, utilize o comando \'shutdown\' para desligar o servidor!\n");
  }

  if(s == SIGUSR1)
  {
    serverShutdown();
  }
}

gameInfo * createMapPackage()
{
  gameInfo * temp = malloc(sizeof(gameInfo));
  if(temp == NULL)
  {
    perror("Não foi possível alocar memória para o pacote do mapa!: ");
    exit(0);
  }
  return temp;
}

int main(int argc, char *argv[])
{
  signal(SIGUSR1,HandleSignal);
  signal(SIGINT,HandleSignal);

  ClientsData data;
  data.nClients = 0;
  global_clients = &data;
  global_map = createMapPackage();

  clearScreen();

  pthread_t thread_id;
  pthread_create(&thread_id,NULL,pipeMain,(void *) &data);

  console(&data);
}
