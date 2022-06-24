//
//  HashMap.h
//  TablaHashLab
//
//  Created by Matias Barrientos on 11-08-18.
//  Copyright © 2018 Matias Barrientos. All rights reserved.
//

#ifndef HashMap_h
#define HashMap_h

typedef struct HashMap HashMap;

HashMap * createHashMap(long capacity);

void insertHashMap(HashMap * table, char * key, void * value);

void eraseHashMap(HashMap * table, char * key);

void * searchHashMap(HashMap * table, char * key);

void * firstHashMap(HashMap * table);

void * nextHashMap(HashMap * table);


void enlarge(HashMap * map);

#endif /* HashMap_h */
