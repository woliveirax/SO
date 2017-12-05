#include "client.h"
#include "../comun_info.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void cls()
{
printf ("\033c");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GO_TO_USER_EXIT(Client_data *info)
{
  msg_user_option msg_user_exit;

  msg_user_exit.type = USER_EXIT;
  msg_user_exit.user.PID = getpid();
  msg_user_exit.user.ACTION = USER_EXIT;

 if ( write (info->FD_SERVER_PIPE, &msg_user_exit, sizeof(msg_user_option)) < 0)
 {

   printf ("\nError to send message USER_EXIT for SERVER .. !\n");

   return;
 }
 close (info->FD_SERVER_PIPE);
 unlink(info->CLIENT_PIPE);
 exit (0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO perguntar ao prof duraes a ideia do send private message;
void VIEW_CON_USERS(Client_data *info)
{
  printf ("Available in a few days");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VIEW_TOP_10(Client_data *info)
{
  printf ("Available in a few days");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GO_TO_GAME(Client_data *info)
{
  printf ("Available in a few days");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void USER_MENU( Client_data *info)
{
  int choice = 0;

  while(1)
    do {
      cls();
      printf ("\nUSER OPTION\n");
      printf("1.Play.\n");
      printf("2.Top 10.\n");
      printf("3.View connected users.\n");
      printf("4.Exit.\n");
      printf("Choice: ");
      scanf(" %d", &choice);

      switch (choice)
      {
        case 1 : GO_TO_GAME(info);
                 break;
        case 2 : VIEW_TOP_10(info);
                 break;
        case 3 : VIEW_CON_USERS(info);
                 break;
        case 4 : GO_TO_USER_EXIT(info);
                 break;
      }
    }while (choice < 1 || choice > 4);

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void check_options_login(Client_data *info)
{

  switch (info->LOGIN_CONFIRMATION)
  {
    case USER_LOGIN_ACCEPTED:     printf("\nLogin successfully. Now you are logged in..!\n");
                                  USER_MENU(info);
                                  break;

    case USER_DOESNT_EXIST:       printf("\n\nthis user does not exist, please try another account\n\n"); // TODO fazer a mensagem request create user para admin;
                                  break;

    case USER_LOGIN_WRONG_PASS:   printf("\nWrong user password..!\n");
                                  break;

    case USER_ALREADY_IN:         printf("\nThe player is already logged in..!\n");
                                  break;

    case SERVER_FULL:             printf ("\nServer Full. Try again later ...\n");
                                  break;

    case SERVER_KICK:             printf("\nYou've been kicked from the server.\n"); //TODO PODEMOS MANDAR UMA STRING COM A RAZÃO DE KICK.

                                  unlink(info->CLIENT_PIPE);                         // TODO repensar no kick em vez de exit(0) apenas voltar ao menu login;
                                  return;

    case SERVER_SHUTDOWN:         printf("\nServer going off...\n");                // TODO  acho que falta aqui qualquer coisal
                                  unlink(info->CLIENT_PIPE);
                                  exit(0);

  }
  return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int OPEN_CLIENT_PIPE_READ (Client_data *info)
{

  info->FD_CLIENT_PIPE = open ( info->CLIENT_PIPE, O_RDWR);

  if ( info->FD_CLIENT_PIPE < 0)
  {
    return -1;

  } else {

    return 1;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int SEND_CLIENT_SERVER ( Client_data *info, MSG_Login Client_login )
{

  if ( write(info->FD_SERVER_PIPE, &Client_login, sizeof(MSG_Login)) < 0 )
  {
    return -1;
  }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLIENT_LOGIN( Client_data *info)
{

  pthread_cond_init(&info->AWAITED_REPLY_LOGIN, NULL);

  pthread_mutex_init(&info->LOCK_LOGIN, NULL);

  do {

  MSG_Login Client_login;

  Client_login.type = USER_AUTH;

  printf ("\nUsername: ");

  scanf (" %49s", Client_login.login.username);

  printf ("\nPassword:");

  scanf(" %49s", Client_login.login.password);                //TODO alterar para getpass();

  Client_login.login.PID = getpid();

  if ( SEND_CLIENT_SERVER (info , Client_login ) < 0)
  {
    printf ("\nError sending for server login package ...!\n");
    return;
  }
  pthread_cond_wait(&info->AWAITED_REPLY_LOGIN, &info->LOCK_LOGIN);

  printf ("\n\n%d\n\n", info->LOGIN_CONFIRMATION);

  check_options_login(info);

}while( info->LOGIN_CONFIRMATION != 0);

    return;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLIENT_EXIT(Client_data *info)
{
  close(info->FD_SERVER_PIPE);
  unlink(info->CLIENT_PIPE);
  exit(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Client_options (Client_data *info)
{
  int choice;

while(1)
  do {
    cls();
    printf ("\n\n\nMenu Client\n");
    printf ("1. Login\n");
    printf ("2. Sair\n");
    printf ("Choice: ");
    scanf ("%d", &choice);

    switch (choice)
    {
      case 1: CLIENT_LOGIN(info);
              break;

      case 2: CLIENT_EXIT(info);
              break;
    }

  }while (choice < 1 || choice > 2);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int create_CLIENT_PIPE(Client_data *info)
{
  if ( mkfifo (info->CLIENT_PIPE, 0664) < 0 )
  {
    return -1;
  } else {
    return 0;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int  open_SERVER_PIPE_WRITE (Client_data *info)
{

  info->FD_SERVER_PIPE = open (SERVER_PIPE, O_WRONLY);

  if ( info->FD_SERVER_PIPE < 0 )
  {
    return -1;

  } else {

    return 1;

  }
}
void change_pipe_path(Client_data *info)
{
  sprintf (info->CLIENT_PIPE, CLIENT_PIPE_TEMPLATE , getpid());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void * receive_from_server( void * info)
{
  Client_data * info_client = ( Client_data * ) info;

  if ( OPEN_CLIENT_PIPE_READ(info_client) < 0 )
  {
    printf ("Error opening CLIENT PIPE for reading");
    return NULL;
  }
while (1){

  read( info_client->FD_CLIENT_PIPE, &info_client->LOGIN_CONFIRMATION, sizeof(int) );

  if ( info_client->LOGIN_CONFIRMATION < 1){

      pthread_cond_signal(&info_client->AWAITED_REPLY_LOGIN);

    }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void verify_server()
{
  if(access(SERVER_PIPE,F_OK) < 0){

    printf (" \n\nServer Is Down, try later..!\n\n");

    exit(0);
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Client_console()
{
  MSG_Login LOGIN;

  Client_data info_client;

  info_client.LOGIN_CONFIRMATION = 1;

  change_pipe_path(&info_client);

  if ( open_SERVER_PIPE_WRITE(&info_client) < 0 )
  {
    printf (" \n Error opening SERVER_PIPE for writing..! \n");

    return;
  }

  if ( create_CLIENT_PIPE(&info_client) < 0 )
  {
    printf ("\n Error creating CLIENT_PIPE ! \n");

    return;
  }

  pthread_create(&info_client.RECEIVE_THREAD, NULL, receive_from_server, (void *) &info_client);

  Client_options(&info_client);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main ( int argc, char * argv[])
  {
    setbuf(stdout, NULL);

    verify_server();

    Client_console();

    return 0;
  }
