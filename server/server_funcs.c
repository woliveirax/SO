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


int verifyLoggedPlayers(Data * Data, Login login)
{
  for(int i = 0; i < Data->nClients; i++)
    if(strcmp(Data->Clients[i].Username,login.Username) == 0)
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
        return 0; //Correct password
      else
        return -2; //Bad password
    }

  return -1; //Username doesn't exist
}
int verifyPlayerCredentials(Login login)
{

  if(access(USERS_LOGIN_DATA,F_OK) < 0)
    return -1; //No users and no file TODO Alterar erro

  FILE *f;

  if((f = fopen(USERS_LOGIN_DATA,"rt")) == NULL)
  {
    perror("Erro ao abrir o ficheiro de utilizadores: ");
    return -1;
  }

  if(!credentialValidation(f,login)) //TODO Falta tratar -2
  {
    fclose(f);
    return 0; //Password valida
  }
  else
  {
    fclose(f);
    return -1; //Password Invalida
  }
}


void acceptLogin(ClientsData * Data, Login login)
{
  addClientsToArray(Data.clients[nClients],login)

}


int verifyPlayerLoginRequest(ClientsData *Data)
{
  Login login;

  if(Data->nClients = 20)
    return -4; //SERVER FULL

  if((read(serverFD,&login,sizeof(Login))) < 0)
  {
    perror("Error reading from Server Pipe: ");
    return -1;  //PIPE ERROR
  }

  if(!verifyPlayerCredentials(login))
    if(!verifyLoggedPlayers(login))
      acceptLogin(Data, login);
    else
      return -3; //PLAYER ALREADY LOGGED IN
  else
    return -2; //PASSWORD MISMATCH

  return 0; //USER WAS ACCEPTED AND LOGGED
}


int authentication(ClientsData * Data)
{
  int response;

  response = verifyPlayerLoginRequest(Data);
  if(response == -1)
    return -1;




}


void readData(ClientsData * Data,int serverFD)
{
  int type = 69;

  read(serverFD,&type,sizeof(int));//TODO Verificar perror

  switch(type)
  {
    case USER_AUTH:
      authentication(Data,serverFD);
      break;

    case USER_EXIT:
      userExit(Data,serverFD);
      break;

    case USER_COM:
      break;

    case USER_ACTION:
      break;

    case USER_REQUEST:
      break;

    case default:
      break;
  }
}


void pipeMain(ClientsData * Data)
{
  int serverFD;

  criaServerPipe();

  if((serverFD = open(SERVER_PIPE,O_RDONLY)) < 0)
  {
    perror("Erro ao abrir pipe do servidor: ");
    return -1;
  }

  while(1) //TODO ver isto
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

//TODO tentar melhorar se houver tempo;
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
        if(chdir("../server_support_funcs") < 0)
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

void console()
{
  char buffer[buffer_size];

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
