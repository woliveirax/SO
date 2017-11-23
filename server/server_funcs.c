#include "server_funcs.h"

void freeSpace(char **array)
{
  for(int i = 0;array[i] != NULL;i++)
  {
    free(array[i]);
  }
  free(array);
}

//TODO tentar melhorar se houver tempo;
char ** getComandAndArguments(char * string, char ** command)
{
  char *argument;
  char **arguments = NULL;
  char **aux;
  int argQuant = 0;

  *command = strtok(string," \n");

  aux = realloc(arguments,sizeof(char *) * (argQuant + 1));
  if(aux != NULL)
    arguments = aux;
  else
    return NULL;

  arguments[argQuant] = malloc(50 * sizeof(char));
  if(arguments[argQuant] == NULL)
  {
    free(arguments);
    return NULL;
  }

  strcpy(arguments[argQuant],*command);
  argQuant++;

  //NOTE ARGUMENT AND ARGUMENTS
  for (argument = strtok(NULL, " \n");argument != NULL;)
  {
    aux = realloc(arguments,sizeof(char *) * (argQuant + 1));
    if(aux != NULL)
      arguments = aux;
    else
    {
      freeSpace(arguments);
      return NULL;
    }

    arguments[argQuant] = malloc(50 * sizeof(char));
    if(arguments[argQuant] == NULL)
    {
      freeSpace(arguments);
      return NULL;
    }
    strcpy(arguments[argQuant],argument);

    argQuant++;
    argument = strtok(NULL, " \n");
  }

  aux = realloc(arguments,sizeof(char *) * (argQuant + 1)); //NOTE INSERE NULL NO ULTIMO PONTEIRO DA STRING PARA SABER O FIM.
  aux[argQuant] = NULL;
  arguments = aux;

  return arguments;
}


char * getPath (char * command)
{
  char *path = malloc( sizeof(char) * (strlen(command) + 3));

  strcat(path,(const char *) "./");
  strcat(path,(const char *) command);

  return path;
}


int handleCommand(char * str)
{
  char *commands[] = {"add","users","kick","game","shutdown","map","help",NULL};
  char *command;
  char **arguments;

  arguments = getComandAndArguments(str,&command);

  for(int i = 0; commands[i] != NULL;i++)
  {
    if(strcmp(command,commands[i]) == 0)
    {
      command = getPath(command);

      if(fork() == 0)
      {
        if(chdir("../server_support_funcs") < 0)
          perror("Erro chdir: ");

        execvp(command,arguments);
      }
      else
        wait(NULL);

      free(command);
      freeSpace(arguments);
      return 0;
    }
  }
  freeSpace(arguments);
  return -1;
}


void invalidCommand(char * command)
{
  fprintf(stderr,"O comando:\' %s \' nao existe. utilize help para ajuda!",command);
}

void console()
{
  char buffer[buffer_size];

  //TODO trata sinais
  setbuf(stdout,NULL);
  printf("\n\nServer Console: [Admin]\n");

  while(1)
  {
    printf("\n$: ");

    fscanf(stdin," %299[^\n]",buffer);

    if(handleCommand(buffer) < 0)
    {
      invalidCommand(buffer);
    }
  }
}
