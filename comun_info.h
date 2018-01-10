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

//Representacao

enum messageType {
  CLIENT_LOGIN = 0,
  CLIENT_LOGOUT,
  CLIENT_COMMAND,
  CLIENT_CHAT
};

enum  commands {
  COMMAND_UP = KEY_UP,
  COMMAND_DOWN = KEY_DOWN,
  COMMAND_RIGHT = KEY_RIGHT,
  COMMAND_LEFT = KEY_LEFT,
  COMMAND_SMALLBOMB1 = 122,   //TODO REVER ISTO
  COMMAND_SMALLBOMB2 = 90,
  COMMAND_BIGBOMB1 = 120,
  COMMAND_BIGBOMB2 = 88,     //TODO REVER ISTO
  COMMAND_JUMP1 = 97,
  COMMAND_JUMP2 = 65,
  COMMAND_QUIT1 = 113,
  COMMAND_QUIT2 = 81,
  COMMAND_CHAT = KEY_ENTER
};

//##############################################################################
//                              PACOTE DO CLIENTE:
//##############################################################################
// estrutura para que o Cliente consiga fazer o login
typedef struct USER_LOGIN_
{
  char username[MAX];
  char password[MAX];
  int try_login;                // vezes que um cliente consegue tentar Logar no servidor;
  int login_answer;             // Resposta que virá do servidor para fazer o Login;
}Login;

// Extrutura que representa o movimento do Bomberman a ser enviada para o servidor;
typedef union ACTION
{
  int key;
  Login login_request;         // pedido de login por parte do jogador;
}Action;
// Estrura Cabeçalho do Cliente
typedef struct PACKAGE_CLIENTE
{
  int TYPE;                   // tipo de Pacote
  int PID;                    // PID do Cliente que manda o Pacote
  Action action;              // respetiva Acçao a tomar mediante o tipo de Pacote
}Package_Cli;

//##############################################################################
//                              PACOTE DO SERVIDOR:
//##############################################################################

//Mensagens de controlo do servidor
enum cellType {
  cellType_FREE = 0,      //Pos Livre
  cellType_WALL,          //parede Destrutivel
  cellType_IRON_WALL,     //Parede indestrutivel
  cellType_ENEMY,
  cellType_PLAYER,
  cellType_BOMB,
  cellType_MEGABOMB,
  cellType_OBJECT,
  cellType_BONUS,
  cellType_EXIT,
  cellType_LOGIN_RESPONSE,    // resposta de pedido de login
  cellType_SERVER_SHUTDOWN,   // resposta servidor vai encerrar
  cellType_LOGOUT_RESPONSE,   // resposta utilizador vai fazer shutdown
  cellType_SERVER_KICK,       // reposta utilizado vai kickar
  cellType_CHAT
};

typedef union INFO {
  int login_answer;
  char msg[30];
}Info;

typedef struct PLAYER {
  int PID;                //Para identificar o proprio jogador.
  int posx,posy;          //Posicao do jogador
  int score;              //Score do jogador
  int bomb;               //Bombinhas
  int megabomb;
  //int vidas;                //TODO: int vidas ? se houve tempo
}Player;

typedef struct map
{
  int type;

  int exit;
  Player player;
}Map;

typedef struct GAMEINFO {
  int type;

  Map map[20][30];
  int login_answer;
  char msg[30];
  int time;
} gameInfo;


#endif /* COMUN_INFO_H */
