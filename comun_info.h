#ifndef COMUN_INFO_H
#define COMUN_INFO_H

//Colors
#define BLACK   "\x1B[30m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"
#define RESET   "\x1B[0m"

#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"
#define BOLDWHITE   "\033[1m\033[37m"

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
  COMMAND_SMALLBOMB = 'z',
  COMMAND_BIGBOMB = 120,
  COMMAND_JUMP = ' ',   //TODO Rever isto
  COMMAND_JUMP2 = 65,
  COMMAND_QUIT1 = 113,  //TODO Rever isto
  COMMAND_QUIT2 = 81,
  COMMAND_CHAT = '\n'
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

enum ORIENTATION {
  up = 0,
  down,
  left,
  right
};
//Mensagens de controlo do servidor
enum cellType {
  FREE = ' ',      //Pos Livre
  WALL = '|',          //parede Destrutivel
  IRON_WALL = '#',     //Parede indestrutivel
  ENEMY = 'E',
  PLAYER = 'B',
  BOMB = 'o',
  MEGABOMB = '0',
  OBJECT = '@',
  BONUS = '*',
  EXPLOSION = '=',
  EXIT = 'X'
};

enum Server_Response {
  LOGIN_RESPONSE,    // resposta de pedido de login
  SERVER_SHUTDOWN,   // resposta servidor vai encerrar
  LOGOUT_RESPONSE,   // resposta utilizador vai fazer shutdown
  SERVER_KICK,       // reposta utilizado vai kickar
  SERVER_MAP,
  SERVER_CHAT
};

////////////////////////////////////////////
typedef struct ENEMY {
    pthread_t enemy_ID;    //ID da thread do inimigo
    alive;
    int posx,posy;         //Posicao do inimigo
    char drop;             //Bonus que o inimigo poderá eventualmente deixar ao morrer.
}Enemy;

typedef union INFO {
  int login_answer;
  char msg[30];
}Info;

typedef struct PLAYER {
  int PID;                //Para identificar o proprio jogador.
  int score;              //Score do jogador
  int bomb;               //Bombinhas
  int megabomb;
  int orientation;
  int posx,posy;
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

  Map map[21][31];
  int login_answer;
  char msg[100];
  int time;
} gameInfo;



#endif /* COMUN_INFO_H */
