#ifndef COMUN_INFO_H

#define COMUN_INFO_H



#define MAX 50



//#define SERVER_PIPE "../Data/ServerPipe" TODO: Trocar isto

#define SERVER_PIPE "/tmp/ServerPipe"

#define CLIENT_PIPE_TEMPLATE "/tmp/Client_%d"

//#define CLIENT_PIPE_TEMPLATE "../Data/Client_%d" TODO: Trocar isto

#define USERS_LOGIN_DATA "../Data/users.data"


//tipo de mensagem.
#define USER_COM      0 //Client connection
#define USER_AUTH     1 //User authentication
#define USER_REQUEST  2 //User Request Cred
#define USER_ACTION   3 //Movement
#define USER_EXIT     4 //Client Exit

//Resposta DA MENSAGEM USER_AUTH
#define USER_LOGIN_ACCEPTED      0
#define USER_DOESNT_EXIST       -2
#define USER_LOGIN_WRONG_PASS   -3
#define USER_ALREADY_IN         -4
#define SERVER_FULL             -5

//Mensagens de controlo do servidor
#define SERVER_KICK           20
#define SERVER_AUTH           21
#define SERVER_REQ_RESP       22
#define SERVER_GAME_INFO      23
#define SERVER_SHUTDOWN       24


//Estrutura usada para ser enviada pelo cliente para ser verificado o seu login

typedef struct REQ_REG_USER
{
  int PID;
  char username[MAX];
  char password[MAX];
}Login;

// estrutura que será enviada com o request para o servidor adicionar o novo utilizador;

typedef struct MSG_REQ_REG_USER
{
  int type;
  Login login;
}MSG_Login;


typedef struct USER_OPTION
{
  int PID;
  int ACTION;
}USER_action;

typedef struct MSG_USER_OPTION
{
  int type;
  USER_action user;
}msg_user_option;


//union onde irão estar todas as açoes que poderam ser feitas, bomberman, Client, user etc:...

typedef struct ACTION

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

#endif /* COMUN_INFO_H */
