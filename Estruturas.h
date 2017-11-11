#include <stdio.h>

typedef struct Data {

}, Data; //Might need it or not.

typedef struct client {
  int PID;
  char username[50];
},Client;

typedef struct Bomb {
  int nBombs;
  int posx,posy; // TODO Redundant ?
  int bombSize;
  int triggerTime; //TODO: maybe ?

},Bomb;

typedef struct player {
  int id;
  int posx,posy;
  Bomb nuke;
  Bomb grenade;
  //int life quantity TODO: Consider
},Player;

typedef struct enemy {
    int health;
    int posx,posy;
},Enemy;

typedef struct exit {
  int open;
},Exit;

typedef union block {
  char wall;
  char bonus;
  Enemy enemy;
  Player player;
  Exit exit;
  Bomb bomb;
} ,Block;

typedef struct object {
  int type;
  Block block;
},Object;
