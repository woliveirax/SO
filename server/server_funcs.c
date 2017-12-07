#include "server_funcs.h"
#include "../comun_info.h"
#include "../server_support_funcs/server_support_funcs.h"

//NOTE Named pipes
// aqui vai mostrar todas as variaveis ambiente do sistema;
/*void showEnviron()
{
  int i = 0;

  extern  char **environ;

  while(environ[i]) {

    if( environ[i][0] == 'N' )
    printf(" %s\n ", environ[i++]);
  }
}
*/
/*void create_environment_variables()
{
    char variable_name[2][10] = {"NENEMY", "NOBJET"};
    putenv(*(variable_name), "00");
    putenv(*(variable_name + 1), "00");

}
*/
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
  {
    if(strcmp(temp.username, login.username) == 0)
      if(strcmp(temp.password,login.password) == 0)
        return USER_LOGIN_ACCEPTED;
      else
        return USER_LOGIN_WRONG_PASS;
  }
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
      removeClientFromArray(Data,cli.username);
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

void removeUserByPID(ClientsData * Data, int PID)
{
  for(int i = 0; i < Data->nClients; i++)
    if(Data->clients[i].PID == PID)
    {
      Data->clients[i] = Data->clients[(Data->nClients)-1];
      (Data->nClients)--;
    }
}

void removeUser(ClientsData * Data, int serverFD)
{
  USER_action cli;

  if(read(serverFD,&cli,sizeof(USER_action)) < 0)
  {
    perror("Erro ao ler estrutura de saida de cliente: ");
    return;
  }

  removeUserByPID(Data,cli.PID);
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
  else if( strcmp(command, "env") == 0)
  {
    //showEnviron();
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
    unlink(SERVER_PIPE);
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
