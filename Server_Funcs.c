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

int handleCommand(char * call)
{
  char *commands[] = {"add","users","kick","game","shutdown","map"};
  char *command;

  command = strtok(call, " ");

  if(strcmp(command,"help") == 0)
    helpCommand();

  for(int i = 0; i < 6;i++)
  {
    if(strcmp(command,commands[i]) == 0)
    {
      exit(0); //TODO falta fazer o fork aqui com os comandos
      return 0;
    }
  }
  return -1;
}

void invalidCommand(char * command)
{
  command = strtok(command," ");
  fprintf(stderr,"O comando:\'%s\' nao existe.\nutilize help para ajuda!",command);
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
      scanf(" %299s",buffer);

      if(handleCommand(buffer) < 0)
      {
        invalidCommand(buffer);
        setbuf(stdin, NULL);
      }

  }
}

int main(int argc, char *argv[])
{
  console();
}
