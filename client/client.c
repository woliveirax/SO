#include "client.h"
#include "../comun_info.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void USER_MENU()
{
  printf ("\nEm construçao...\n");
}

int RECEIVE_CLIENT_PIPE(int *fd_CLIENT_PIPE)
{
  int answer;

  read (*fd_CLIENT_PIPE, &answer, sizeof(int));

  switch (answer)
  {
    case USER_LOGIN_WRONG_PASS : printf("\nWrong user password..!\n");

    case USER_ALREADY_IN : printf ("\nThe player is already logged in..!\n");

    case SERVER_FULL : printf ("\nServer Full. Try again later ...\n");

  }
  return answer;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int OPEN_CLIENT_PIPE_READ (int *fd_CLIENT_PIPE, char CLIENT_PIPE[MAX] )
{

  *fd_CLIENT_PIPE = open ( CLIENT_PIPE, O_RDONLY);

  if ( *fd_CLIENT_PIPE < 0)
  {
    return -1;

  } else {

    return 1;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int SEND_CLIENT_SERVER ( int *fd_SERVER_PIPE, MSG_Login Client_login )
{

  if ( write(*fd_SERVER_PIPE, &Client_login, sizeof(MSG_Login)) < 0 )
  {
    return -1;
  }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLIENT_LOGIN( int *fd_SERVER_PIPE, char CLIENT_PIPE[MAX])
{
  int fd_CLIENT_PIPE;

  MSG_Login Client_login;

  Client_login.type = USER_AUTH;

  printf ("\nUsername: ");

  scanf (" %49[^\n] ", Client_login.login.username);

  printf ("\nPassword: ");

  scanf( " %49[^\n] ", Client_login.login.password);                 //TODO alterar para getpass();

  Client_login.login.PID = getpid();

  if ( SEND_CLIENT_SERVER (fd_SERVER_PIPE , Client_login ) < 0)
  {
    printf ("\nError sending for server login package ...!\n");
    return;
  }
  if ( OPEN_CLIENT_PIPE_READ(&fd_CLIENT_PIPE, CLIENT_PIPE) < 0 )
  {
    printf ("Error opening CLIENT PIPE for reading");
    return;
  }
  if ( RECEIVE_CLIENT_PIPE(&fd_CLIENT_PIPE) == USER_LOGIN_ACCEPTED )
  {
    USER_MENU();
  }

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLIENT_EXIT(int *fd_SERVER_PIPE, char CLIENT_PIPE[MAX] )
{
  close(*fd_SERVER_PIPE);

  unlink(CLIENT_PIPE);

  exit(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Client_options ( int *fd_SERVER_PIPE, char CLIENT_PIPE[MAX])
{
  int choice;

  do {
    printf ("\nMenu Client\n");
    printf ("1. Login\n");
    printf ("2. Sair\n");
    printf ("Choice: ");
    scanf ("%d", &choice);

    switch (choice)
    {
      case 1: CLIENT_LOGIN(fd_SERVER_PIPE, CLIENT_PIPE);

      case 2: CLIENT_EXIT(fd_SERVER_PIPE, CLIENT_PIPE);
    }

  }while (choice < 1 || choice > 2);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int create_CLIENT_PIPE(char * PIPE)
{
  if ( mkfifo (PIPE, 0664) < 0 )
  {
    return -1;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int  open_SERVER_PIPE_WRITE (int *fd)
{

  *fd = open (SERVER_PIPE, O_WRONLY);

  if ( *fd < 0 )
  {
    return -1;

  } else {

    return 1;

  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Client_console()
{
  MSG_Login LOGIN;

  char CLIENT_PIPE[MAX];

  int fd_SERVER_PIPE;

  sprintf (CLIENT_PIPE, CLIENT_PIPE_TEMPLATE , getpid());

  printf ("\n%s\n", CLIENT_PIPE);

  if ( open_SERVER_PIPE_WRITE(&fd_SERVER_PIPE) < 0 )
  {
    printf (" \n Error opening SERVER_PIPE for writing..! \n");

    return;
  }

  if ( create_CLIENT_PIPE(CLIENT_PIPE) < 0 )
  {
    printf ("\n Error creating CLIENT_PIPE ! \n");

    return;
  }

  Client_options(&fd_SERVER_PIPE, CLIENT_PIPE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main ( int argc, char * argv[])
  {
    Client_console();
    return 0;
  }
