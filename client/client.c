#include "client.h"

void create_user()
{
  printf (" em construçao");
}
void menu_jogador(){

  printf (" em construçao");
}

void verify_login()
{
    int flag = 0, i = 0;

    char c, username[MAX], password[MAX];


    existing_users verify;

    FILE *f;

    f = fopen (FILE_USERS, "rt");

    if ( f == NULL ){
      printf ("Error open file users.data");
      return;
    }
    printf ("\n<< Login >>\n");
    printf ("\nUsername: ");
    scanf ("%s", username);
    printf ("\nPassword: ");

    while( c != '\n')
    {
      scanf ("%c", &c);
      password[i] = c;
      printf("*");
      i++;
    }
    while ( fscanf (f, " %s %s ", verify.name, verify.pass) == 2 ){

      if ( strcmp ( verify.name, username ) == 0 )
      {
        if ( strcmp ( verify.pass, password ) == 0 )
          flag = 1;
      }
    }
    if ( flag = 1 )
      menu_jogador();
    else
      return;
}

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
