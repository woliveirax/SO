#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int verify_player( char *argv, char *jogador)
{
  if(strcmp(argv, jogador) == 0)
    return 1;
  else
    return 0;
}

  void main(int argc, char *argv[1])
  {
    int i = 0;
    char *jogador[]={"joao","Miguel","Afonso","Tina","Tio"};

    if(argc != 2)
    {
      printf("Modo de uso: %s username ",argv[0]);
      exit(1);
    }
    while (i != 5){

      if (verify_player(argv[1], jogador[i]) == 1){
        printf("o jogador %s existe e será apagado\n", argv[1]);
        exit(0);
      }
      i++;
      }
      printf("Jogador %s nao existe\n", argv[1]);
      exit(1);
    }
