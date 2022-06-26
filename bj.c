#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "Map.c"

int valores[13] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
char pintas[4] = {'C', 'D', 'T', 'P'};

typedef struct{
  int valor;
  char pinta;
} Carta;

typedef struct{
  char * nombre;
  int dinero;
  int suma;
  int apuestaTurno;
  Carta mano[6];
} Jugador;

int lower_than_int(void* key1, void* key2){
    int* k1=(int*) key1;
    int* k2=(int*) key2;
    if(*k1<*k2) return 1;
    return 0;
}

int is_equal_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

void chomp(char *s) {
    while(*s && *s != '\n' && *s != '\r') s++;
    *s = 0;
}

void mostrarValor(int valor){
  switch (valor){
    case 1: printf("A");
            break;
    case 11:printf("J");
            break;
    case 12:printf("Q");
            break;
    case 13:printf("K");
            break;
    default:printf("%d", valor);
            break; 
  }
}

void mostrarCarta(Carta * card){
  printf("[");
  mostrarValor(card->valor);
  printf("%c] ", card->pinta);
}

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

void crearMazo(Carta *mazo){
  int cont = 0;
  for (int i = 0 ; i < 4 ; i++){
    for (int j = 0 ; j < 13 ; j++){
      mazo[cont].valor = valores[j];
      mazo[cont].pinta = pintas[i];
      cont++;
    }
  }
}

void barajarMazo(Carta * mazo, Map * baraja, int * contCarta){
  *contCarta = 0;
  for (int i = 0 ; i < 52 ; i++){
    Carta * nuevo = (Carta*)calloc(1, sizeof(Carta));
    nuevo->pinta = mazo[i].pinta;
    nuevo->valor = mazo[i].valor;

    int * key = (int*)malloc(sizeof(int));
    *key = rand();

    insertMap(baraja, key, nuevo);
  }
}

void blackjack(int *cantJugadores, Map *jugadores, Carta * mazo, Map * baraja, int * contCarta){
  printf("******\n");
  printf("Empieza el juego!\n\n");
  Jugador * aux;
  Carta * auxCarta;

  // ********  HAY DINERO?  ********
  int cantRetirada = 0;
  aux = firstMap(jugadores);

  while (aux != NULL){
    if (aux->dinero <= 0){
      printf("\"%s\": Te quedaste sin dinero!\n\n", aux->nombre);
      cantRetirada++;
      eraseMap(jugadores, aux->nombre);
    }
    aux = nextMap(jugadores);
  }
  (*cantJugadores) -= cantRetirada;
  if(*cantJugadores == 0){
    printf("No quedan jugadores!\n");
    return;
  }

  // ********  BARAJAR EL MAZO  ********
  barajarMazo(mazo, baraja, contCarta);

  auxCarta = firstMap(baraja);
  if (auxCarta == NULL) printf("QUe loco\n");
  while(auxCarta != NULL){
    mostrarCarta(auxCarta);
    printf("\n");
    auxCarta = nextMap(baraja);
  }
  printf("\n");

  

  // ********  APUESTAS  ********
  printf("** Empiezan las apuestas! **\n\n");

  aux = firstMap(jugadores);
  while (aux != NULL){
    printf("\"%s\"\nIngrese su apuesta: \n > ", aux->nombre);
    // Aqui cada jugador apuesta
    // Se comprueba si tiene el dinero suficiente
    aux = nextMap(jugadores);
    printf("\n");
  }
  printf("\n\n");


  // ********  REPARTICION CARTAS INICIALES  ********


  // ********  TURNOS JUGADORES  ********
  aux = firstMap(jugadores);
  while (aux != NULL){
    printf("Turno de \"%s\"\n", aux->nombre);
    // Se muestra la mano del jugador

    printf("Que desea hacer?\n");
    //opcionesJugador(); Funcion que muestra las opciones
    printf("\n");
    aux = nextMap(jugadores);
  }
  printf("\n\n");


  // ********  RESULTADOS  ********


  // ********  RETIRO DE JUGADORES  ********
  int respuesta;
  cantRetirada = 0;

  aux = firstMap(jugadores);
  while (aux != NULL){
    printf("\"%s\"\nDesea retirarse?\nSi(1)\nNo(2)\n > ", aux->nombre);
    scanf("%d", &respuesta);
    if (respuesta == 1){
      cantRetirada++;
      eraseMap(jugadores, aux->nombre);
    }
    aux = nextMap(jugadores);
    printf("\n\n");
  }
  (*cantJugadores) -= cantRetirada;


  // ********  AÑADIR JUGADORES  ********
  int capacidad = 5 - *cantJugadores;
  if (capacidad != 0){
    printf("Desea unirse otro jugador?\n");

    if (capacidad == 1) printf("Solo puede unirse 1 mas\n");
    else printf("Solo pueden unirse %d mas\n", capacidad);

    int respuesta;
    while(true){
      printf("Cuantos quieren unirse?\n > ");
      scanf("%d", &respuesta);
      printf("\n");
      if (respuesta > capacidad){
        printf("Numero no valido\n\n");
        continue;
      }
      else{
        for (int i = 0 ; i < respuesta ; i++){
          printf("Jugador %d: \n", i+1);
          crearJugador(jugadores);
          (*cantJugadores)++;
        }
        break;
      }
    }

  }
  printf("******\n\n");
}

bool menuPrincipal(){
  printf("**** BLACKJACK ****\n\n");
  
  printf("Empezar juego (1)\nSalir (2)\n > ");
  int respuesta;
  scanf("%d", &respuesta);

  if (respuesta != 1) return false;

  // ********  PREPARACION JUEGO  ********

  // ********  CREACION DEL MAZO ORIGINAL  ********
  Carta * mazo = (Carta*)calloc(52,sizeof(Carta));
  crearMazo(mazo);


  // ********  CANTIDAD DE JUGADORES  ********
  printf("\nCuantos jugadores son? (Maximo 5)\n");
  int * cantJugadores = (int*)calloc(1, sizeof(int));

  while(true){
    printf(" > ");
    scanf("%d", cantJugadores);
    printf("\n");
    
    if (0 < *cantJugadores && *cantJugadores <= 5) break;
    printf("Número no valido\n");
  }
  printf("\n");


  // ********  CREACION MAPA DE JUGADORES  ********
  Map * jugadores = createMap(is_equal_string);

  for (int i = 0 ; i < *cantJugadores ; i++){
    printf("Jugador %d: \n", i+1);
    crearJugador(jugadores);
  }
  printf("\n\n");


  // ********  CREACION MAPA DE BARAJA  ********
  Map * baraja = createMap(lower_than_int);
  int * contCarta;


  // ********  BLACKJACK  ********
  while(*cantJugadores != 0){
    blackjack(cantJugadores, jugadores, mazo, baraja, contCarta);
  }

  return true;
}

int main(){
  srand(time(NULL));
  while(menuPrincipal()){}

  printf("\n\nGracias por jugar ^-^\n > ");
  fflush(stdin);
  getchar();
    return 0;
}