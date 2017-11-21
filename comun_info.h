#ifndef COMUN_INFO_H
#define COMUN_INFO_H

#define MAX 50

// estrutura que será enviada com o request para o servidor adicionar o novo utilizador;
typedef struct REQ_REG_USER
{
  username[MAX];
  password[MAX];
}request_user;
/////////////////////////////////////////////////


#endif /* COMUN_INFO_H */
