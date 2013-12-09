#ifndef MALLOC_H
#define MALLOC_H

#include "stdbool.h"

#define malloc( x ) my_malloc( x, __FILE__ , __LINE__ )
#define free( x ) my_free( x, __FILE__, __LINE__ )

#define MEM_SIZE 5000

struct MemEntry {
    unsigned int size;
    unsigned int isfree;
    struct MemEntry *succ, *prev;
};

void * my_malloc( unsigned int size, char * file, int line );
void my_free( void * p, char * file, int line );

#endif