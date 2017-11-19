
typedef struct Game {
  int PID;                // Process ID do jogo
  Segment *map; // 20x30  // Ponteiro para o mapa que será alocado antes do jogo começar
  Client *clients[20];    //Ponteiro para os clientes que estao ao jogar

}, Game;

typedef struct client {
  int PID;    // Process ID do cliente
  char username[50];  //Username do cliente
  char password[50];  //Password do cliente
  Player *player;     //Ponteiro para o jogador -- Eventualmente para a funcao kick

},Client;

typedef struct Bomb {
  int nBombs;       //Numero de bombas
  int posx,posy;    //Posicao da bomba
  int explosionSize; //Tamanho explosao //Para alterar entra megabomba e bombinha
},Bomb;

typedef struct player {
  int PID;          //Process ID do jogador
  int posx,posy;    //Posicao do jogador
  int score;        //Pontuação do jogador
  Bomb nuke;        //Megabomb
  Bomb grenade;     //Bombinha

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
  int value;          //Valor do objetivo
  int posx,posy;      //Posicao do objetivo
}, Objective;



//Tagged union de maneira a ser somente um dos tipos a existerem em cada segmento do mapa
//Polpando assim a quantidade de dados a enviar p o processo
typedef union block { //falta testar - Tagged Union - Possivel alteração
  char wall;          //Parede (Tipo, A ,B ,C ) Esta em pensamento
  char bonus;         //Bonus (aumento da bomba ou n de bombas ou vida) --- esta em pensamento
  Objective objective; //Objetivos
  Enemy *enemy;       //Ponteiro para monstro
  Player *player;     //Ponteiro para jogador
  Exit exit;          //Saida do labirinto que podera estar aberta ou fechada
  Bomb bomb;          //Bomba deixada por um jogador.

} ,Block;

typedef struct Segment {
  int type; //tipo de dados que esta em bloco
  Block block;  //Dados do segmento
},Segment;
