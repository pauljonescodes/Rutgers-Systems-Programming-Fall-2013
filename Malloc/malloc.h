#ifndef MALLOC_H
#define MALLOC_H

#include "stdbool.h"

#define MEM_SIZE 5000

struct MemEntry {
    unsigned int size;
    unsigned int isfree;
    struct MemEntry *succ, *prev;
};

void * my_malloc( unsigned int size, char * file, int line );
void my_free( void * p, char * file, int line );

#endif