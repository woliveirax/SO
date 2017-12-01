#ifndef SERVER_STRUCTURES_H
#define SERVER_STRUCTURES_H

#define users_file "../Data/users.data"
#define buf_size 1024
#define MAX 50
#define MAX_CLI 20

typedef struct Bomb {
  int nBombs;
  int posx,posy;
  int explosionSize; //Tamanho explosao
}Bomb;

typedef struct player {
  int PID;
  int posx,posy;
  int score;
  Bomb nuke;
  Bomb grenade;
} Player;

typedef struct client {
  int PID;
  int FD;
  char username[MAX];
  Player *player;

} Client;

typedef struct RCVED_DATA {
  Client clients[MAX_CLI];
  int nClients;
}ClientsData;

#endif
