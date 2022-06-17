#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Map.c"

typedef struct{
  char valor;
  char pinta;
} Carta;

typedef struct{
  char * nombre;
  int dinero;
  int suma;
  int apuestaTurno;
  Carta mano[6];
} Jugador;

int is_equal_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

void chomp(char *s) {
    while(*s && *s != '\n' && *s != '\r') s++;
    *s = 0;
}

void blackjack(int *cantJugadores, Map *jugadores){
  // Se verifica si los jugadores tienen dinero( =! 0)
  
  // Se crean los datos para el croupier (en desarrollo)
  
  printf("** Empiezan las apuestas! **\n\n");
  // Inicio de apuestas
  
  Jugador * aux = firstMap(jugadores);
  for  (int i = 0 ; i < *cantJugadores ; i++){
    printf("\"%s\"\nIngrese su apuesta: \n > ", aux->nombre);
    // Aqui cada jugador apuesta
    // Se comprueba si tiene el dinero suficiente
    aux = nextMap(jugadores);
    printf("\n");
  }
  printf("\n\n");

  printf("** Se reparten cartas iniciales **\n\n");
  for (int i = 0 ; i < *cantJugadores ; i++){
    //Función para repartir las cartas iniciales
  }

  aux = firstMap(jugadores);
  for (int i = 0 ; i < *cantJugadores ; i++){
    printf("Turno de \"%s\"\n", aux->nombre);
    // Se muestra la mano del jugador

    printf("Que desea hacer?\n");
    //opcionesJugador(); Funcion que muestra las opciones
    printf("\n");
    aux = nextMap(jugadores);
  }
  printf("\n\n");

  printf("** Turno del Croupier **\n");
  // while(sumaCroupier >= 17) repetitivamente sacará cartas

  printf("\nCalculando resultado...\n\n");

  // Se repartiran las ganancias de los jugadores victoriosos

  // Sistema en desarrollo
  int respuesta;
  int cantRetirada = 0;

  
  aux = firstMap(jugadores);
  for (int i = 0 ; i < *cantJugadores ; i++){
    printf("\"%s\"\nDesea retirarse?\nSi(1)\nNo(2)\n > ", aux->nombre);
    scanf("%d", &respuesta);
    if (respuesta == 1) cantRetirada++;
    
    eraseMap(jugadores, aux->nombre);
    aux = nextMap(jugadores);
    printf("\n\n");
  }
  (*cantJugadores) -= cantRetirada;

  // Pregunta por nuevos jugadores
  
}

/*
void crearMazo(Carta *mazo){
  char * valores[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
  for (int i = 0 ; i < 13 ; i++) printf("%c" , valores[i]);
}*/

void crearJugador(Map * jugadores){
  Jugador * nuevo = (Jugador*)calloc(1, sizeof(Jugador));

  nuevo->nombre = (char*)malloc(15*sizeof(char));

  printf("Ingrese su nombre: \n > ");
  fflush(stdin);
  fgets(nuevo->nombre, 100, stdin);
  chomp(nuevo->nombre);
  nuevo->dinero = 10000;

  insertMap(jugadores, nuevo->nombre, nuevo);

  printf("\n");
}

bool menuPrincipal(){
  printf("**** BLACKJACK ****\n\n");
  
  printf("Empezar juego (1)\nSalir (2)\n > ");
  int respuesta;
  scanf("%d", &respuesta);

  if (respuesta != 1) return false;

  /* Creacion del mazo
  Carta * mazo = (Carta*)calloc(52,sizeof(Carta));
  crearMazo(mazo);*/

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
  Map * jugadores = createMap(is_equal_string);

  for (int i = 0 ; i < *cantJugadores ; i++){
    printf("Jugador %d: \n", i+1);
    crearJugador(jugadores);
  }
  printf("\n\n");

  // BLACKJACK!
  while(*cantJugadores != 0){
    blackjack(cantJugadores, jugadores);
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