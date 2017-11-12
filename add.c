#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define file "users.data"
#define buf_size 1024
#define MAX 50

void errorx(char * msg)
{
  fprintf(stderr,"%s\n",msg);
  exit(1);
}


void addUser(char *user, char *pass)
{
  FILE *fd;

  if((fd = fopen(file,"at")) == NULL)
    errorx("Erro ao abrir o ficheiro!");

    fprintf(fd,"%s %s\n",user,pass);
}

int verifyUserFile()
{
  int fd;

  if(access(file,F_OK) < 0)
  {
      fd = open(file, O_WRONLY | O_CREAT,0664);
      close(fd);
      return -1;
  }
  return 0;
}

int verifyUserExistence(char * user, char *pass)
{
  FILE * fd;
  char u[MAX],p[MAX];

  if((fd = fopen(file,"rt")) == NULL)
    errorx("Erro ao abrir o ficheiro!");

  while(fscanf(fd," %s %s ",u,p) == 2)
  {
    if(strcmp(user,u) == 0)
      return 1;
  }
  fclose(fd);
}


int main (int argc, char *argv[])
{

  if(argc != 3)
  {
    printf("Modo de uso: %s username password",argv[0]);
    exit(1);
  }

  if(verifyUserFile() >= 0)
  {
    if(verifyUserExistence(argv[1],argv[2]) == 1)
      errorx("O utilizador ja existe");

    addUser(argv[1],argv[2]);
    printf("Utilizador \'%s\' adicionado com sucesso!\n",argv[1]);
  }
  else
  {
    addUser(argv[1],argv[2]);
    printf("Utilizador \'%s\' adicionado com sucesso!\n",argv[1]);
  }

  return 0;
}
