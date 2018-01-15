#include "server_funcs.h"
#include "../comun_info.h"
#include "../server_support_funcs/server_support_funcs.h"

int TotalEnemy = 3;

void movEnemyforPosition(Map *orig, Map *dest){

  pthread_mutex_lock(&map_token);
  orig->type=FREE;
  dest->type=ENEMY;
  pthread_mutex_unlock(&map_token);
}
/* NA FUNCAO MOVE ENEMY AS DIREÇOES FICARAM AO CONTRARIO
enum ORIENTATION {

up = 0, // left
down,   // right
left,   // up
right   // down
};
*/
void * Move_Enemy(void * enemy){

int orientation = 0;
int x = 0, y = 0;
Enemy *enemY = ( Enemy *) enemy;
enemY->alive = 1;
y = enemY->posx;
x = enemY->posy;
srand(time(0));

while (enemY->alive == 1){

  orientation = rand()%4;

  switch ( orientation ){
    case up:
      if(y - 1  >= 0 && global_map->map[x][y-1].type == FREE || global_map->map[x][y-1].type  == PLAYER){
          movEnemyforPosition(&global_map->map[x][y], &global_map->map[x][y-1]);
          y = y - 1;
        }
      break;
    case down:
      if (y + 1 < 20 && global_map->map[x][y+1].type == FREE || global_map->map[x][y+1].type  == PLAYER){
        movEnemyforPosition(&global_map->map[x][y], &global_map->map[x][y+1]);
        y = y + 1;
      }
      break;
    case left:
      if ( x - 1 >= 0 && global_map->map[x-1][y].type == FREE || global_map->map[x-1][y].type  == PLAYER){
          movEnemyforPosition(&global_map->map[x][y], &global_map->map[x-1][y]);
          x = x-1;
    }
    break;
    case right:
      if ( x + 1 < 30 && global_map->map[x+1][y].type == FREE || global_map->map[x+1][y].type  == PLAYER){
        movEnemyforPosition(&global_map->map[x][y], &global_map->map[x+1][y]);
        x = x+1;
    }
    break;
  }
  sendMapToClients(global_clients);
  usleep(1000*400);
}
}

void startEnemyMove(Enemy *enemy){
  for (int i = 0; i < TotalEnemy; i++){  //TODO POR THREADS
    pthread_create(&enemy[i].enemy_ID, NULL, Move_Enemy, (void *) &enemy[i]);
  }
}

void putEnemyInMap(Enemy *enemy){
  srand(time(0));
  //global_map[20][30]
  int x, y;
  for ( int i = 0 ; i < TotalEnemy; i++){
    pthread_mutex_lock(&map_token);
    do{
      x = rand()%20;
      y = rand()%30;

    }while (global_map->map[x][y].type != FREE);

    global_map->map[x][y].type = ENEMY;
    enemy[i].posx = x;
    enemy[i].posy = y;
    pthread_mutex_unlock(&map_token);
  }
}

Enemy * CreateEnemy(){
  Enemy *enemy;
  enemy = malloc(sizeof(Enemy)*TotalEnemy);
  if ( enemy == NULL){
    perror("Error to make malloc for enemyes  struct");
  }
  return enemy;
}

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
  for(int x = 0; x < 21; x++)
    for(int y = 0; y < 31; y++)
      if(y % 2 == 1 && x % 2 == 1)
        global_map->map[x][y].type = IRON_WALL;
      else
        global_map->map[x][y].type = FREE;//type[rand()%2];
}

void sendMapToClients(ClientsData * data)
{
  pthread_mutex_lock(&map_send);
  global_map->type = SERVER_MAP;
  for(int i = 0; i < data->nClients; i++)
    if(data->clients[i].inGame)
      if(write(data->clients[i].FD,global_map,sizeof(gameInfo)) == -1)
        printf("Erro ao mandar mapa ao cliente %s...\n",data->clients[i].username);

  pthread_mutex_unlock(&map_send);
}

void getFreeMapPos(Client * cli)
{
  srand(time(0));
  Map * map;

  pthread_mutex_lock(&map_token);
  do
  {
    map = &global_map->map[rand()%20][rand()%30];
  }while(map->type != FREE);

  map->type = PLAYER;
  cli->player = &map->player;
  map->player.PID = cli->PID;
  map->player.orientation = left;
  map->player.bomb = 2;
  map->player.megabomb = 1;

  pthread_mutex_unlock(&map_token);

}

