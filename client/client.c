#include "client.h"
#include "../comun_info.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GO_TO_USER_EXIT(int * fd_SERVER_PIPE, char *CLIENT_PIPE)
{
  msg_user_option msg_user_exit;

  msg_user_exit.type = USER_EXIT;
  msg_user_exit.user.PID = getpid();
  msg_user_exit.user.ACTION = USER_EXIT;

 if ( write ( *fd_SERVER_PIPE, &msg_user_exit, sizeof(msg_user_option)) < 0)
 {

   printf ("\nError to send message USER_EXIT for SERVER .. !\n");

   return;
 }
 close (*fd_SERVER_PIPE);
 unlink(CLIENT_PIPE);
 exit (0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO perguntar ao prof duraes a ideia do send private message;
void VIEW_CON_USERS(int * fd_SERVER_PIPE, char * CLIENT_PIPE)
{
  printf ("Available in a few days");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VIEW_TOP_10(int * fd_SERVER_PIPE, char * CLIENT_PIPE)
{
  printf ("Available in a few days");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GO_TO_GAME(int * fd_SERVER_PIPE, char * CLIENT_PIPE)
{
  printf ("Available in a few days");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void USER_MENU( int * fd_SERVER_PIPE, char * CLIENT_PIPE)
{
  int choice = 0;

  while(1)
    do {
      printf ("\nUSER OPTION\n");
      printf("1.Play.\n");
      printf("2.Top 10.\n");
      printf("3.View connected users.\n");
      printf("4.Exit.\n");
      printf("Choice: ");
      scanf(" %d", &choice);

      switch (choice)
      {
        case 1 : GO_TO_GAME(fd_SERVER_PIPE, CLIENT_PIPE);
                 break;
        case 2 : VIEW_TOP_10(fd_SERVER_PIPE, CLIENT_PIPE);
                 break;
        case 3 : VIEW_CON_USERS(fd_SERVER_PIPE, CLIENT_PIPE);
                 break;
        case 4 : GO_TO_USER_EXIT(fd_SERVER_PIPE, CLIENT_PIPE);
                 break;
      }
    }while (choice < 1 || choice > 4);

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int RECEIVE_CLIENT_PIPE(int *fd_CLIENT_PIPE)
{
  int answer;

  read (*fd_CLIENT_PIPE, &answer, sizeof(int));

  switch (answer)
  {
    case USER_LOGIN_ACCEPTED:     return USER_LOGIN_ACCEPTED;

    case USER_LOGIN_WRONG_PASS :  printf("\nWrong user password..!\n");
                                  break;

    case USER_ALREADY_IN :        printf("\nThe player is already logged in..!\n");
                                  break;

    case SERVER_FULL :            printf ("\nServer Full. Try again later ...\n");
                                  break;
  }
  return -1;
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

  scanf (" %49s", Client_login.login.username);

  printf ("\nPassword: ");

  scanf( "%49s", Client_login.login.password);                 //TODO alterar para getpass();

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
    printf("\nLogin successfully. Now you are logged in..!\n");

    USER_MENU(fd_SERVER_PIPE, CLIENT_PIPE);

  } else {

    return;

  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLIENT_EXIT(int *fd_SERVER_PIPE, char CLIENT_PIPE[MAX] )
{
  close(*fd_SERVER_PIPE);

  unlink(CLIENT_PIPE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Client_options ( int *fd_SERVER_PIPE, char CLIENT_PIPE[MAX])
{
  int choice;

while(1)
  do {
    printf ("\nMenu Client\n");
    printf ("1. Login\n");
    printf ("2. Sair\n");
    printf ("Choice: ");
    scanf ("%d", &choice);

    switch (choice)
    {
      case 1: CLIENT_LOGIN(fd_SERVER_PIPE, CLIENT_PIPE);
              break;

      case 2: CLIENT_EXIT(fd_SERVER_PIPE, CLIENT_PIPE);
              break;
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
