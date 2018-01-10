#include "server_funcs.h"
#include "../comun_info.h"
#include "../server_support_funcs/server_support_funcs.h"

//NOTE Named pipes
int criaServerPipe()
{
  if(access(SERVER_PIPE,F_OK) < 0)
  {
    if(mkfifo(SERVER_PIPE,0664) < 0)
    {
      perror("Erro ao criar pipe no servidor: ");

      return -1;
    }
  }
  else
  {
      printf("Uma instancia do servidor já se econtra inicializada!\n");

      exit(1);
  }

  return 0;
}


int verifyLoggedPlayers(ClientsData * Data, Login *login)
{
  for(int i = 0; i < Data->nClients; i++)
    if(strcmp(Data->clients[i].username,login->username) == 0)
      return -1;

  return 0;
}

int credentialValidation(FILE *f,Login *login)
{
  Login temp;

  while(fscanf(f," %s %s ",temp.username,temp.password) == 2)
  {
    if(strcmp(temp.username, login->username) == 0)
      if(strcmp(temp.password,login->password) == 0)
        return USER_LOGIN_ACCEPTED;
      else
        return USER_LOGIN_WRONG_PASS;
  }
}

int verifyPlayerCredentials(Login *login_request)
{
  int ret;

  if(access(USERS_LOGIN_DATA,F_OK) < 0)
    return USER_DOESNT_EXIST;

  FILE *f;

  if((f = fopen(USERS_LOGIN_DATA,"rt")) == NULL)
  {
    perror("Erro ao abrir o ficheiro de utilizadores: ");
    return -1;
  }

  ret = credentialValidation(f,login_request);
  fclose(f);
  return ret;
}

int openClientFD(Client * cli)
{
  char path[100];
  sprintf(path,CLIENT_PIPE_TEMPLATE,cli->PID);

  if((cli->FD = open(path,O_WRONLY)) == -1)
  {
    perror("Error opening client FD: ");
    return -1;
  }

  return 0;
}


void addClientToArray(ClientsData * Data, Client cli)
{
  Data->clients[Data->nClients] = cli;
  (Data->nClients)++;
}


void copyLoginToClient(Client * cli, Login *login, int PID)
{
  strcpy(cli->username,login->username);
  cli->PID = PID;
  cli->player = NULL;

  openClientFD(cli);
}

int verifyPlayerLoginRequest(ClientsData *Data,Client * cli,int serverFD, Login *login_request, int PID)
{
  if(Data->nClients == 20)
    return SERVER_FULL;

  copyLoginToClient(cli,login_request, PID);

  if(verifyPlayerCredentials(login_request) == 0)
    if(verifyLoggedPlayers(Data, login_request) == 0)
      return USER_LOGIN_ACCEPTED;
    else
      return USER_ALREADY_IN;
}

void removeClientFromArray(ClientsData * Data, char * username)
{
  for(int i = 0;i < Data->nClients; i++)
    if(strcmp(Data->clients[i].username,username) == 0)
    {
      close(Data->clients[i].FD);
      Data->clients[i] = Data->clients[(Data->nClients)-1];
      (Data->nClients)--;
    }
}

int authentication(ClientsData * Data,int serverFD, Login *login_request, int PID)
{
  int response;
  Client cli;

  gameInfo answer_login;

  response = verifyPlayerLoginRequest(Data,&cli,serverFD, login_request, PID);

  answer_login.cellType = cellType_LOGIN_RESPONSE;
  answer_login.info.login_answer = response;

  if(response == USER_LOGIN_ACCEPTED)
  {
    addClientToArray(Data,cli);

    if(write(cli.FD,&answer_login,sizeof(gameInfo)) < 0)
    {
      removeClientFromArray(Data,cli.username);
      return -1;
    }
  }
  else
  {
    if(write(cli.FD,&answer_login,sizeof(gameInfo)) < 0)
    {
      close(cli.FD);
      return -1;
    }

    close(cli.FD);
  }

  return 0;
}

//NOTE: Remove user Function
// TODO
void removeUserByPID(ClientsData * Data, int PID)
{
  for(int i = 0; i < Data->nClients; i++)
    if(Data->clients[i].PID == PID)
    {
      gameInfo SERVER_ANSWER_LOG;   // resposta ao servidor que vai sair do login
      SERVER_ANSWER_LOG.cellType = cellType_LOGOUT_RESPONSE;

      write(Data->clients[i].FD, &SERVER_ANSWER_LOG, sizeof(gameInfo));

      Data->clients[i] = Data->clients[(Data->nClients)-1];

      (Data->nClients)--;
    }
}

