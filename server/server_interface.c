#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../server/server_funcs.h"

int main(int argc, char *argv[])
{
  ClientsData data;
  data.nClients = 0;
  
  pipeMain(&data);

  //console();
}
