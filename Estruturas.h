
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
    int ID;
    int posx,posy;
    char drop; //TODO ou struct, rever.
},Enemy;

typedef struct exit {
  int open;
  int posx,posy;
},Exit;

typedef struct Objective {
  int value;
  int posx,posy;
}, Objective;

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
  int type; //Pode alterar
  Block block;
},Segment;
