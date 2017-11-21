typedef struct Game {
  int PID;
  Segment *map; // 20x30
  Client *clients[20];

}, Game;

typedef struct client {
  int PID;
  char username[50];
  char password[50];
  Player *player;

},Client;

typedef struct Bomb {
  int nBombs;
  int posx,posy;
  int explosionSize; //Tamanho explosao
},Bomb;

typedef struct player {
  int PID;
  int posx,posy;
  int score;
  Bomb nuke;
  Bomb grenade;

  //int life quantity TODO: Consider
},Player;

typedef struct enemy {
    int ID;         //ID do inimigo
    int posx,posy;  //Posicao
    char drop; //TODO ou struct, rever. //o drop que o inimigo pode deixar quando morre
},Enemy;

typedef struct exit {
  int open;       //Saida eberta ou fechada
  int posx,posy;  //Posicao da saida
},Exit;

typedef struct Objective {
  int value;
  int posx,posy;
}, Objective;



//Tagged union de maneira a ser somente um dos tipos a existerem em cada segmento do mapa
//Polpando assim a quantidade de dados a enviar p o processo
typedef union block { //falta testar - Tagged Union - Possivel alteração
  char wall;
  char bonus;
  Objective objective;
  Enemy *enemy;
  Player *player;
  Exit exit;
  Bomb bomb;

} ,Block;

typedef struct Segment {
  int type; //tipo de dados que esta em bloco
  Block block;  //Dados do segmento
},Segment;

typedef struct server {
  Client *clientes;
  Game *Game;
}, Server;

typedef struct topTen {
  int pos;
  char nome[100];
  int score;
}, Top10;
