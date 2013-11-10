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
int 		processCatagories(catagory_t*, char*);			/* processes the catagory input list into an array */
int 		processDatabase(FILE*, consumer_t*);			/* parses the database file into a list of consumers */
int 		processOrders(FILE*, order_t*, catagory_t*, size_t);	/* parses the order file into a list of orders to be executed */
int 		getFileLength(FILE*);					/* returns the length of the passed file */
void* 		cleanMalloc(size_t);					/* returns allocated memory of all 0's */
char*		trim(char*);						/* trims first and last characters */
char* 		clean(char*); 						/* trims leading and tailing whitespace */
int 		sArraySearch(char**, char*, size_t); 			/* searches in a string array, returns subscript */
void 		printData(consumer_t*, order_t*, catagory_t*, size_t);	/* prints out parsed data */

void printData(consumer_t* consumerData, order_t* orderData, catagory_t* catList, size_t numCat) {
	int 		i;		/* iterator */
		
	printf("-PARSED DATA-\nConsumers:\n");
	for(i=0;consumerData[i].name != 0;i++) {
		printf("Consumer %d: Name = \'%s\' Address = \'%s\' State = \'%s\' Zip = \'%s\' Money = %f\n", consumerData[i].id, consumerData[i].name, consumerData[i].address, consumerData[i].state, consumerData[i].zip, consumerData[i].credit);
	}
	printf("\nOrders:\n");
	for(i=0;orderData[i].title != 0;i++) {
		printf("Order by %d: Title = \'%s\' Cost = %f Catagory = \'%s\'\n", orderData[i].customerId, orderData[i].title, orderData[i].cost, orderData[i].catagory);
	}
	printf("\nCatagories:\n");
	for(i=0;i<numCat;i++) {
		printf("%s\n",catList[i]);
	}
	return;
}

int sArraySearch(char** a, char* s, size_t size) {
	int 		i;		/* iterator */
	
	for(i=0;i<size;i++) {
		if(strcmp(a[i], s) == 0) {
			return i;
		}
	}
	return -1;
}

char* trim(char* s) {
	s++;
	s[strlen(s) - 1] = 0;
	return s;
}

char* clean(char* s) {
 	while(s[0] == ' ') {
		s++;
	}
	while(s[strlen(s) - 1] == ' ') {
		s[strlen(s) - 1] = 0;
	}
	return s;
}

void* cleanMalloc(size_t size) {
	void* 		chunk;		/* requested chunk of memory */
	
	chunk = malloc(size);
	memset(chunk,0,size);
	return chunk;
}

int getFileLength(FILE* f) {
	int 		POS;		/* The saved current position */
	int 		LEN;		/* The length of the file */
	
	POS = ftell(f);
	fseek(f, 0, SEEK_END);
	LEN = ftell(f);
	fseek(f, POS, SEEK_SET);
	return LEN;
}

int processCatagories(catagory_t* catList, char* s) {
	TokenizerT* 	tk;		/* Tokenizer used to split the input string */
	
	tk = TKCreate(s, 0, NULL);
	if(tk == 0) { 
		fprintf(stderr,"ERROR: Cannot tokenize catagory list.\n");
		return 0; 
	}
	catList = (catagory_t*) tk->tokens;
	return tk->num_tok;
}

