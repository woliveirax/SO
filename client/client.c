#include "client.h"
// funçao que vai fazer o request ao servidor para registar o novo user
void create_user()
{
  printf (" em construçao será criado um pacote para ser enviado ao servidor para o utilizador ser registado");
}

// funçao menu do jogador;

void menu_jogador(){

  printf (" em construçao");
}

// funçao verifica e confirma login dos users;
void verify_login()
{
    int flag = 0, i = 0;

    char c, username[MAX], password[MAX];


    existing_users verify;


    if(access(file,F_OK) < 0)
    {
      printf ("\nThere isn't a user with this name\n");
      return;
    }

    FILE *f;

    f = fopen (FILE_USERS, "rt");

    if ( f == NULL ){
      printf ("Error openning file '%s' :",FILE_USERS);
      perror ("");
      return;
    }

    printf ("\n<< Login >>\n");
    printf ("\nUsername: ");
    scanf (" %s", username);

    printf ("\nPassword: ");
    scanf (" %s", password);

    while ( fscanf (f, " %s %s ", verify.name, verify.pass) == 2 ){
      printf ("%s\n", verify.name);
      printf ("%s \n", verify.pass);
      if ( strcmp ( verify.name, username ) == 0 )
      {
        if ( strcmp ( verify.pass, password ) == 0 )
        {
          flag = 1;
        }
      }
    }
    if ( flag == 1 )
    {
      menu_jogador();
    }else
      {
        printf ("\nThere isn't a user with this name\n");
        return;
      }
}
// funçao que inicia as opçoes para cada cliente;
int main (int argc, char * argv[])
{
    int choice;

    while(choice < 1 || choice > 3)
    {
      printf("\n<<  Client  >>");
      printf("\n  1. Login.  ");
      printf("\n  2. Register.");
      printf("\n  3. Exit.\n");
      printf("\nChoice: ");
      scanf (" %d", &choice);

      switch (choice)
      {
        case 1 :
          verify_login();
          break;
        case 2 :
          create_user(); // TODO isto será para ser feito o pedido de criaçao de user ao admin no servidor;
          break;
        case 3 :
          exit (0);
      }
    }
  }