void moveToPos(Client * cli,Map * orig, Map * dest, int orientation)
{

  if(orig->type == BOMB || orig->type == MEGABOMB)
  {
    pthread_mutex_lock(&map_token);
    dest->type = PLAYER;
    dest->player.orientation = orientation;
    cli->player = &dest->player;
    pthread_mutex_unlock(&map_token);
    dest->player.bomb = orig->player.bomb;
    dest->player.megabomb = orig->player.megabomb;
  }
  else if(dest->type != EXIT)
  {
    pthread_mutex_lock(&map_token);
    orig->type = FREE;
    dest->type = PLAYER;
    dest->player.orientation = orientation;
    cli->player = &dest->player;
    pthread_mutex_unlock(&map_token);
    dest->player.bomb = orig->player.bomb;
    dest->player.megabomb = orig->player.megabomb;
  }
  else
  {
    pthread_mutex_lock(&map_token);
    orig->type = FREE;
    dest->player.orientation = orientation;
    cli->player = &dest->player;
    pthread_mutex_unlock(&map_token);
    dest->player.bomb = orig->player.bomb;
    dest->player.megabomb = orig->player.megabomb;
  }
}

int posValid(Client * cli, int orientation, int x, int y)
{
  if(global_map->map[x][y].type == FREE || global_map->map[x][y].type == ENEMY)
    return 1;

  cli->player->orientation = orientation;
  return 0;
}

int jump(int x1, int x2, int y1, int y2)
{
  int avoid[4] = {WALL,BOMB,MEGABOMB,EXIT};

  for(int i = 0; i < 4 ; i++)
    if(global_map->map[x1][y1].type == avoid[i] || global_map->map[x2][y2].type == avoid[i])
      return 0;

  return 1;
}

void plantBomb(Map * pos, int size)
{
  if(size == small)
    pos->type = BOMB;
  else
    pos->type = MEGABOMB;
}

void sendDeathToClient(int pid)
{
  Client * c = getUserByPID(global_clients,pid);
  c->player = NULL;
  userLeavesGame(global_clients,c->PID);
  pthread_mutex_lock(&map_send);
  global_map->type = USER_DIE;
  if(write(c->FD,global_map,sizeof(gameInfo)) == -1)
    perror("Impossivel enviar kill ao cliente.");
  pthread_mutex_unlock(&map_send);
}

void explode(Client * cli, int x,int y, int size)
{
  //Explode up n down
  int initx,endx,inity,endy;
  int t = 0;
  Client * c = NULL;

while(t < 4)
{
    usleep(1000*500);
    t++;

    if(x - size < 0)
      initx = 0;
    else
      initx = x - size;

    if(x+size > 20)
      endx = 20;
    else
      endx = x+size;

    for(int i = initx; i < endx; i++)
    {
      if(global_map->map[i][y].type == PLAYER)
      {
        sendDeathToClient(global_map->map[i][y].player.PID);
        global_map->map[i][y].type = FREE;
      }
      else if(global_map->map[x][i].type == ENEMY)
      {
        //TODO:
      }
      else
      {
        global_map->map[i][y].type = EXPLOSION;
      }
    }

    if(y - size < 0)
      inity = 0;
    else
      inity = y-size;

    if(y + size > 30)
      endy = 30;
    else
      endy = y+size;

    for(int i = inity; i < endy; i++)
    {
      if(global_map->map[x][i].type == PLAYER)
      {
        sendDeathToClient(global_map->map[x][i].player.PID);
        global_map->map[i][y].type = FREE;
      }
      else if(global_map->map[x][i].type == ENEMY)
      {
        //TODO:
      }
      else
      {
        global_map->map[x][i].type = EXPLOSION;
      }
    }
    sendMapToClients(global_clients);
  }
  for(int i = initx; i < endx; i++)
    if(global_map->map[i][y].type == EXPLOSION)
      global_map->map[i][y].type = FREE;

  for(int i = inity; i < endy; i++)
    if(global_map->map[x][i].type == EXPLOSION)
      global_map->map[x][i].type = FREE;

  sendMapToClients(global_clients);
}

//Funções da bomba
void * bombAction (void * param)
{
  bombParam * info = (bombParam *) param;

  int size = info->size;
  Client * cli = info->player;
  int x = cli->player->posx;
  int y = cli->player->posy;
  Map * bombPos = &global_map->map[x][y];

  if(size == small)
  {
    if(cli->player->bomb < 1)
      return NULL;

    (cli->player->bomb)--;
  }
  else
  {
    if(cli->player->megabomb < 1)
      return NULL;

    (cli->player->megabomb)--;
  }

  pthread_mutex_lock(&map_token);
  plantBomb(bombPos,size);
  pthread_mutex_unlock(&map_token);

  sleep(2);

  //pthread_mutex_lock(&map_token);
  explode(cli,x,y,size);
  //pthread_mutex_unlock(&map_token);
  if(cli->player != NULL)
    if(cli->inGame == 1)
      if(size == small)
        (cli->player->bomb)++;
      else
        (cli->player->megabomb)++;

  free(info);
}

