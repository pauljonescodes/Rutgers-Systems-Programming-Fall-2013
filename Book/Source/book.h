#ifndef BOOK_H
#define BOOK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "tokenizer.h"

#define debug printf("LINE = %d\n",__LINE__);
#define FAIL 1
#define SUCCESS 0

/* A pseudo-type to hold catagory values. */
typedef char* catagory_t;

/* Struct with information about a consumer */
typedef struct consumer_t_ {
	int 		id;		/* The numerical ID of this customer. */
	char* 		name;		/* The customer's name */
	char* 		address;	/* The customer's street address */
	char* 		state;		/* The customer's state */
	char* 		zip;		/* The customer's zip code */
	double 		credit;		/* The customer's current account balence ($) */
}consumer_t;

/* Struct with informatio about a book order */
typedef struct order_t_ {
	double		cost;		/* The cost of the book. */
	char*		title;		/* The book's title */
	int 		customerId;	/* The ID of the ordering customer */
	int	 	catagory;	/* The catagory listed for this book */
}order_t;

/* Function prototypes */
catagory_t* 	processCatagories(int*, char*);				/* processes the catagory input list into an array */
int 		processDatabase(FILE*, consumer_t*);			/* parses the database file into a list of consumers */
int 		processOrders(FILE*, order_t*, catagory_t*, size_t);	/* parses the order file into a list of orders to be executed */
int 		getFileLength(FILE*);					/* returns the length of the passed file */
void* 		cleanMalloc(size_t);					/* returns allocated memory of all 0's */
char*		trim(char*);						/* trims first and last characters */
char* 		clean(char*); 						/* trims leading and tailing whitespace */
int 		sArraySearch(char**, char*, size_t); 			/* searches in a string array, returns subscript */
void 		printData(consumer_t*, order_t*, catagory_t*, size_t);	/* prints out parsed data */

#endif
