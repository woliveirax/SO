#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FILE_USERS "../Data/users.data"
#define MAX 50

typedef struct EXISTING_USERS {

  char name[MAX];
  char pass[MAX];

}existing_users;

int main(int argc, char * argv[]);

#endif /* CLIENT_H */
