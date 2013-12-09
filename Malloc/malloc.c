#include <stdio.h>
#include "malloc.h"

static char memory_big_block[BLOCKSIZE];
static char memory_lil_block[BLOCKSIZE];
static bool initialized = false;

struct MemoryChunk {
    unsigned int size;
    bool is_free;
    struct MemoryChunk *prev, *succ;
};

void * mymalloc( unsigned int size )
{
    static struct MemoryChunk * memory_big_root;
    static struct MemoryChunk * memory_lil_root;
    struct        MemoryChunk * this;
    struct        MemoryChunk * succ;
    
    if (!initialized)
    {
        memory_big_root = (struct MemoryChunk*) memory_big_block;
        memory_big_root->size = BLOCKSIZE - sizeof(struct MemoryChunk);
        memory_big_root->is_free = true;
        
        memory_lil_root = (struct MemoryChunk*) memory_lil_block;
        memory_lil_root->size = BLOCKSIZE - sizeof(struct MemoryChunk);
        memory_lil_root->is_free = true;
        initialized = true;
    }
}

void myfree( void * p )
{
    
}
