#include "book.h"

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
	
	printf("searching for :%s:\n",s);
	for(i=0;i<size;i++) {
		if(strcmp(a[i], s) == 0) {
			printf("found %d\n",i);
			return i;
		}
	}
	printf("not found\n");

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

catagory_t* processCatagories(int* numCat, char* s) {
	TokenizerT* 	tk;		/* Tokenizer used to split the input string */
	
	tk = TKCreate(s, 0, NULL);
	if(tk == 0) { 
		fprintf(stderr,"ERROR: Cannot tokenize catagory list.\n");
		return 0; 
	}
	
	*numCat = tk->num_tok;
	return (catagory_t*) tk->tokens;
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
		if(tk->num_tok != 4) { 
			fprintf(stderr,"ERROR: Invalid line number %d in order file.\n", i);
			fprintf(stderr,"LINE = %s\n",lines[i]);
			return FAIL; 
		}
		orderData[i].title 	= trim(clean(items[0]));
		orderData[i].cost   	= atof(clean(items[1]));
		orderData[i].customerId	= atoi(clean(items[2]));
		orderData[i].catagory 	= sArraySearch(catList, clean(items[3]), numCat);
		if(orderData[i].catagory == -1) {
			fprintf(stderr,"ERROR: Invalid catagory \'%s\' on line %d of order file.\n", items[3], i);
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
	int 		i;
		
	numCat = 0;
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
	
	catList = processCatagories(&numCat, argv[3]);
	if(numCat == 0) {
		fprintf(stderr,"ERROR: Invalid list of catagories.\n");
		return FAIL;
	}
	if(processDatabase(DATABASE, consumerData)) 		{ return FAIL; }
	if(processOrders(ORDER, orderData, catList, numCat)) 	{ return FAIL; }
	
	printData(consumerData, orderData, catList, numCat);
	return SUCCESS;
}
