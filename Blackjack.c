#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "Map.c"

#define GANASTE printf("Has Ganado!\n")
#define PERDISTE printf("Has Perdido!\n")
#define EMPATE printf("Has Empatado!\n")
#define OUCH printf("ouch\n")

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
  int cantCarta;
  bool bj;
  bool retirado;
  bool sePaso;
  int cantAss;
} Jugador;

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

void mostrarCarta(Carta card){
  printf("[");
  mostrarValor(card.valor);
  printf("%c] ", card.pinta);
}

void crearJugador(Map * jugadores){
  Jugador * nuevo = (Jugador*)calloc(1, sizeof(Jugador));

  nuevo->nombre = (char*)malloc(15*sizeof(char));
  int dinero;

  printf("Ingrese su nombre: \n > ");
  fflush(stdin);
  fgets(nuevo->nombre, 100, stdin);
  chomp(nuevo->nombre);
  
  printf("\nIngrese su dinero: \n > ");
  scanf("%d", &dinero);
  nuevo->dinero = dinero;

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

void barajarMazo(Carta * mazo, Carta *baraja, int *indice){
  int largo = 52;
  int idx;
  crearMazo(mazo);

  int cont = 0;
  while (cont < 52){
    idx = rand()%52;

    if (mazo[idx].valor != 0){
      baraja[cont] = mazo[idx];
      mazo[idx].valor = 0;
      cont++;
    }
  }

  *indice = 51;
}

int tieneAs(Jugador* j){
  int cont = 0;
  for (int i = 0 ; i < j->cantCarta ; i++)
    if (j->mano[i].valor == 1) cont++;
  return cont;
}

void repartirCarta(Jugador * jugador, Carta * baraja, int * idx){
  jugador->mano[jugador->cantCarta] = baraja[*idx];
  int valor = jugador->mano[jugador->cantCarta].valor;
  (jugador->cantCarta)++;


  switch(valor){
    case 1: 
      valor+= 10;
      break;
    case 11:
      valor--;
      break;
    case 12:
      valor -=2 ;
      break;
    case 13:
      valor -= 3;
      break;
  }
  
  jugador->suma += valor;

  if (jugador->suma > 21){

    int contAss = tieneAs(jugador);
    if (contAss != 0){
      if (jugador->cantAss < contAss){
      jugador->suma -= 10;
      (jugador->cantAss) ++;
      (*idx)--;
      return;
      }
    }

    jugador->sePaso = true;
  }
  (*idx)--; //Con el fin de que no se repitan las cartas
}

void mostrarMano(Jugador * Jugador){
  for (int i = 0 ; i < Jugador->cantCarta ; i++){
    mostrarCarta(Jugador->mano[i]);
  }
  printf("\n");
}

bool opcionJugador(Jugador * aux, Carta * baraja, int * idx){
  printf("Mano: ");
  mostrarMano(aux);

  if (aux->sePaso) return false;

  if (aux->suma == 21 && aux->cantCarta == 2){
    printf("Blackjack!\n");
    aux->bj = true;
    return false;
  }

  printf("Que desea hacer?\n");
  int resp;
  printf("Pedir carta(1)\nPlantarse(2)\nRendirse(3)\nDoblar Apuesta(4)\n > ");
  scanf("%d", &resp);
  printf("\n");

  switch(resp){
  case 1:
    if (aux->cantCarta < 6) repartirCarta(aux,baraja,idx);
    else printf("No puedes pedir mas cartas\n");
    if (aux->sePaso) printf("Te pasaste!\n");
  return true;

  case 2:
    printf("Te plantaste\n");
    return false;

  case 3:
    printf("Te rendiste\n");
    aux->retirado = true;
    return false;

  case 4:
    if (aux->cantCarta == 2){
      if (aux->suma >= 9 && aux->suma <= 11){
        if (aux->dinero > 2* aux->apuestaTurno){
          aux->apuestaTurno *= 2;
          repartirCarta(aux, baraja, idx);
          printf("Has doblado tu apuesta!\n");
          mostrarMano(aux);
          return false;
        }
      }
    }

    printf("\nNo puedes doblar apuesta\n");
    return true;

  default:
    printf("Numero no valido\n");
    return true;
  }
  return true;
}

void turnoCroupier(Jugador * croupier, Carta * baraja,int * idx){
  mostrarMano(croupier);
  while (true){
    if (croupier->suma <= 16){
      repartirCarta(croupier, baraja, idx);
      mostrarMano(croupier);
    }
    if (croupier->suma >= 17) break;
  }
  printf("\n");
}

void calcularApuestas(Map *jugadores, Jugador *croupier){
  Jugador * aux;
  aux = firstMap(jugadores);

  while (aux != NULL){
    printf("\n\nMano \"%s\"\n", aux->nombre);
    mostrarMano(aux);
    printf("\n");

    if (aux->retirado){
      int perdido;
      perdido = aux->apuestaTurno/2;
      aux->dinero = aux->dinero - perdido;
      printf("Te Retiraste\n");
      PERDISTE;
      printf("Dinero perdido >>>> %d\n", perdido);
      printf("Dinero Total >>>> %d", aux->dinero);
      aux = nextMap(jugadores);
      continue;
    }
    if (aux->sePaso){
      aux->dinero = aux->dinero - aux->apuestaTurno;
      printf("Te pasaste\n");
      PERDISTE;
      printf("Dinero perdido >>>> %d\n", aux->apuestaTurno);
      printf("Dinero Total >>>> %d\n", aux->dinero);
      aux = nextMap(jugadores);
      continue;
    }

    if (aux->bj){
      if (croupier->bj){
        printf("Dos Blackjacks!\n");
        EMPATE;
        printf("Dinero Total >>>> %d\n", aux->dinero);
        aux = nextMap(jugadores);
        continue;
      }
      else{
      int ganado;
      ganado = (aux->apuestaTurno)+(aux->apuestaTurno/2);
      aux->dinero = (aux->dinero+ganado);
      printf("Blackjack!\n");
      GANASTE;
      printf("Dinero Ganado >>>> %d\n", ganado);
      printf("Dinero Total >>>> %d\n", aux->dinero);
      aux = nextMap(jugadores);
      continue;
      }
    }
    else{
      if (croupier->bj){
        aux->dinero -= aux->apuestaTurno;
        printf("Blackjack del Croupier!\n");
        PERDISTE;
        printf("Dinero Perdido >>>> %d\n", aux->apuestaTurno);
        printf("Dinero Total >>>> %d\n", aux->dinero);
        aux = nextMap(jugadores);
        continue;
      }
      else{
        if (croupier->sePaso){
          aux->dinero += aux->apuestaTurno;
          printf("Se paso el croupier\n");
          GANASTE;
          printf("Dinero Ganado >>>> %d\n", aux->apuestaTurno);
          printf("Dinero Total >>>> %d\n", aux->dinero);
          aux = nextMap(jugadores);
          continue;
        }
      }
    }

    if (aux->suma == croupier->suma){
      EMPATE;
      printf("Dinero Total >>>> %d\n", aux->dinero);
    }
    else{
      if (aux->suma > croupier->suma){
        aux->dinero += aux->apuestaTurno;
        GANASTE;
        printf("Dinero Ganado >>>> %d\n", aux->apuestaTurno);
        printf("Dinero Total >>>> %d\n", aux->dinero);
      }
      else{
        aux->dinero -= aux->apuestaTurno;
        PERDISTE;
        printf("Dinero Perdido >>>> %d\n", aux->apuestaTurno);
        printf("Dinero Total >>>> %d\n", aux->dinero);
      }
    }

    aux = nextMap(jugadores);
    continue;
  }
  printf("\n\n");
}

void inicializarJugador(Jugador* jugador){
  jugador->bj = false;
  jugador->retirado = false;
  jugador->sePaso = false;
  jugador->suma = 0;
  jugador->cantCarta = 0;
  jugador->cantAss = 0;
}

void blackjack(int *cantJugadores, Map *jugadores, Carta * mazo, Carta * baraja, int * idx){
  printf("********************************************\n");
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
  barajarMazo(mazo, baraja, idx);

  // ********  APUESTAS  ********
  int apuesta;
  printf("********** COMIENZAN LAS APUESTAS **********\n\n");

  aux = firstMap(jugadores);
  while (aux != NULL){
    inicializarJugador(aux);
    printf("\"%s\"\nIngrese su apuesta: \n > ", aux->nombre);
    
    while(1){ 
      scanf("%d", &apuesta);
      if(aux->dinero >= apuesta && apuesta > 0){
        aux->apuestaTurno = apuesta;
        break;
      }else printf("*****Apuesta invalida*****\nIntentelo de nuevo\n > ");
    }

    aux = nextMap(jugadores);
    printf("\n");
  }
  printf("\n\n");


  // ********  CREAR CROUPIER  ********
  
  Jugador *croupier = (Jugador*)malloc(sizeof(Jugador));
  inicializarJugador(croupier);
  repartirCarta(croupier, baraja, idx);
  repartirCarta(croupier, baraja, idx);
  printf("Mano del Croupier\n");
  mostrarCarta(croupier->mano[0]);
  printf("[XX]\n\n");
  if (croupier->suma == 21 && croupier->cantCarta == 2) croupier->bj = true;


  // ********  REPARTICION CARTAS INICIALES  ********
  aux = firstMap(jugadores);
  while (aux!=NULL){
    aux->cantCarta = 0;
    repartirCarta(aux, baraja, idx);
    repartirCarta(aux, baraja, idx);
    aux = nextMap(jugadores);
  }


  // ********  TURNOS JUGADORES  ********
  printf("************ TURNO DE JUGADORES ************\n");
  aux = firstMap(jugadores);
  while (aux != NULL){

    printf("Turno de \"%s\"\n", aux->nombre);

    while(opcionJugador(aux, baraja, idx));
    printf("\n");
    aux = nextMap(jugadores);
  }


  // ********  RESULTADOS  ********
  printf("\n**************** RESULTADOS ****************\n\n");
  printf("Mano Croupier >>> \n");
  turnoCroupier(croupier, baraja, idx);

  calcularApuestas(jugadores, croupier);


  // ********  RETIRO DE JUGADORES  ********
  printf("\n************* DESEA RETIRARSE? *************\n");
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
  printf("************** QUIERE UNIRSE? **************\n");
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
  free(croupier);
  printf("\n");
}

bool menuPrincipal(){
  printf("********************************************\n");
  printf("**************** BLACKJACK! ****************\n\n");
  
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

  while(1){
    printf(" > ");
    scanf("%d", cantJugadores);
    printf("\n");
    
    if (0 < *cantJugadores && *cantJugadores <= 5) break;
    printf("Numero no valido\n");
  }
  printf("\n");


  // ********  CREACION MAPA DE JUGADORES  ********
  Map * jugadores = createMap(is_equal_string);

  for (int i = 0 ; i < *cantJugadores ; i++){
    printf("Jugador %d: \n", i+1);
    crearJugador(jugadores);
  }
  printf("\n\n");


  // ********  CREAR BARAJA DE CARTAS  ********
  Carta * baraja = (Carta*)malloc(52*sizeof(Carta));
  int * idx = (int*)malloc(sizeof(int));


  // ********  BLACKJACK  ********
  while(*cantJugadores != 0){
    blackjack(cantJugadores, jugadores, mazo, baraja, idx);
  }

  return true;
}