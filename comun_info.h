#ifndef COMUN_INFO_H
#define COMUN_INFO_H

#define MAX 50

// estrutura que será enviada com o request para o servidor adicionar o novo utilizador;
typedef struct REQ_REG_USER
{
  username[MAX];
  password[MAX];
}request_user;
//union onde irão estar todas as açoes que poderam ser feitas, bomberman, Client, user etc:...
typedef union ACTION
{
// variavel para defenir possiveis tarefas do bomberman;
  //  valor  assignment      Tecla
  // B = 1   NORTH           up key
  // B = 2   EAST            left key
  // B = 3   SOUTH           low key
  // B = 4   WEAST           right
  // B = 5   bomb nuke       C key
  // B = 6   Bomb granede    D key
  // B = 7   jump            SPACE key
  int MOV_Bomberman;
// variavel para defenir tarefas do ENEMY;
  // value   assignment      key
  // E = 1   NORTH           up key
  // E = 2   EAST            left key
  // E = 3   SOUTH           low key
  // E = 4   WEAST           right
  int MOV_Enemy;
// extrutura que irá servir para enviar os dados do cliente para efetuar o login;
  request_user request_user;
}action;
//estrutura pacote a enviar com as enumeras açoes que podem existir
typedef struct Package
{
  int type;
  action action;

}pack;
#endif /* COMUN_INFO_H */
