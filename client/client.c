#include "client.h"
#include "../comun_info.h"



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//funçao que limpa o ecran;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cls()
{
  printf ("\033c");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//funçao que faz com que o utilizador faça logout
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GO_TO_USER_EXIT(Client_data *info)
{
  Package_Cli User_exit;

  User_exit.TYPE = USER_EXIT;
  User_exit.PID = getpid();

 if ( write (info->FD_SERVER_PIPE, &User_exit, sizeof(Package_Cli)) < 0)
 {

   printf ("\nError to send message USER_EXIT for SERVER .. !\n");

   exit(1);
 }
 return;
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
void writeChatMessenger(Client_data *info){

  Package_Cli ChatMessage;
  ChatMessage.TYPE = USER_CHAT;
  ChatMessage.PID = getpid();
  keypad(info->CHATWRITER, TRUE);
  for ( int i = 0; i < 30;i++){
    ChatMessage.action.msg[i] = getch();
    box(info->CHATWRITER, 0, 5);
    wrefresh(info->CHATWRITER);
  }
    write(info->FD_SERVER_PIPE, &ChatMessage, sizeof(Package_Cli));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int sendCommandToServer(int key, Client_data *info){

  Package_Cli SendComand;

  SendComand.TYPE = USER_ACTION;
  SendComand.PID = getpid();

  if (write(info->FD_SERVER_PIPE, &SendComand, sizeof(Package_Cli))< 0)
    return -1;
    else
    return 0;
}

void GAME_START(Client_data *info){

  int key = 0, i=0;
keypad(info->INFOGAME, TRUE);
  while (getch() != 113){
    key = getch();
    box(info->INFOGAME, 0, 5);
    wrefresh(info->INFOGAME);
    switch ( key ) {
      case COMMAND_UP:
        sendCommandToServer(key, info);
        break;
      case COMMAND_DOWN:
        sendCommandToServer(key, info);
        break;
      case COMMAND_RIGHT:
        sendCommandToServer(key, info);
        break;
      case COMMAND_LEFT:
        sendCommandToServer(key, info);
        break;
      case  COMMAND_CHAT:
        writeChatMessenger(info);
        break;
    }
    i++;
  }
  Package_Cli User_exit;

  User_exit.TYPE = USER_QUIT;
  User_exit.PID =getpid();

  write(info->FD_SERVER_PIPE, &User_exit, sizeof(Package_Cli));
  return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CREATE_SPACE_GAME(Client_data *info)
{

  initscr(); // inicia a janela de ncurses
  refresh(); // atualiza
  start_color(); // inicia cores
  init_pair(1, COLOR_WHITE, COLOR_BLACK); // inicia par das cores da fonte e do Fundo
//       altura Comprimento posicao y posicao

  info->LOGOGAME = newwin(4,80,0,0);
  info->INFOGAME = newwin(20,25,4,0);
  info->MAPVIEWER = newwin( 20, 30,4,25);
  info->TIMEGAMEVIEWER = newwin (4,25,4,55);
  info->CHATVIWER = newwin(16,25,8,55);
  info->CHATWRITER = newwin(16,25,8,55);
  // pintar janela
  wbkgd(info->MAPVIEWER, COLOR_PAIR(1));
  wbkgd(info->LOGOGAME, COLOR_PAIR(1));
  wbkgd(info->INFOGAME, COLOR_PAIR(1));
  wbkgd(info->TIMEGAMEVIEWER, COLOR_PAIR(1));
  wbkgd(info->CHATVIWER, COLOR_PAIR(1));
  wbkgd(info->CHATWRITER, COLOR_PAIR(1));

  //fazer casquilho
  box(info->MAPVIEWER, ACS_VLINE, ACS_HLINE);
  box(info->LOGOGAME, ACS_VLINE, ACS_HLINE);
  box(info->INFOGAME, ACS_VLINE,ACS_HLINE);
  box(info->TIMEGAMEVIEWER, ACS_VLINE,ACS_HLINE);
  box(info->CHATVIWER, ACS_VLINE,ACS_HLINE);
  box(info->CHATWRITER, ACS_VLINE,ACS_HLINE);

/*  for ( int i = 0 ; i < 20 ; i++){
    for ( int j = 0; j < 30 ; j++){
      mvwprintw(teste, i, j, "b");
    }
  }
  */
  //box(teste, ACS_VLINE, ACS_HLINE);
  wgetch(info->MAPVIEWER);
  wgetch(info->LOGOGAME);
  wgetch(info->INFOGAME);
  wgetch(info->TIMEGAMEVIEWER);
  wgetch(info->CHATVIWER);
  wgetch(info->CHATWRITER);
  // deve-se fazer o refresh se quiser mostrar alteraçoes
  wrefresh(info->MAPVIEWER);
  wrefresh(info->LOGOGAME);
  wrefresh(info->INFOGAME);
  wrefresh(info->TIMEGAMEVIEWER);
  wrefresh(info->CHATVIWER);
  wrefresh(info->CHATWRITER);


  /*delwin(info->MAPVIEWER);
  delwin(info->LOGOGAME);
  delwin(info->INFOGAME);
  delwin(info->TIMEGAMEVIEWER);
  delwin(info->HISTORYGAMEKEY);
  endwin();
  */
  GAME_START(info);
  // aqui fecha janela
  delwin(info->MAPVIEWER);
  delwin(info->LOGOGAME);
  delwin(info->INFOGAME);
  delwin(info->TIMEGAMEVIEWER);
  delwin(info->CHATVIWER);
  delwin(info->CHATWRITER);
  endwin();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Envia Mensagem para Servidor a informar que vai começar a jogar
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SEND_USER_START_GAME(Client_data *info)
{
  Package_Cli User_start_game;

  User_start_game.TYPE = USER_PLAY;
  User_start_game.PID = getpid();

  if ( write (info->FD_SERVER_PIPE, &User_start_game, sizeof(Package_Cli) ) < 0)
  {
    printf (" \nError to sendo messagem USER_PLAY for Server\n");
    return;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// menu do utilizador com as respetivas opçoes  de cada utilizador
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void USER_MENU( Client_data *info)
{
  int choice = 0;

  while(1)
    do {

      printf ("\nUSER OPTION\n");
      printf("1.Play.\n");
      printf("2.Top 10.\n");
      printf("3.View connected users.\n");
      printf("4.Logout.\n");
      printf("Choice: ");
      scanf(" %d", &choice);

      switch (choice)
      {
        case 1 : CREATE_SPACE_GAME(info);
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//opçoes da resposta obtida apos tentar fazer login
void check_options_login(Client_data *info)
{
  switch (info->LOGIN_CONFIRMATION)
  {
    case USER_LOGIN_ACCEPTED:
      printf("\nLogin successfully. Now you are logged in..!\n");
      USER_MENU(info);
      break;

    case USER_LOGIN_WRONG_PASS:
      printf("\nWrong User password..! Try Again.\n");
      break;

    case USER_DOESNT_EXIST:
      printf("\n\nThis user does not exist, please try with another account\n\n");      // TODO fazer a mensagem request create user para admin;
      break;

    case USER_ALREADY_IN:
      printf("\nThe player is already logged in..!\n");
      break;

    case SERVER_FULL:
      printf ("\nServer Full. Try again later ...\n");
      break;
  }
  return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//abrir CLIENT PIPE PARA LEITURA
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ENVIAR MENSAGEM LOGIN PARA SERVIDOR
int SEND_CLIENT_SERVER ( Client_data *info, Package_Cli *login )
{
  if ( write(info->FD_SERVER_PIPE, login, sizeof(Package_Cli)) < 0 )
  {
    return -1;
  }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNÇAO QUE PEDE DADOS PARA FAZER LOGIN
void CLIENT_LOGIN_( Client_data *info)
{

  pthread_cond_init(&info->AWAITED_REPLY_LOGIN, NULL);

  pthread_mutex_init(&info->LOCK_LOGIN, NULL);

  Package_Cli login;

  login.TYPE = USER_AUTH;
  login.PID =getpid();
  login.action.login_request.try_login = 0;

  do {

  printf ("\nUsername: ");

  scanf (" %49s", login.action.login_request.username);

  printf ("\nPassword:");

  scanf(" %49s", login.action.login_request.password);                //TODO alterar para getpass();

  if ( SEND_CLIENT_SERVER (info , &login ) < 0)
  {
    printf ("\nError sending for server login package ...!\n");
    return;
  }
  pthread_cond_wait(&info->AWAITED_REPLY_LOGIN, &info->LOCK_LOGIN);

  check_options_login(info);

  if( info->LOGIN_CONFIRMATION != 0 )
    login.action.login_request.try_login ++;

}while( login.action.login_request.try_login < 4);

    return;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNÇAO SAIR DO CLIENTE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLIENT_EXIT(Client_data *info)
{
  close(info->FD_SERVER_PIPE);
  unlink(info->CLIENT_PIPE);
  exit(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNÇAO DE OPÇOES DE CLIENTE
void Client_options(Client_data *info)
{
  int choice;
                                                                                  // TODO fazer os menus, ver na nas curiosidades;
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
      case 1: CLIENT_LOGIN_(info);
              break;

      case 2: CLIENT_EXIT(info);
              break;
    }

  }while (choice < 1 || choice > 2);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNÇAO CRIA PIPE CLIENTe
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
//funçao que ABRE PIPE SERVIDOR PARA ESCRITA
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//funçao que ALTERA O DIRETORIO ONDE SER CRIADO O PIPE DO CLIENTE
void change_pipe_path(Client_data *info)
{
  sprintf (info->CLIENT_PIPE, CLIENT_PIPE_TEMPLATE , getpid());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇAO QUE RECEBE RESPOSTA DE LOGIN DO SERVIDOR
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void * receive_from_server( void * info)
{
  Client_data * info_client = ( Client_data * ) info;

  gameInfo Package_Server;

  if ( OPEN_CLIENT_PIPE_READ(info_client) < 0 )
  {
    printf ("Error opening CLIENT PIPE for reading");
    return NULL;
  }
  while (1){

    read( info_client->FD_CLIENT_PIPE, &Package_Server, sizeof(gameInfo) );

    switch (Package_Server.type)
    {
      case LOGIN_RESPONSE:
        info_client->LOGIN_CONFIRMATION = Package_Server.login_answer;
        pthread_cond_signal(&info_client->AWAITED_REPLY_LOGIN);
        break;
      case SERVER_SHUTDOWN:
        printf ("\n\nYour connection will shut down because the server will shut down in a few moments.\n\n");
        CLIENT_EXIT(info);
        break;
      case LOGOUT_RESPONSE:
        printf ("\n\n Successful logout..!\n\n");
        CLIENT_EXIT(info);
        break;
      case SERVER_KICK:
        printf("\nYou've been kicked from the server.\n");
        CLIENT_EXIT(info);
        break;
      }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNÇAO VERIFICA SE SERVIDOR ESTA LIGADO atraves do PIPE DELE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void verify_server()
{
  if(access(SERVER_PIPE,F_OK) < 0){

    printf (" \n\nServer Is Down, try later..!\n\n");

    exit(0);
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//funçao inicial do cliente onde cria  e abre pipes e para alem disso tambem cria a THREAD
void Client_console()
{
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

void HandleSignal(int s){
  if(s == SIGINT)
  {
    printf("Please, choose exit to leave!\n");
    return;
  }

  return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main ( int argc, char * argv[])
  {
    signal(SIGINT,HandleSignal);
    setbuf(stdout, NULL);

    verify_server();
    Client_console();

    return 0;
  }