int processDatabase(FILE* DATABASE, consumer_t* consumerData) {
	char* 		file_string;	/* string containing all the contents of the database */
	int 		database_size;	/* size (number of characters) of the database */
	TokenizerT* 	tk;		/* tokenizer for the database */
	char** 		lines;		/* list of lines in the database */
	char** 		items;		/* reused array for each line; the properties of each consumer */
	int 		numlines;	/* number of lines */
	int 		i;		/* iterator */
	
	database_size = getFileLength(DATABASE);
	file_string = cleanMalloc(database_size + 1);
	fread(file_string, 1, database_size, DATABASE);
	fclose(DATABASE);
	
	tk = TKCreate(file_string, 1, "\n");
	lines = tk->tokens;
	numlines = tk->num_tok;
	consumerData = cleanMalloc((numlines + 1) * sizeof(consumer_t));
	
	for(i=0;i<numlines;i++) {
		tk = TKCreate(lines[i], 1, "|");
		items = tk->tokens;
		if(tk->num_tok != 6) { 
			fprintf(stderr,"ERROR: Invalid line number %d in database file.\n", i);
			return FAIL; 
		}
		consumerData[i].name 	= trim(clean(items[0]));
		consumerData[i].id   	= atoi(clean(items[1]));
		consumerData[i].credit 	= atof(clean(items[2]));
		consumerData[i].address = trim(clean(items[3]));
		consumerData[i].state 	= trim(clean(items[4]));
		consumerData[i].zip	= trim(clean(items[5]));
	}

	return SUCCESS;
}

int processOrders(FILE* ORDER, order_t* orderData, catagory_t* catList, size_t numCat) {
	char* 		file_string;	/* string containing all the contents of the database */
	int 		order_size;	/* size (number of characters) of the database */
	TokenizerT* 	tk;		/* tokenizer for the database */
	char** 		lines;		/* list of lines in the database */
	char** 		items;		/* reused array for each line; the properties of each consumer */
	int 		numlines;	/* number of lines */
	int 		i;		/* iterator */
	
	order_size = getFileLength(ORDER);
	file_string = cleanMalloc(order_size + 1);
	fread(file_string, 1, order_size, ORDER);
	fclose(ORDER);
	
	tk = TKCreate(file_string, 1, "\n");
	lines = tk->tokens;
	numlines = tk->num_tok;
	orderData = cleanMalloc((numlines + 1) * sizeof(order_t));
	
	for(i=0;i<numlines;i++) {
		tk = TKCreate(lines[i], 1, "|");
		items = tk->tokens;
		if(tk->num_tok != 6) { 
			fprintf(stderr,"ERROR: Invalid line number %d in order file.\n", i);
			return FAIL; 
		}
		orderData[i].title 	= trim(clean(items[0]));
		orderData[i].cost   	= atof(clean(items[1]));
		orderData[i].customerId	= atoi(clean(items[2]));
		orderData[i].catagory 	= sArraySearch(catList, clean(items[3]), numCat);
		if(orderData[i].catagory == -1) {
			fprintf(stderr,"ERROR: Invalid catagory \'%s\' on line %d of order file.\n", clean(items[3]), i);
			return FAIL;
		}
	}
	return SUCCESS;

}

int main(int argc, char** argv) {
	/* Input handing / error checking. */
	FILE* 		DATABASE;	/* File pointer to the customer database */
	FILE* 		ORDER;		/* File pointer to the order database */
	consumer_t* 	consumerData;	/* Data held about consumers, parsed from the database file */
	order_t*	orderData; 	/* List of all book orders, parsed from the order list file */
	catagory_t* 	catList;	/* List of catagories. */
	int 		numCat;		/* number of catagories */
		
	if(argc != 4) {
		fprintf(stderr,"ERROR: Invalid number of arguments.\n\tPlease see readme for usage information.\n");
		return FAIL;
	}
	
	DATABASE = fopen(argv[1], "r+");
	if(DATABASE == 0) {
		fprintf(stderr,"ERROR: Database input file \'%s\' does not exist.\n", argv[1]);
		return FAIL;
	}
	ORDER = fopen(argv[2], "r+");
	if(ORDER == 0) {
		fprintf(stderr,"ERROR: Book order file \'%s\' does not exist.\n", argv[2]);
		return FAIL;
	}
	
	numCat = processCatagories(catList, argv[3]);
	if(numCat == 0) {
		fprintf(stderr,"ERROR: Invalid list of catagories.\n");
		return FAIL;
	}
	if(processDatabase(DATABASE, consumerData)) 		{ return FAIL; }
	if(processOrders(ORDER, orderData, catList, numCat)) 	{ return FAIL; }
	
	printData(consumerData, orderData, catList, numCat);
	return SUCCESS;
}
