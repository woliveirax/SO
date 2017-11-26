#include "client.h"
#include "../comun_info.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int RECEIVE_CLIENT_PIPE(int *fd_CLIENT_PIPE)
{
  int response;

  read (*fd_CLIENT_PIPE, response, sizeof(int));

  switch (response)
  {
    case -2:
    case -3:
    case -4:

  }
  return response;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int OPEN_CLIENT_PIPE_READ (int *fd_CLIENT_PIPE)
{

  *fd_CLIENT_PIPE = open ( PIPE, O_WRONLY);

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
void CLIENT_LOGIN( int *fd_SERVER_PIPE)
{
  int fd_CLIENT_PIPE;
  MSG_Login CLient_login;

  CLient_login.type = USER_AUTH;

  printf ("\nUsername: ");

  scanf (" %49[^\n]", Client_login.login.username);

  printf ("\n Password: ");

  scanf( "%49[^\n]", Client_login.login.password);                 //TODO alterar para getpass();

  Client_login.login.PID = getpid();

  if ( SEND_CLIENT_SERVER (fd_SERVER_PIPE , &Client_login ) < 0)
  {
    printf ("\nError sending for server login package ...!\n");
    return;
  }
  if ( OPEN_CLIENT_PIPE_READ(&fd_CLIENT_PIPE) < 0 )
  {
    printf ("Error opening CLIENT PIPE for reading");
    return;
  }
  if ( RECEIVE_CLIENT_PIPE(&fd_CLIENT_PIPE) == 0)
  {
    USER_MENU();
  }

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLIENT_EXIT(int fd_SERVER_PIPE, char CLIENT_PIPE[MAX] )
{
  close(fd_SERVER_PIPE);

  unlink(CLIENT_PIPE);

  exit(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Client_options ( int fd_SERVER_PIPE, char CLIENT_PIPE[MAX])
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
      case 1: CLIENT_LOGIN(fd_SERVER_PIPE);

      case 2: CLIENT_EXIT(fd_SERVER_PIPE, CLIENT_PIPE);
    }

  }while (choice < 1 || ou choice > 2);
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

  *fd = open ( SERVER_PIPE, O_WRONLY );

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

    return 1;
  }

  if ( create_CLIENT_PIPE(CLIENT_PIPE) < 0 )
  {
    printf ("\n Error creating CLIENT_PIPE ! \n");

    return 1;
  }

  Client_options(&fd_SERVER_PIPE, CLIENT_PIPE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main ( int argc, char * argv[])
  {
    Client_console();
    return 0;
  }
