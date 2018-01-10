#ifndef COMUN_INFO_H
#define COMUN_INFO_H

#define MAX 50
#define SERVER_PIPE "../Data/ServerPipe"
#define CLIENT_PIPE_TEMPLATE "../Data/Client_%d"

//#define SERVER_PIPE "/tmp/ServerPipe"
//#define CLIENT_PIPE_TEMPLATE "/tmp/Client_%d"

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

//Representacao

enum messageType {
  LOGIN = 0,
  LOGOUT,
  COMMAND,
  CHAT
}

enum commandos {
  UP = KEY_UP,
  DOWN = KEY_DOWN,
  RIGHT = KEY_RIGHT,
  LEFT = KEY_LEFT,
  SMALLBOMB = KEY_Z   //TODO REVER ISTO
  BIGBOMB = KEY_X     //TODO REVER ISTO
  JUMP = KEY_SPACE
  QUIT = KEY_Q
  CHAT = KEY_ENTER
}

//PACOTE DO CLIENTE:
typedef struct USER_LOGIN_
{
  char username[MAX];
  char password[MAX];
  int try_login;
  int login_answer;
}Login;

// Extrutura que representa o movimento do Bomberman a ser enviada para o servidor;
typedef union ACTION
{
  int key;
  Login login_request;  // pedido de login por parte do jogador;
}Action;

typedef struct PACKAGE_CLI
{
  int TYPE;
  int PID;
  Action action;
}Package_cli;


//PACOTE DO SERVIDO:
typedef struct Bomb {
  pthread_t bomb_id;
  int posx,posy;     //Posição da bomba (ainda nao definida)
  int explosionSize; //Tamanho explosao
}Bomb;

typedef struct enemy {
    pthread_t enemy_ID;    //ID da thread do inimigo
    int posx,posy;         //Posicao do inimigo
    char drop;             //Bonus que o inimigo poderá eventualmente deixar ao morrer.
}Enemy;

typedef struct player {
  int PID;           //Para identificar o proprio jogador.
  int posx,posy;     //Posicao do jogador
  int score;         //Score do jogador
  int bomb;          //Bombinhas
  int megabomb;
  //TODO: int vidas ? se houve tempo
}Player;

enum cellType {
  FREE = 0,   //Pos Livre
  WALL,       //parede Destrutivel
  IRON_WALL,  //Parede indestrutivel
  ENEMY,
  PLAYER,
  BOMB,
  MEGABOMB,
  OBJECT,
  BONUS,
  EXIT,
  LOGIN_RESPONSE,
  CHAT
}

typedef union info {
  int login_answer;
  int exit;
  char msg[30];
  Player player;

}Info;

typedef struct gameInfo {
  cellType a;
  Info info;
  int time;
} gameInfo
