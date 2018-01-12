#ifndef COMUN_INFO_H
#define COMUN_INFO_H


#define MAX 50
#define SERVER_PIPE "../Data/pipes/ServerPipe"
#define CLIENT_PIPE_TEMPLATE "../Data/pipes/Client_%d"

//#define SERVER_PIPE "/tmp/ServerPipe"
//#define CLIENT_PIPE_TEMPLATE "/tmp/Client_%d"

#define USERS_LOGIN_DATA "../Data/users.data"

//tipo de mensagem.
enum MessageType {
  USER_COM = 0,         //Client connection
  USER_AUTH,            //User authentication
  USER_REQUEST_CRED,    //User Request Cred
  USER_ACTION,          //Movement
  USER_EXIT,            //User Logout
  USER_SHUTDOWN,        //Client exit Terminal
  USER_REQUEST_LOGOUT,  //pedido de logout pelo utilizador
  USER_CHAT,            //Mensagem de chat
  USER_PLAY,            //Mensagem de inicio de jogo
  USER_QUIT             //Mensagem de quit do jogo mas nao da aplicacao.
};

//Resposta DA MENSAGEM USER_AUTH
enum User_Auth_Resp {
  USER_LOGIN_ACCEPTED       =    0,
  USER_DOESNT_EXIST         =   -1,
  USER_LOGIN_WRONG_PASS     =   -2,
  USER_ALREADY_IN           =   -4,
  SERVER_FULL               =   -5
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
  COMMAND_JUMP1 = 97,   //TODO Rever isto
  COMMAND_JUMP2 = 65,
  COMMAND_QUIT1 = 113,  //TODO Rever isto
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
  char msg[30];
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
  FREE = 0,      //Pos Livre
  WALL,          //parede Destrutivel
  IRON_WALL,     //Parede indestrutivel
  ENEMY,
  PLAYER,
  BOMB,
  MEGABOMB,
  OBJECT,
  BONUS,
  EXIT
};

enum Server_Response {
  LOGIN_RESPONSE,    // resposta de pedido de login
  SERVER_SHUTDOWN,   // resposta servidor vai encerrar
  LOGOUT_RESPONSE,   // resposta utilizador vai fazer shutdown
  SERVER_KICK,       // reposta utilizado vai kickar
  MAP,
  CHAT
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
  int orientation;
  //int vidas;            //TODO: int vidas ? se houve tempo
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
  char msg[100];
  int time;
} gameInfo;


#endif /* COMUN_INFO_H */
