#include "server_support_funcs.h"
//NOTE ADD USER FUNCS

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
// NOTE kick user function

void removeClientFromServer(ClientsData * Data, char * username) //TODO ambiquidade
{
  for(int i = 0;i < Data->nClients; i++)
    if(strcmp(Data->clients[i].username,username) == 0)
    {
      close(Data->clients[i].FD);
      Data->clients[i] = Data->clients[(Data->nClients)-1];
      (Data->nClients)--;
    }
}

int sendKickToClient(Client cli)
{
  int msg = SERVER_KICK;

  if(write(cli.FD,&msg,sizeof(int)) <= 0)
  {
    perror("Nao foi possivel enviar a mensagem de kick ao utilizador: ");
    return -1;
  }
  return 0;
}

int kickUser(int argc, char * argv[],ClientsData * Data)
{
  if(argc != 2)
  {
    fprintf(stderr,"\nmodo de uso: %s [username]\n",argv[0]);
    return -1;
  }

  if(verifyLoggedPlayers(Data,argv[0]) == 0)
  {
    for(int i = 0; i < Data->nClients; i++)
      if(strcmp(Data->clients[i].username,argv[0]) == 0)
      {
        sendKickToClient(Data->clients[i]);
        removeClientFromServer(Data,argv[0]);
        return 0;
      }
  }
  else
  {
    printf("O utilizador '%s', nao se encontra no servido.",argv[0]);
    return -1;
  }
}

//####################################################################################
// NOTE show users function

void showCurrentUsers(int argc,ClientsData Data)
{
  if(argc != 1)
  {
    fprintf(stderr,"\nmodo de uso: %s\n",argv[0]);
    return -1;
  }
  printf("\n\n");
  printf("Clientes ligados: \n");

  for(int i = 0;i < Data.nClients;i++)
    printf(" %d -- %s\n",i+1,Data.clients[i].username);
}

//####################################################################################
// NOTE show users function

void serverShutdown(int argc, ClientsData * Data)
{
  if(argc != 1)
  {
    fprintf(stderr,"\nmodo de uso: %s\n",argv[0]);
    return -1;
  }

  unlink(SERVER_PIPE);//TODO TROCAR ISTO
  exit(0);
  //TODO fechar pipes cliente;
  //TODO unlink do pipe servidor
  //TODO acabar threads Servidor
}
