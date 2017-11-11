
typedef struct Game {
  int PID;
  Segment map[30][60];
  Client clients[20];

}, Game; //Might need it or not.

typedef struct client {
  int PID;
  char username[50];
},Client;

typedef struct Bomb {
  int nBombs;
  int posx,posy; // TODO Redundant ?
  int bombSize;
  int explosionTime;
  int triggerTime; //TODO: maybe ?

},Bomb;

typedef struct player {
  int id;
  int posx,posy;
  int score;
  Bomb nuke;
  Bomb grenade;

  //int life quantity TODO: Consider
},Player;

typedef struct enemy {
    int health;
    int posx,posy;
    char drop; //TODO ou struct, rever.
},Enemy;

typedef struct exit {
  int open;
  int posx,posy;
},Exit;

typedef struct Obective {
  int value;
  int posx,posy;
}, Objective;

typedef union block {
  char wall;
  char bonus;
  Enemy enemy;
  Player player;
  Exit exit;
  Bomb bomb;
  O
} ,Block;

typedef struct Segment {
  int type;
  Block block;
},Segment;
