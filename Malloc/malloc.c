#ifndef MALLOC_H
#define MALLOC_H

#define BLOCKSIZE 5000
#define CHUNKTHRESHOLD 200

#define malloc( x ) mymalloc( x, __FILE__ , __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )

void * mymalloc( unsigned int size );
void myfree( void * p );

#endif