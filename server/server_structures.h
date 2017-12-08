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
  int posx,posy;     //Posicao do jogador
  int score;         //Score do jogador
  Bomb nuke;         //bombas
  Bomb grenade;      //Bombinhas

  //int vidas NOTE: considerar ?
}Player;

typedef struct client {
  int PID;                    //PID do jogador
  int FD;                     //FD do pipe do jogador
  char username[MAX];         //Username do jogador
  Player *player;             //Ponteiro para o boneco do jogador

} Client;

typedef struct RCVED_DATA {
  Client clients[MAX_CLI];    //Guarda array de clientes ligados ao servidor
  int nClients;               //Contador de jogadores
}ClientsData;

#endif
