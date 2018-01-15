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

gameInfo * initMapPackage()
{
  gameInfo * temp = malloc(sizeof(gameInfo));
  if(temp == NULL)
  {
    perror("Não foi possível alocar memória para o pacote do mapa!: ");
    exit(0);
  }

  memset(temp,0,sizeof(gameInfo));

  for(int x = 0; x < 21; x++)
  {
    for(int y = 0; y < 31; y++)
    {
      temp->map[x][y].player.posx = x;
      temp->map[x][y].player.posy = y;
      }
  }

  game = false;
  return temp;
}

int main(int argc, char *argv[])
{
  signal(SIGUSR1,HandleSignal);
  signal(SIGINT,HandleSignal);

  pthread_mutex_init(&map_token,NULL);
  pthread_mutex_init(&map_send,NULL);

  global_map = initMapPackage();
  global_clients = malloc(sizeof(ClientsData));
  global_clients->nClients = 0;
  mapImported = 0;

  clearScreen();

  pthread_t thread_id;
  pthread_create(&thread_id,NULL,pipeMain,(void *) global_clients);

  console(global_clients);
}