void readData(ClientsData * Data,int serverFD)
{
  int type = 69;

  Package_Cli package_cli;

  read(serverFD,&package_cli,sizeof(Package_Cli));

  switch(package_cli.TYPE)
  {
    case USER_AUTH:
      authentication(Data,serverFD, &package_cli.action.login_request, package_cli.PID);
      break;

    case USER_EXIT:
        removeUserByPID(Data, package_cli.PID);
      break;

    case USER_COM:
      break;

    case USER_ACTION:
      break;

    default:
      break;
  }
}

void * pipeMain(void * rcvData)
{
  ClientsData * Data = (ClientsData *) rcvData;

  int serverFD;

  criaServerPipe();

  if((serverFD = open(SERVER_PIPE,O_RDWR)) < 0)
  {
    perror("Erro ao abrir pipe do servidor: ");
    return NULL;
  }

  while(1) //TODO ver isto NOTE: SHUTDOWN
    readData(Data,serverFD);
}


//NOTE Server core funcs
void freeSpace(char **array)
{
  for(int i = 0;array[i] != NULL;i++)
  {
    free(array[i]);
  }
  free(array);
}

char ** getComandAndArguments(char * string, char ** command, int * argQuant)
{
  char *argument;
  char **arguments = NULL;
  char **aux;
  *argQuant = 0;

  *command = strtok(string," \n");

  aux = realloc(arguments,sizeof(char *) * ((*argQuant) + 1));
  if(aux != NULL)
    arguments = aux;
  else
    return NULL;

  arguments[*argQuant] = malloc(50 * sizeof(char));
  if(arguments[*argQuant] == NULL)
  {
    free(arguments);
    return NULL;
  }

  strcpy(arguments[(*argQuant)],*command);
  (*argQuant)++;

  //NOTE ARGUMENT AND ARGUMENTS
  for (argument = strtok(NULL, " \n");argument != NULL;)
  {
    aux = realloc(arguments,sizeof(char *) * ((*argQuant) + 1));
    if(aux != NULL)
      arguments = aux;
    else
    {
      freeSpace(arguments);
      return NULL;
    }

    arguments[*argQuant] = malloc(50 * sizeof(char));
    if(arguments[*argQuant] == NULL)
    {
      freeSpace(arguments);
      return NULL;
    }
    strcpy(arguments[*argQuant],argument);

    (*argQuant)++;
    argument = strtok(NULL, " \n");
  }

  aux = realloc(arguments,sizeof(char *) * ((*argQuant) + 1)); //NOTE INSERE NULL NO ULTIMO PONTEIRO DA STRING PARA SABER O FIM.
  aux[*argQuant] = NULL;
  arguments = aux;

  return arguments;
}


void handleCommand(char * str, ClientsData * Data)
{
  char *command;
  char **arguments;
  int argc;

  arguments = getComandAndArguments(str,&command,&argc);

  if(strcmp(command,"add") == 0)
  {
    addUser(argc,arguments);
  }
  else if(strcmp(command,"kick") == 0)
  {
    kickUser(argc,arguments,Data);
  }
  else if(strcmp(command,"users") == 0)
  {
    showCurrentUsers(argc,arguments,*Data);
  }
  else if(strcmp(command,"shutdown") == 0)
  {
    serverShutdown(argc,arguments,Data);
  }
  else if(strcmp(command,"map") == 0)
  {
    printf("Mapa!!!");
  }
  else if(strcmp(command,"help") == 0)
  {
    help(argc,arguments);
  }
  else if (strcmp(command,"clear") == 0)
  {
    clearScreen();
  }
  else if( strcmp(command, "env") == 0)
  {
    printf ("\nAvailable in a few days\n");
  }
  else
  {
    invalidCommand(command);
  }

  freeSpace(arguments);
}


void invalidCommand(char * command)
{
  fprintf(stderr,"O comando:\' %s \' nao existe. utilize help para ajuda!",command);
}

void trataSinal(int s)
{
  if(s == SIGINT) //FAZ ROTINA DE SHUTDOWN
    unlink(SERVER_PIPE); //TODO TRATAR SIGINT E SIGUSR 1 PARA SHUTDOWN
    exit(0);
}

//IGNORE SIGNALS: TODO
//sigset_t mask;
//sigfillset(&mask);
//sigprocmask(SIG_SETMASK, &mask, NULL);

void console(ClientsData * Data)
{
  //create_environment_variables();
  char buffer[buffer_size];

  signal(SIGINT,trataSinal);

  //TODO trata sinais
  setbuf(stdout,NULL);
  printf("\n\nServer Console: [Admin]\n");

  while(1) //Mudar para do while ?
  {
    printf("\n$: ");

    fscanf(stdin," %299[^\n]",buffer);

    handleCommand(buffer,Data);
  }

  //NOTE Handle command pode devolver um valor e este pode ser o valor para acabar o ciclo.
  //NOTE: Pensar nisso.
  //shutdown()
}
