#include "Serve_Funcs.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define buffer_size 300

char * strUpper(char * str)
{
  char * a = malloc(sizeof(str));
  
  for(;a != NULL;a++)
    toupper(a);

  return(a)
}

int strcmp_upperCase(char *str1, char *str2) //
{
  char *strA,*strB;




  return strcmp(strUpper(strA),strUpper(strB));
}

void handleCommand(char * call)
{
  char *command, *saveptr;
  command = strtok_r(call, " ", &saveptr);


  strcmp(command,"add")

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
