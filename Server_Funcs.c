#include "Serve_Funcs.h"

#define buffer_size 300


void handleCommand(char * command)
{
  
}

void console()
{
  char buffer[buffer_size];

  setbuf(stdout,NULL);
  printf("Server Console: [Admin]");

  while(1)
  {
      printf("$: ");
      scanf(" %299s",buffer);

      handleCommand(buffer);
  }
}

int main(int argc, char *argv[])
{
  console();
}
