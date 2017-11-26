#ifndef COMUN_INFO_H
#define COMUN_INFO_H

#define MAX 50

//#define SERVER_PIPE "../Data/ServerPipe" TODO: Trocar isto
#define SERVER_PIPE "/tmp/ServerPipe"
#define CLIENT_PIPE_TEMPLATE "/tmp/Client_%d"
//#define CLIENT_PIPE_TEMPLATE "../Data/Client_%d" TODO: Trocar isto

#define USER_COM      0 //Client connection
#define USER_AUTH     1 //User authentication
#define USER_REQUEST  2 //User Request Cred
#define USER_ACTION   3 //Movement
#define USER_EXIT     4 //Client Exit

#define SERVER_KICK           20
#define SERVER_AUTH           21
#define SERVER_REQ_RESP       22
#define SERVER_SEND_GAME_INFO 23
//#define SERVER_KILL_CLI       24

// estrutura que será enviada com o request para o servidor adicionar o novo utilizador;

typedef struct MSG_REQ_REG_USER
{
  int type;
  Login login;

}MSG_Login;


typedef struct REQ_REG_USER
{
  int PID;
  char username[MAX];
  char password[MAX];

}Login;

typedef struct MSG_BomberMan_Movement
{
  int type;
  Action
}Movement;

//union onde irão estar todas as açoes que poderam ser feitas, bomberman, Client, user etc:...
typedef struct ACTION
{
  int PID;
  // variavel para defenir possiveis tarefas do bomberman;
  //  valor  assignment      Tecla
  // B = 1   NORTH           up key
  // B = 2   EAST            left key
  // B = 3   SOUTH           low key
  // B = 4   WEST           right
  // B = 5   bomb nuke       C key
  // B = 6   Bomb granede    D key
  // B = 7   jump            SPACE key
  int Movement;


}Action;
//estrutura pacote a enviar com as enumeras açoes que podem existir
#endif /* COMUN_INFO_H */
