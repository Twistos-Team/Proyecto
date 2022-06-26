//
//  HashMap.h
//

#ifndef HashMap_h
#define HashMap_h

typedef struct HashMap HashMap;

HashMap * createHashMap(long capacity);

void insertHashMap(HashMap * table, int * key, void * value);

void eraseHashMap(HashMap * table, int * key);

void * searchHashMap(HashMap * table, int * key);

void * firstHashMap(HashMap * table);

void * nextHashMap(HashMap * table);


void enlarge(HashMap * map);

#endif /* HashMap_h */
