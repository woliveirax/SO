#include "server_support_funcs.h"
//NOTE ADD USER FUNCS

void addUserToFile(char *user, char *pass)
{
  FILE *fd;

  if((fd = fopen(users_file,"at")) == NULL)
  {
    perror("Erro: ");
  }
  else
  {
    fprintf(fd,"%s %s\n",user,pass);
    fclose(fd);
  }
}

int verifyUserFile()
{
  int fd;

  if(access(users_file,F_OK) < 0)
  {
      fd = open(users_file, O_WRONLY | O_CREAT,0664);
      close(fd);
      return -1;
  }
  return 0;
}

int verifyUserExistence(char * user, char *pass)
{
  FILE * fd;
  char u[MAX],p[MAX];

  if((fd = fopen(users_file,"rt")) == NULL)
  {
    perror("Erro ao abrir ficheiro: ");
    return -1;
  }

  while(fscanf(fd," %s %s ",u,p) == 2)
  {
    if(strcmp(user,u) == 0)
    {
      fclose(fd);
      return 1;
    }
  }
  fclose(fd);
  return 0;
}


int addUser (int argc, char *argv[])
{

  if(argc != 3)
  {
    printf("Modo de uso: %s username password\n",argv[0]);
    return 1;
  }

  if(verifyUserFile() == 0)
  {
    if(verifyUserExistence(argv[1],argv[2]) == 1)
    {
      printf("O utilizador '%s' ja existe",argv[1]);
      return -1;
    }

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
  printf("\n\n env - Shows all environment variables as well as their value, created by the administrator\n");

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
      printf("O cliente \'%s\' foi removido do servidor!",username);
    }
}

int checkIfUserOn(ClientsData * Data, char * user)
{
  if(Data->nClients == 0)
  {
    printf("Não existem utilizadores ligados ao servidor...");
    return -1;
  }

  for(int i = 0; i < Data->nClients ; i++)
  {
    if(strcmp(Data->clients[i].username, user) == 0)
      return i;
  }

  printf("O cliente \'%s\' não se encontra ligado.",user);
  return -1;
}

int sendKickToClient(Client cli)
{
  gameInfo kick_user;

  kick_user.type = SERVER_KICK;

  if(write(cli.FD,&kick_user,sizeof(gameInfo)) <= 0)
  {
    perror("Nao foi possivel enviar a mensagem de kick ao utilizador: ");

    return -1;
  }
  return 0;
  }


int kickUser(int argc, char * argv[],ClientsData * Data)
{
  int pos;

  if(argc != 2)
  {
    fprintf(stderr,"\nmodo de uso: %s [username]\n",argv[0]);
    return -1;
  }

  if((pos = checkIfUserOn(Data,argv[1])) >= 0)
  {
    sendKickToClient(Data->clients[pos]);

    removeClientFromServer(Data,argv[1]); //TODO alterar isto para mandar POS.

    return 0;
  }
  else
  {
    return -1;
  }
}

//####################################################################################
// NOTE show users function

void showCurrentUsers(int argc, char * argv[], ClientsData Data)
{
  if(argc != 1)
  {
    fprintf(stderr,"\nmodo de uso: %s\n",argv[0]);
    return;
  }

  if(Data.nClients == 0)
  {
    printf("Não existem utilizadores ligados ao servidor...");
    return;
  }

  printf("\n\n");
  printf("Clientes ligados: \n");

  for(int i = 0;i < Data.nClients;i++)
  {
    printf(" %d ->" BOLDYELLOW " %s ",i+1,Data.clients[i].username);
    if(Data.clients[i].inGame)
      printf(BOLDGREEN "%s\n" RESET,"InGame");
    else
      printf(BOLDCYAN "%s\n" RESET,"Online");
  }
}

//####################################################################################
// NOTE show users function
void sendfromserverShutdown(ClientsData * Data)
{
    int i = 0;

    gameInfo Server_Shutdown;

    Server_Shutdown.type = SERVER_SHUTDOWN;

    while(i < Data->nClients)
    {
      if(write(Data->clients[i].FD,&Server_Shutdown,sizeof(gameInfo)) <= 0)
      {
        printf("Nao foi possivel enviar a mensagem de Shutdown ao utilizador %s\n",Data->clients[i].username);
        return;
      }
      i++;
    }
    return;
  }
void serverShutdown()
{
  sendfromserverShutdown(global_clients);
  unlink(SERVER_PIPE);//TODO TROCAR ISTO
  free(global_map);
  free(global_clients);
  free(enemy);
  exit(0);
  //TODO fechar pipes cliente;
  //TODO unlink do pipe servidor
  //TODO acabar threads Servidor
}

//####################################################################################
// NOTE show users function
void clearScreen()
{
  printf("\033c");
}
