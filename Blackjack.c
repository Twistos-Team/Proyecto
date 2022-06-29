#include <time.h>
#include <stdio.h>

#include "bj.c"

int main(){
  srand(time(NULL));

  system("COLOR 2F");
  while(menuPrincipal()){}
  
  OUCH;
  system("COLOR F4");
  system("cls");

  printf("\n\n   %c Gracias por jugar %c", 3,3);

  fflush(stdin);
  getchar();
    return 0;
}