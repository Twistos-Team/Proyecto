#include <time.h>
#include <stdio.h>

#include "Blackjack.c"

int main(){
  srand(time(NULL));

  system("COLOR A0");
  while(menuPrincipal()){}
  
  OUCH;
  system("COLOR F4");
  system("cls");

  printf("\n\n   %c Gracias por jugar %c", 3,3);

  fflush(stdin);
  getchar();
    return 0;
}