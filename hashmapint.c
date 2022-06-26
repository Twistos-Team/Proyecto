#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmapint.h"

typedef struct Pair Pair;
typedef struct HashMap HashMap;
int enlarge_called=0;

struct Pair {
     int * key;
     void * value;
};

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( int * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( int * key, long capacity) {
    return (*key)%capacity;
}

int is_equal(int* key1, int* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(*key1 == *key2) return 1;
    return 0;
}

void insertHashMap(HashMap * map, int * key, void * value) {
    unsigned long pos = hash(key, map->capacity);
    unsigned long aux = pos;
    struct Pair *n = createPair(key, value);
    while ( (map->buckets[pos] != NULL) && (!is_equal(map->buckets[pos]->key, key)) ) {
        pos++;
        if (pos == map->capacity) {
            pos = 0;
        }
        if (pos == aux) {
            break;
        }
    }
    map->buckets[pos] = n;
    map->size++;
    map->current = pos;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair **aux = (Pair **)malloc(map->capacity*sizeof(Pair *));
    unsigned long i;
    aux = map->buckets;
    map->capacity *= 2;
    map->buckets = (Pair **)malloc(map->capacity*sizeof(Pair *));
    map->size = 0;
    for (i = 0; i < map->capacity/2; i++) {
        if ((aux[i] != NULL) && (aux[i]->key != NULL)) {
            insertHashMap(map, aux[i]->key, aux[i]->value);
            map->size++;
        }
    }
}

HashMap * createHashMap(long capacity) {
    HashMap *map = (HashMap *) malloc(sizeof(HashMap));
    map->buckets = (Pair **) calloc(10, sizeof(Pair *));
    map->capacity = capacity;
    map->size = 0;
    map->current = -1;
    return map;
}

void eraseHashMap(HashMap * map,  int * key) {
    unsigned long pos = hash(key, map->capacity);
    unsigned long aux = pos;    
    while (map->buckets[pos] != NULL) {
        if (is_equal(map->buckets[pos]->key, key)) {
            map->buckets[pos]->key = NULL;
            map->size--;
            break;
        }
        pos++;
        if (pos == map->capacity) {
            pos = 0;
        }
        if (pos == aux) {
            break;
        }
    }
}

void * searchHashMap(HashMap * map,  int * key) {  
    Pair *n;
    unsigned long pos = hash(key, map->capacity);
    unsigned long aux = pos;
    n = map->buckets[pos];
    while (!is_equal(n->key, key)) {
        n = map->buckets[pos];
        pos++;
        if (pos == map->capacity) {
            pos = 0;
        }
        if (pos == aux) {
            break;
        }
        if (map->buckets[pos] == NULL) {
            return NULL;
        }
    }
    map->current = pos;
    return n->value;
}

void * firstHashMap(HashMap * map) {
    unsigned long i = 0;
    while (i < map->capacity) {
        if ((map->buckets[i] != NULL) && (map->buckets[i]->key != NULL)) {
            map->current = i;
            return map->buckets[i]->value;
        }
        i++;
    }
    return NULL;
}

void * nextHashMap(HashMap * map) {
    unsigned long i = map->current + 1;
    while (i < map->capacity) {
        if (i == map->capacity) {
            i = 0;
        }
        if ((map->buckets[i] != NULL) && (map->buckets[i]->key != NULL)) {
            map->current = i;
            return map->buckets[i]->value;
        }
        i++;
        if (i == map->current) {
            break;
        }
    }
    return NULL;
}