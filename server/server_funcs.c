#include "server_funcs.h"


void helpCommand()
{
  printf("\n\n\n");
  printf("Comandos Disponiveis:\n");

  printf("\nadd [username] [password]\n\t-Adiciona Utilizador");
  printf("\n\nusers - Mostra lista de jogadores ativos");
  printf("\n\nkick [username]\n\t-Remove um jogador do jogo");
  printf("\n\ngame\n\t-Mostra informacoes do jogo corrente:\n\t\t-Jogadores e as suas pontuações;\n\t\t-Objetos objetivos a apanhar");
  printf("\n\nshutdown \n\t-Termina o jogo atual e desliga o servidor.");
  printf("\n\nmap [nome do ficheiro]\n\t-Muda o mapa do jogo, a mudanca sera feita no fim do jogo atual, se este estiver a decorrer.\n");
}


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
  char *path = malloc( sizeof(char) * (strlen(command) + strlen(functions_path) + 1));

  strcat(path,(const char *) functions_path);
  strcat(path,(const char *) command);

  return path;
}


int handleCommand(char * str)
{
  char *commands[] = {"add","users","kick","game","shutdown","map"};
  char *command;
  char **arguments;

  arguments = getComandAndArguments(str,&command);

  for(int i = 0; arguments[i] != NULL; i++)
    puts(arguments[i]);

  if(strcmp(command,"help") == 0) // TODO meter isto no fork
  {
      helpCommand();
      return 0;
  }

  for(int i = 0; i < 6;i++)
  {
    if(strcmp(command,commands[i]) == 0)
    {
      command = getPath(command);

      if(fork() == 0)
        execvp(command,arguments);
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

  setbuf(stdout,NULL);
  printf("Server Console: [Admin]\n\n");
  helpCommand();

  while(1)
  {
    printf("\n$: ");

    fgets(buffer, buffer_size, stdin);

    if(handleCommand(buffer) < 0)
    {
      invalidCommand(buffer);
    }
  }
}
