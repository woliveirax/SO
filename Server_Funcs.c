#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define buffer_size 300

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
  //TODO falta implementar a função q faz free do array de strings.
  for(int i = 0;array[i] != NULL;i++)
  {
    free(array[i]);
  }
  free(array);
}

char ** getComandAndArguments(char * string, char ** command)
{
  char *argument;
  char **arguments = NULL;
  char **aux;
  int argQuant = 0;

  //NOTE ARGUMENT AND ARGUMENTS
  for (*command = strtok(string," "),argument = strtok(NULL, " ");argument != NULL;)
  {
    aux = realloc(arguments,sizeof(char *) * (argQuant + 1));
    if(aux != NULL)
      arguments = aux;
    else
      return NULL;

    arguments[argQuant] = malloc(50 * sizeof(char));
    if(arguments[argQuant] == NULL)
    {
      freeSpace(arguments);
      return NULL;
    }
    strcpy(arguments[argQuant],argument);

    argQuant++;
    argument = strtok(NULL, " ");
  }

  aux = realloc(arguments,sizeof(char *) * (argQuant + 1)); //NOTE INSERE NULL NO ULTIMO PONTEIRO DA STRING PARA SABER O FIM.
  aux[argQuant] = NULL;
  arguments = aux;

  return arguments;
}

int handleCommand(char * call)
{
  char *commands[] = {"add","users","kick","game","shutdown","map"};
  char *command;
  char **arguments;

  arguments = getComandAndArguments(call,&command);

  if(strcmp(command,"help") == 0)
  {
      helpCommand();
      return 0; //TODO meter isto no fork
  }

  for(int i = 0; i < 6;i++)
  {
    if(strcmp(command,commands[i]) == 0)
    {
      //exit(0); //TODO falta fazer o fork aqui com os comandos
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

int main(int argc, char *argv[])
{
  console();
}
