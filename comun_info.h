#ifndef COMUN_INFO_H

#define COMUN_INFO_H



#define MAX 50



//#define SERVER_PIPE "../Data/ServerPipe" TODO: Trocar isto

#define SERVER_PIPE "/tmp/ServerPipe"

#define CLIENT_PIPE_TEMPLATE "/tmp/Client_%d"

//#define CLIENT_PIPE_TEMPLATE "../Data/Client_%d" TODO: Trocar isto

#define USERS_LOGIN_DATA "../Data/users.data"


//tipo de mensagem.
#define USER_COM            0 //Client connection
#define USER_AUTH           1 //User authentication
#define USER_REQUEST_CRED   2 //User Request Cred
#define USER_ACTION         3 //Movement
#define USER_EXIT           4 //User Logout
#define USER_SHUTDOWN       5 //Client exit Terminal
#define USER_REQUEST_LOGOUT 6 // pedido de logout pelo utilizador

//Resposta DA MENSAGEM USER_AUTH
#define USER_LOGIN_ACCEPTED      0
#define USER_DOESNT_EXIST       -1
#define USER_LOGIN_WRONG_PASS   -2
#define USER_ALREADY_IN         -4
#define SERVER_FULL             -5

//Mensagens de controlo do servidor
#define SERVER_ANSWER_AUTH    20
#define SERVER_KICK           21
#define SERVER_AUTH           22
#define SERVER_REQ_RESP       23
#define SERVER_GAME_INFO      24
#define SERVER_SHUTDOWN       25
#define SERVER_ANSWER_LOGOUT  26


//Estrutura usada para ser enviada pelo cliente para ser verificado o seu login

typedef struct USER_LOGIN_
{
  int PID;
  char username[MAX];
  char password[MAX];
  int try_login;
}Login;

// estrutura que será enviada com o request para o servidor adicionar o novo utilizador;

typedef struct MSG_USER_EXIT
{
  int TYPE;
  int PID;
}User_Exit;
// Extrutura que representa o movimento do Bomberman a ser enviada para o servidor;
typedef union ACTION
{
  Login login_request;  // pedido de login por parte do jogador;
  int login_answer;     // resposta de login do SERVIDOR
  User_Exit user_exit;  // mensagem a enviar que o Client vai sair do programa   TODO
  int server_shutdown;  // inteiro enviado para informar que servidor vai encerrar

}Action;

typedef struct PACKAGE
{
  int TYPE;
  int PID;
  Action action;
}Package;


















//union onde irão estar todas as açoes que poderam ser feitas, bomberman, Client, user etc:...

/*typedef struct ACTION

{

  int PID;

  int Movement;

                  // variavel para defenir possiveis tarefas do bomberman;

                  //    valor      assignment        Tecla

                  // Movement = 1   NORTH           up key

                  // Movement = 2   EAST            left key

                  // Movement = 3   SOUTH           low key

                  // Movement = 4   WEST            right key

                  // Movement = 5   bomb nuke       C key

                  // Movement = 6   Bomb granede    D key

                  // Movement = 7   jump            SPACE key



}Action;

// Extrutura que representa o movimento do Bomberman a ser enviada para o servidor;

typedef struct MSG_BomberMan_Movement

{

  int type;

  Action action;

}Movement;
*/
#endif /* COMUN_INFO_H */
