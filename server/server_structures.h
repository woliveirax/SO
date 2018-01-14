#ifndef SERVER_STRUCTURES_H
#define SERVER_STRUCTURES_H

#define users_file "../Data/users.data"
#define buf_size 1024
#define MAX 50
#define MAX_CLI 20

typedef struct BOMB {
  pthread_t bomb_id;
  int posx,posy;          //Posição da bomba (ainda nao definida)
  int explosionSize;      //Tamanho explosao
}Bomb;
/*
typedef struct ENEMY {
    pthread_t enemy_ID;    //ID da thread do inimigo
    int posx,posy;         //Posicao do inimigo
    char drop;             //Bonus que o inimigo poderá eventualmente deixar ao morrer.
}Enemy;
*/

typedef struct client {
  int PID;                    //PID do jogador
  int FD;                     //FD do pipe do jogador
  int inGame;
  char username[MAX];         //Username do jogador
  Player *player;             //Ponteiro para o boneco do jogador

} Client;

typedef struct RCVED_DATA {
  Client clients[MAX_CLI];    //Guarda array de clientes ligados ao servidor
  int nClients;               //Contador de jogadores
}ClientsData;

#endif
