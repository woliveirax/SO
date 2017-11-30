#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

//NOTE ADD USER FUNCS
#define file "../Data/users.data"
#define buf_size 1024
#define MAX 50

void addUserToFile(char *user, char *pass)
{
  FILE *fd;

  if((fd = fopen(file,"at")) == NULL)
    perror("Erro: ");

    fprintf(fd,"%s %s\n",user,pass);
}

int verifyUserFile()
{
  int fd;

  if(access(file,F_OK) < 0)
  {

  }
  return 0;
}

int verifyUserExistence(char * user, char *pass)
{
  FILE * fd;
  char u[MAX],p[MAX];

  if((fd = fopen(file,"rt")) == NULL)
    perror("Erro ao abrir ficheiro: ");

  while(fscanf(fd," %s %s ",u,p) == 2)
  {
    if(strcmp(user,u) == 0)
      return 1;
  }
  fclose(fd);
}


int addUser (int argc, char *argv[])
{

  if(argc != 3)
  {
    printf("Modo de uso: %s username password",argv[0]);
    exit(1);
  }

  if(verifyUserFile() == 0)
  {
    if(verifyUserExistence(argv[1],argv[2]) == 1)
      printf("O utilizador '%s' ja existe",argv[1]);

    addUserToFile(argv[1],argv[2]);
    printf("Utilizador \'%s\' adicionado com sucesso!\n",argv[1]);
  }
  else
  {
    addUserToFile(argv[1],argv[2]);
    printf("Utilizador \'%s\' adicionado com sucesso!\n",argv[1]);
  }

  return 0;
}


//####################################################################################
// NOTE HELP FUNCS

int help(int argc, char *argv[])
{
  if(argc != 1)
  {
    printf("\nModo de uso: %s\n",argv[0]);
    return -1;
  }

  printf("\n\n\n");
  printf("Comandos Disponiveis:\n");

  printf("\nadd [username] [password]\n\t-Adiciona Utilizador");
  printf("\n\nusers - Mostra lista de jogadores ativos");
  printf("\n\nkick [username]\n\t-Remove um jogador do jogo");
  printf("\n\ngame\n\t-Mostra informacoes do jogo corrente:\n\t\t-Jogadores e as suas pontuações;\n\t\t-Objetos objetivos a apanhar");
  printf("\n\nshutdown \n\t-Termina o jogo atual e desliga o servidor.");
  printf("\n\nmap [nome do ficheiro]\n\t-Muda o mapa do jogo, a mudanca sera feita no fim do jogo atual, se este estiver a decorrer.\n");

  return 0;
}

//####################################################################################
// NOTE SHOW USER FUNC
