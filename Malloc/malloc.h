#ifndef MALLOC_H
#define MALLOC_H

#include "stdbool.h"

struct MemEntry {
    unsigned int size;
    unsigned int isfree;
    struct MemEntry *succ, *prev;
};

void * my_malloc( unsigned int size, char * file, int line );
void my_free( void * p, char * file, int line );

#endif