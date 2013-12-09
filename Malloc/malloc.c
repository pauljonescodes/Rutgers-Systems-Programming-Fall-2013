#include <stdio.h>
#include "malloc.h"

static char memory_big_block[BLOCKSIZE];
static char memory_lil_block[BLOCKSIZE];
static bool initialized = false;

struct MemoryChunk {
    unsigned int size;
    bool is_free;
    struct MemoryChunk *prev, *next;
};

void * mymalloc( unsigned int requested_size )
{
    if (requested_size > BLOCKSIZE || requested_size == 0)
        return;
    
    static struct MemoryChunk * memory_big_root;
    static struct MemoryChunk * memory_lil_root;
    struct        MemoryChunk * current_mem_chk;
    struct        MemoryChunk * succ;
    
    if (!initialized) {
        memory_big_root          = (struct MemoryChunk*) memory_big_block;
        memory_lil_root          = (struct MemoryChunk*) memory_lil_block;
        
        memory_big_root->next    = memory_lil_root->next    = 0;
        memory_big_root->prev    = memory_lil_root->next    = 0;
        memory_big_root->size    = memory_lil_root->size    = BLOCKSIZE - sizeof(struct MemoryChunk);
        memory_big_root->is_free = memory_lil_root->is_free = true;
        
        initialized              = true;
    }
    
    if (size > TINYBLOCK) {
        current_mem_chk = memory_big_block;
    } else {
        current_mem_chk = memory_lil_block;
    }
    
    do {
        if (current_mem_chk->size < requested_size) {
            // this chunk is too small, so move on
            
            current_mem_chk = current_mem_chk->next;
        } else if (!current_mem_chk->is_free) {
            // this chunk was taken, so move on
            
            current_mem_chk = current_mem_chk->next;
        } else if(current_mem_chk->size < (requested_size + sizeof(struct MemoryChunk))) {
            // this chunk can hold, but there can be no more chunks
            
        } else {
            //  this chunk can hold, and there can be more chunk
            
            current_mem_chk = current_mem_chk->next;
        }
    } while(current_mem_chk != 0);
}

void myfree( void * p )
{
    /* free()ing pointers that were never allocated. */
    
    
    
    /* free()ing pointers to dynamic memory that were not returned from malloc(). */
    
    
    
    /* redundant free()ing of the same pointer. */
    
    
}
