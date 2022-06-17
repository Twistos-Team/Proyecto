#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Map.c"

void blackjack(int *cantJugadores){
  // Se verifica si los jugadores tienen dinero( =! 0)
  
  // Se crean los datos para el croupier (en desarrollo)
  
  printf("** Empiezan las apuestas! **\n\n");
  // Inicio de apuestas
  
  for  (int i = 0 ; i < *cantJugadores ; i++){
    printf("(Nombre Jugador)\nIngrese su apuesta: \n > ");
    // Aqui cada jugador apuesta
    // Se comprueba si tiene el dinero suficiente
    printf("\n");
  }
  printf("\n\n");

  printf("** Se reparten cartas iniciales **\n\n");
  for (int i = 0 ; i < *cantJugadores ; i++){
    //Función para repartir las cartas iniciales
  }

  for (int i = 0 ; i < *cantJugadores ; i++){
    printf("Turno de (Nombre Jugador)\n");
    // Se muestra la mano del jugador

    printf("Que desea hacer?\n");
    //opcionesJugador(); Funcion que muestra las opciones
    printf("\n");
  }
  printf("\n\n");

  printf("** Turno del Croupier **\n");
  // while(sumaCroupier >= 17) repetitivamente sacará cartas

  printf("\nCalculando resultado...\n\n");

  // Se repartiran las ganancias de los jugadores victoriosos

  // Sistema en desarrollo
  int respuesta;
  int cantRetirada = 0;

  for (int i = 0 ; i < *cantJugadores ; i++){
    printf("(Nombre Jugador)\nDesea retirarse?\nSi(1)\nNo(2)\n > ");
    scanf("%d", &respuesta);
    if (respuesta == 1) cantRetirada++;
    // Se le elimina del Mapa
    printf("\n\n");
  }
  (*cantJugadores) -= cantRetirada;

  // Pregunta por nuevos jugadores
}

bool menuPrincipal(){
  printf("**** BLACKJACK ****\n\n");
  
  printf("Empezar juego (1)\nSalir (2)\n > ");
  int respuesta;
  scanf("%d", &respuesta);

  if (respuesta != 1) return false;

  // Preparación juego
  printf("\nCuantos jugadores son? (Maximo 5)\n");
  int * cantJugadores = (int*)calloc(1, sizeof(int));

  // Cantidad de jugadores
  while(true){
    printf(" > ");
    scanf("%d", cantJugadores);
    printf("\n");
    
    if (0 < *cantJugadores && *cantJugadores <= 5) break;
    printf("Número no valido\n");
  }
  printf("\n");

  // Creación Mapa de jugadores
  //Map * jugadores = createMap();
  for (int i = 0 ; i < *cantJugadores ; i++){
    printf("Jugador %d: \n", i+1);
    printf("(Se piden datos)\n");
  }
  printf("\n\n");

  // BLACKJACK!
  while(*cantJugadores != 0){
    blackjack(cantJugadores);
  }
  // Se debe pasar la cantidad y el mapa de jugadores
  return true;
}

int main(){
  while(menuPrincipal()){}

  printf("\n\nGracias por jugar ^-^\n > ");
  fflush(stdin);
  getchar();
    return 0;
}