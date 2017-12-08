#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "server_funcs.h"
#include "../server_support_funcs/server_support_funcs.h"

int main(int argc, char *argv[])
{
  ClientsData data;
  data.nClients = 0;

  clearScreen();

  pthread_t thread_id;
  pthread_create(&thread_id,NULL,pipeMain,(void *) &data);

  console(&data);
}
