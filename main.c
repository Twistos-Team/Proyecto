#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "Blackjack.c"

int main(){
  srand(time(NULL));
  while(menuPrincipal()){}

  printf("\n\nGracias por jugar ^-^\n > ");
  fflush(stdin);
  getchar();
    return 0;
}