void validaMovimentos(Client * cli, int mov)
{

  bombParam * BP;

  int x = cli->player->posx, y = cli->player->posy;
  printf("X: %d Y: %d\n",x,y);

  switch(mov)
  {
    case COMMAND_LEFT:
      //if(global_map->map[x][y-1].type == FREE)
        if((y - 1) >= 0){
          //global_map->map[x][y].player.orientation = left;
          moveToPos(cli,&global_map->map[x][y],&global_map->map[x][y-1],left);

        }
    break;

    case COMMAND_RIGHT:
      //if(global_map->map[x][y+1].type == FREE || global_map->map[x][y+1].type == ENEMY)
      if(y+1 < 31){
        //global_map->map[x][y].player.orientation = right;
        moveToPos(cli,&global_map->map[x][y],&global_map->map[x][y+1], right);
      }

    break;

    case COMMAND_UP:
      if(x-1 >= 0){
      //global_map->map[x][y].player.orientation = up;
      //if(global_map->map[x-1][y].type == FREE || global_map->map[x-1][y].type == ENEMY)
      moveToPos(cli,&global_map->map[x][y],&global_map->map[x-1][y], up);
      }
    break;

    case COMMAND_DOWN:
      if(x+1 < 21){
        //mglobal_map->map[x][y].player.orientation = down;
        moveToPos(cli,&global_map->map[x][y],&global_map->map[x+1][y], down);
      }
      //if(global_map->map[x+1][y].type == FREE || global_map->map[x+1][y].type == ENEMY)
    break;

    case COMMAND_SMALLBOMB:
      BP = malloc(sizeof(bombParam));
      BP->player = cli;
      BP->size = small;
      pthread_create(&BP->id,NULL,bombAction,(void *) BP);
    break;

    case COMMAND_BIGBOMB:
      BP = malloc(sizeof(bombParam));
      BP->player = cli;
      BP->size = 4;
      pthread_create(&BP->id,NULL,bombAction,(void *) BP);
    break;

    case COMMAND_JUMP:
        if (global_map->map[x][y].player.orientation == left && y - 2 >= 0)
        {
          if(jump(x,x,y-1,y-2))
            moveToPos(cli,&global_map->map[x][y],&global_map->map[x][y-2], left);
        }
        else if ( global_map->map[x][y].player.orientation == right & y+2 <= 30)
        {
          if(jump(x,x,y+1,y+2))
            moveToPos(cli,&global_map->map[x][y],&global_map->map[x][y+2], right);
        }
        else if ( global_map->map[x][y].player.orientation == up && x-2 >= 0)
        {
          if(jump(x-1,x-2,y,y))
            moveToPos(cli,&global_map->map[x][y],&global_map->map[x-2][y], up);
        }
        else if  ( global_map->map[x][y].player.orientation == down && x+2 <= 20)
        {
          if(jump(x+1,x+2,y,y))
            moveToPos(cli,&global_map->map[x][y],&global_map->map[x+2][y], down);
        }

    break;
  }
}

void userMovement(ClientsData * data, Package_Cli pkg)
{
  Client * cli = getUserByPID(data,pkg.PID);

  validaMovimentos(cli,pkg.action.key);

  sendMapToClients(data);
}

//################################################################################################
//################################ Fim de funções de jogo
//################################################################################################

void readData(ClientsData * Data,int serverFD)
{
  Enemy *enemy;
  Client * client;
  Package_Cli package_cli;
  //printf(" X: %d Y: %d\n",global_map->map[10][10].player.posx,global_map->map[10][10].player.posx);
  read(serverFD,&package_cli,sizeof(Package_Cli));
  client = getUserByPID(Data,package_cli.PID);

  switch(package_cli.TYPE)
  {
    case USER_AUTH:
      authentication(Data,serverFD, &package_cli.action.login_request, package_cli.PID);
      break;

    case USER_EXIT:
      removeUserByPID(Data, package_cli.PID);
      break;

    case USER_PLAY:
    printf(" X: %d Y: %d\n",global_map->map[10][10].player.posx,global_map->map[10][10].player.posx);
      if(!game)
      {
        generateMap(1);
        enemy = CreateEnemy();
        putEnemyInMap(enemy);
        startEnemyMove(enemy);
        game = true;
      }
      userEntersGame(Data,package_cli.PID);
      getFreeMapPos(client);
      sendMapToClients(Data);

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
void ShowAmbientEnviron(){

  extern char **environ;
  int i= 0;
  while (environ[i]) {
    if (environ[i][0] == 'n')
      printf("%s\n", environ[i++]);
    else
      i++;
  }
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
    ShowAmbientEnviron();
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
void createRnvironmentVariables(){

  char objects[9] = "nObjects";
  char nobjects[2] = "00";
  char ENEMYS[6] = "nEnemy";
  char nENEMYS[2] = "03";
  char PLAYERS[11] = "nMaxPlayers";
  char nPLAYERS[2] = "20";
  setenv(objects,nobjects, 1);
  setenv(ENEMYS,nENEMYS, 1);
  setenv(PLAYERS,nPLAYERS, 1);
}

void console(ClientsData * Data)
{
  //create_environment_variables();
  char buffer[buffer_size];
  createRnvironmentVariables();
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
