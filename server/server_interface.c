#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../server/server_funcs.h"

int main(int argc, char *argv[])
{
  ClientsData data;
  data.nClients = 0;

  //pthread_t thread_id;

  //pthread_create(&thread_id,NULL,pipeMain,(void *) &data);
  //pipeMain(&data);

  console();
}
