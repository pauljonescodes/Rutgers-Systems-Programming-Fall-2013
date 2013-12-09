/*
 *  testprog.c
 *
 *  testprog
 *
 *  Created by Sujish Patel on 12/8/13.
 *  Copyright (c) 2013 Sujish Patel. All rights reserved.
 */

//#include	"malloc.h"
#include	<unistd.h>
#include	<stdio.h>
#include	<string.h>
#include	<errno.h>

#define MEM_SIZE 5000

static char block[MEM_SIZE];

// Full-scale malloc() implementation using sbrk().

struct MemEntry {
    unsigned int size;
    unsigned int isfree;
    struct MemEntry *succ, *prev;
};

void *
my_malloc( unsigned int size )
{
	static struct MemEntry *	root = 0;
	static struct MemEntry *	last = 0;
	struct MemEntry *		p;
	struct MemEntry *		succ;
    
	p = root;
	while ( p != 0 )
	{
		if ( p->size < size )
		{
			p = p->succ;					// too small
		}
		else if ( !p->isfree )
		{
			p = p->succ;					// in use
		}
		else if ( p->size < (size + sizeof(struct MemEntry)) )
		{
			p->isfree = 0;					// too small to chop up
			return (char *)p + sizeof(struct MemEntry);
		}
		else
		{
			succ = (struct MemEntry *)((char *)p + sizeof(struct MemEntry) + size);
			succ->prev = p;
			succ->succ = p->succ;
			//p->succ->prev = succ;
			//begin add
			if(p->succ != 0)
				p->succ->prev = succ;
			//end add
			p->succ = succ;
			succ->size = p->size - sizeof(struct MemEntry) - size;
			succ->isfree = 1;
			p->size = size;
			p->isfree = 0;
			last = (p == last) ? succ : last;
			return (char *)p + sizeof(struct MemEntry);
		}
	}
	if ( (p = (struct MemEntry *)sbrk( sizeof(struct MemEntry) + size )) == (void *)-1 )
	{
		return 0;
	}
	else if ( last == 0 )				// first block created
	{
		printf( "BKR making first chunk size %d\n", size );
		p->prev = 0;
		p->succ = 0;
		p->size = size;
		p->isfree = 0;
		root = last = p;
		return (char *)p + sizeof(struct MemEntry);
	}
	else						// other blocks appended
	{
		printf( "BKR making another chunk size %d\n", size );
		p->prev = last;
		p->succ = last->succ;
		p->size = size;
		p->isfree = 0;
		last->succ = p;
		last = p;
		return (char *)p + sizeof(struct MemEntry);
	}
	return 0;
}

void
my_free( void * p )
{
	struct MemEntry *		ptr;
	struct MemEntry *		pred;
	struct MemEntry *		succ;
    
    if((struct MemEntry*) p == NULL) {
        printf("This memory was never malloced\n");
        return;
    }
    
	ptr = (struct MemEntry *)((char *)p - sizeof(struct MemEntry));
	if ( (pred = ptr->prev) != 0 && pred->isfree )
	{
		pred->size += sizeof(struct MemEntry) + ptr->size;	// merge with predecessor
		
		pred->succ = ptr->succ;
		//begin added
		ptr->isfree=1;
		pred->succ = ptr->succ;
		if(ptr->succ != 0)
			ptr->succ->prev = pred;
		//end added
		printf( "BKR freeing block %#x merging with predecessor new size is %d.\n", p, pred->size );
	}
	else
	{
		printf( "BKR freeing block %#x.\n", p );
		ptr->isfree = 1;
		pred = ptr;
	}
	if ( (succ = ptr->succ) != 0 && succ->isfree )
	{
		pred->size += sizeof(struct MemEntry) + succ->size;	// merge with successor
		pred->succ = succ->succ;
		//begin added
		pred->isfree = 1;
        
		if(succ->succ != 0)
			succ->succ->prev=pred;
		//end added
		printf( "BKR freeing block %#x merging with successor new size is %d.\n", p, pred->size );
	}
}

int main() {
    
    char *p;
     int x = 0;
     int *xp;
     
     p = (char*)my_malloc (50);
     p[0] = 'c';
     p[1] = '\0';
     
     xp = (int*)my_malloc(50);
     
     printf("%s\n", p);
     my_free(x);
     my_free(xp);
     my_free(p);
     p = (char*)my_malloc (100);
     my_free(p);
     my_free(p);
     p = (char*)my_malloc (50);
     my_free(p);
    
    return 0;
}