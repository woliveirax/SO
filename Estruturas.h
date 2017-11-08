#include <stdio.h>

typedef struct Player {
  int id;
  int x,y;

};

typedef union objecto {

  char parede; //Indestructivel(◘) ou Destrutivel (D)
  char bonus; //Um dos bonus (a averiguar);
  Inimigo inimigo; //TODO Pode ser alterado
  Player jogador;
  Porta saida; //TODO Pensar no bonus moedas classificação
  Bomba bomba; //Megabomba ou bombinha (piquinina)

} ,Objectos;


typedef struct Quadrado {
  int x,y;

  Objectos objecto; //TODO repensar este nome de caralhinho
};

// Estruturas:

/*
 1 - Bocados de mapa ( Objecto )

 estrutura - xy
  union - 1 dos campos

*/
