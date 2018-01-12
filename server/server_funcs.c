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
  Data->clients[Data->nClients].inGame = 0;
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

  answer_login.type = LOGIN_RESPONSE;
  answer_login.login_answer = response;

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
      SERVER_ANSWER_LOG.type = LOGOUT_RESPONSE;

      write(Data->clients[i].FD, &SERVER_ANSWER_LOG, sizeof(gameInfo));

      Data->clients[i] = Data->clients[(Data->nClients)-1];

      (Data->nClients)--;
    }
}

//################################################################################################
//################################ Jogo
//################################################################################################

Client * getUserByPID(ClientsData * data, int cli_pid)
{
  for(int i = 0; i < data->nClients ; i++)
    if(cli_pid == data->clients[i].PID)
      return &data->clients[i];

  return NULL;
}

void userEntersGame(ClientsData * data, int cli_pid)
{
  Client *cli = getUserByPID(data, cli_pid);
  cli->inGame = 1;
}

void userLeavesGame(ClientsData * data, int cli_pid)
{
  Client *cli = getUserByPID(data, cli_pid);
  cli->inGame = 0;
}

void sendMessageGlobal(ClientsData * data,Package_Cli cli)
{
  Client * client = getUserByPID(data,cli.PID);

  global_map->type = SERVER_CHAT;
  sprintf(global_map->msg,"%s: %s\n",client->username,cli.action.msg);

  for(int i = 0; i < data->nClients ; i++)
    if(write(data->clients[i].FD,global_map,sizeof(gameInfo)) == -1)
    {
      printf("Impossível enviar mensagem ao utilizador: %s",data->clients[i].username);
    }
}

void generateMap(int complexity)
{
  srand(time(0));
  int type[2] = {FREE, WALL};

  //Fill collums:
  for(int x = 0; x < 20; x++)
    for(int y = 0; y < 30; y++)
      if(y % 2 == 1 && x % 2 == 1)
        global_map->map[x][y].type = IRON_WALL;
      else
        global_map->map[x][y].type = type[rand()%2];
}

void sendMapToClients(ClientsData * data)
{
  global_map->type = SERVER_MAP;
  for(int i = 0; i < data->nClients; i++)
    if(data->clients[i].inGame)
      if(write(data->clients[i].FD,global_map,sizeof(gameInfo)) == -1)
        printf("Erro ao mandar mapa ao cliente %s...\n",data->clients[i].username);
}

Map * getFreeMapPos()
{
  srand(time(0));
  Map * temp;

  do
  {
    temp = &global_map->map[rand()%20][rand()%30];
  }while(temp->type != FREE);

  return temp;
}

void insertUserIntoMap(ClientsData * data, int PID)
{
  Client * cli = getUserByPID(data,PID);
  Map * myPos = getFreeMapPos();

  pthread_mutex_lock(&map_token);
  myPos->type = PLAYER;
  pthread_mutex_unlock(&map_token);
  myPos->player.PID = PID;
  myPos->player.score = 0;
  myPos->player.bomb = 2;
  myPos->player.megabomb = 1;
  myPos->player.orientation = KEY_UP;
}

void validaMovimentos(Player * player, int mov)
{
  switch(mov)
  {
    case KEY_UP:
    break;

    case KEY_DOWN:
    break;

    case KEY_LEFT:
    break;

    case KEY_RIGHT:
    break;
  }
}

void userMovement(ClientsData * data, Package_Cli pkg)
{
  Client * cli = getUserByPID(data,pkg.PID);
  char message[100];
  sprintf(message,"O utilzador %s carregou na tecla %c",cli->username,pkg.action.key);
  printf("%s\n",message);


  //validaMovimentos(Player *player, int mov);

  sendMapToClients(data);
  for(int x = 0; x < 21;x++)
  {
    printf("\n");
    for(int y = 0; y < 31; y++)
      printf("%c",global_map->map[x][y].type);
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

    case USER_PLAY:
      if(!game)
      {
        generateMap(1);
        game = true;
      }
      userEntersGame(Data,package_cli.PID);
      insertUserIntoMap(Data,package_cli.PID);
      break;

    case USER_QUIT:
      userLeavesGame(Data,package_cli.PID);
      break;

    case USER_CHAT:
      sendMessageGlobal(Data,package_cli);
      break;

    case USER_ACTION:
      userMovement(Data,package_cli);
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
    if(argc > 1)
      fprintf(stderr,"\nmodo de uso: %s\n",arguments[0]);
    else
      serverShutdown();
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

void console(ClientsData * Data)
{
  global_map = malloc(sizeof(gameInfo));

  //create_environment_variables();
  char buffer[buffer_size];

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
