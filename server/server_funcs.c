#include "server_funcs.h"
#include "../comun_info.h"

//NOTE Named pipes
int criaServerPipe()
{
  if(access(SERVER_PIPE,F_OK) < 0)
    if(mkfifo(SERVER_PIPE,0664) < 0)
    {
      perror("Erro ao criar pipe no servidor: ");
      return -1;
    }

  return 0;
}


int verifyLoggedPlayers(ClientsData * Data, Login login)
{
  for(int i = 0; i < Data->nClients; i++)
    if(strcmp(Data->clients[i].username,login.username) == 0)
      return -1;

  return 0;
}

int credentialValidation(FILE *f,Login login)
{
  Login temp;

  while(fscanf(f," %s %s ",temp.username,temp.password) == 2)
    if(strcmp(temp.username, login.username) == 0)
    {
      if(strcmp(temp.password,login.password) == 0)
        return USER_LOGIN_ACCEPTED;
      else
        return USER_LOGIN_WRONG_PASS;
    }

  return USER_DOESNT_EXIST;
}


int verifyPlayerCredentials(Login login)
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

  ret = credentialValidation(f,login);
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


void copyLoginToClient(Client * cli, Login login)
{
  strcpy(cli->username,login.username);
  cli->PID = login.PID;
  cli->player = NULL;

  openClientFD(cli);
}

int verifyPlayerLoginRequest(ClientsData *Data,Client * cli,int serverFD)
{
  Login login;

  if(Data->nClients == 20)
    return SERVER_FULL;

  if((read(serverFD,&login,sizeof(Login))) < 0)
  {
    perror("Error reading from Server Pipe: ");
    return -1;
  }

  copyLoginToClient(cli,login);

  if(verifyPlayerCredentials(login) == 0)
    if(verifyLoggedPlayers(Data,login) == 0)
      return USER_LOGIN_ACCEPTED;
    else
      return USER_ALREADY_IN;
  else
    return USER_DOESNT_EXIST;
}

void removeClient(ClientsData * Data, Client cli)
{
  for(int i = 0;i < Data->nClients; i++)
    if(strcmp(Data->clients[i].username,cli.username) == 0)
    {
      Data->clients[i] = Data->clients[(Data->nClients)-1];
      (Data->nClients)--;
    }
}

int authentication(ClientsData * Data,int serverFD)
{
  int response;
  Client cli;

  response = verifyPlayerLoginRequest(Data,&cli,serverFD);
  if(response == USER_LOGIN_ACCEPTED)
  {
    addClientToArray(Data,cli);
    if(write(cli.FD,&response,sizeof(int)) < 0)
    {
      removeClient(Data,cli);
      close(cli.FD);
      return -1;
    }
  }
  else
  {
    if(write(cli.FD,&response,sizeof(int)) < 0)
    {
      close(cli.FD);
      return -1;
    }

    close(cli.FD);
  }

  return 0;
}

//NOTE: Remove user Function

void removeUser(ClientsData * Data, int serverFD)
{
  Client cli;

  if(read(serverFD,&cli,sizeof(Client)) < 0)
  {
    perror("Erro ao ler estrutura de saida de cliente: ");
    return;
  }

  removeClient(Data,cli);
}

void readData(ClientsData * Data,int serverFD)
{
  int type = 69;

  read(serverFD,&type,sizeof(int));

  switch(type)
  {
    case USER_AUTH:
      authentication(Data,serverFD);
      break;

    case USER_EXIT:
      removeUser(Data,serverFD);
      break;

    case USER_COM:
      break;

    case USER_ACTION:
      break;

    case USER_REQUEST:
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

  if((serverFD = open(SERVER_PIPE,O_RDONLY)) < 0)
  {
    perror("Erro ao abrir pipe do servidor: ");
    return NULL;
  }

  while(1) //TODO ver isto NOTE: SHUTDOWN
    readData(Data,serverFD);

  //shutdownPipe(serverFD);
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
        if(chdir("../server_support_funcs/") < 0)
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

void trataSinal(int s)
{
  if(s == SIGINT) //FAZ ROTINA DE SHUTDOWN
    return;
}

//IGNORE SIGNALS: TODO
//sigset_t mask;
//sigfillset(&mask);
//sigprocmask(SIG_SETMASK, &mask, NULL);

void console()
{
  char buffer[buffer_size];

  signal(SIGINT,trataSinal);

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
