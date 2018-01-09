typedef struct Game {
  int time;       //Tempo limite do jogo NOTE: E preciso ?
  Segment * map;  //Cunjunto de segmentos que irao constituir o mapa.
}Game;

typedef struct Bomb {
  int nBombs;        //Numero de bombas disponiveis
  int posx,posy;     //Posição da bomba (ainda nao definida)
  int explosionSize; //Tamanho explosao
}Bomb;

typedef struct player {
  int posx,posy;     //Posicao do jogador
  int score;         //Score do jogador
  Bomb nuke;         //bombas
  Bomb grenade;      //Bombinhas

  //int vidas NOTE: considerar ?
}Player;

typedef struct enemy {
    pthread_t enemy_ID;    //ID da thread do inimigo
    int posx,posy;         //Posicao do inimigo
    char drop;             //Bonus que o inimigo poderá eventualmente deixar ao morrer.
}Enemy;

typedef struct exit {
  int open;       //Saida eberta ou fechada
  int posx,posy;  //Posicao da saida
}Exit;


//NOTE: Redundante ?
typedef struct Objective {
  int value;     //Pontuação ao obter objetivo
  int posx,posy; //Posicao do objetivo
}Objective;



//FIXME:Falta planear melhor a grelha aqui...
typedef union block {
  char wall;
  char bonus;
  Objective objective;
  Enemy * enemy;
  Player * player;
  Exit exit;
  Bomb bomb;
  
}Block;

typedef struct Segment {
  int type; //tipo de dados que esta em bloco
  Block block;  //Dados do segmento
}Segment;

typedef struct topTen {
  int pos;
  char nome[100];
  int score;
}Top10;
