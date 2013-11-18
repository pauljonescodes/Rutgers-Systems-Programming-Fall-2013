#include "book.h"

#define SHM_SIZE 1000

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
		fprintf(stderr,"\e[31mERROR: Cannot tokenize catagory list.\e[0m\n");
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
			fprintf(stderr,"\e[31mERROR: Invalid line number %d in database file.\e[0m\n", i);
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
			fprintf(stderr,"\e[31mERROR: Invalid line number %d in order file.\e[0m\n", i);
			fprintf(stderr,"LINE = %s\n",lines[i]);
			return FAIL; 
		}
		orderData[i].title 	= trim(clean(items[0]));
		orderData[i].cost   	= atof(clean(items[1]));
		orderData[i].customerId	= atoi(clean(items[2]));
		orderData[i].catagory 	= sArraySearch(catList, clean(items[3]), numCat);
		if(orderData[i].catagory == -1) {
			fprintf(stderr,"\e[31mERROR: Invalid catagory \'%s\' on line %d of order file.\e[0m\n", items[3], i);
			return FAIL;
		}
	}
	return SUCCESS;

}

shmdata_t* setup_shm(consumer_t* consumerData, order_t* orderData) {
	key_t 		SHM_KEY;	
	int 		SHMID;
	int 		KEY_ID;
	int		NUMCONSUMER;
	int 		NUMORDER;
	shmdata_t*	shmdata;
	
	NUMCONSUMER 	= sizeof(consumerData)/sizeof(consumer_t);
	NUMORDER 	= sizeof(orderData)/sizeof(order_t);
	
	shmdata = cleanMalloc(sizeof(shmdata_t));
	
	shmdata->numConsumers = NUMCONSUMER;
	shmdata->POS_START = NUMCONSUMER;	
	shmdata->POS_doneFlag = NUMCONSUMER+1;
	shmdata->POS_transmitCode = NUMCONSUMER * 2 + 1;
	shmdata->POS_transmitData = NUMCONSUMER * 2 + 2;
	shmdata->POS_queue = shmdata->POS_transmitData + 400;

	SHMID = -1;
	while(SHMID == -1) {
		SHM_KEY = -1;
		for(KEY_ID = 0; SHM_KEY == -1; KEY_ID++) {
			SHM_KEY = ftok(".shm", KEY_ID);
		}
		
		SHMID = shmget(SHM_KEY, SHM_SIZE, 0700 | IPC_CREAT | IPC_EXCL);
	}
	
	shmdata->SHMID = SHMID;
	return shmdata;	
}

void produce(shmdata_t* shmdata, consumer_t* consumerData, order_t* orderData) {
	void*		SHM;
	int		i;
	int 		NUMORDERS;
	int 		NUMCONSUMERS;
	
	printf("<ProducerBot> Producer has risen!\n");
	
	SHM = shmat(shmdata->SHMID, NULL, 0);
	
	NUMCONSUMERS = sizeof(consumerData)/sizeof(consumer_t);
	NUMORDERS = sizeof(orderData)/sizeof(order_t);

	printf("<ProducerBot> Ready for duty, sir.\n");
	
	/* wait for overseer to send the start signal */
	while(*((char*)SHM + shmdata->POS_START) == 0) { }
	
	printf("<ProducerBot> Beginning production.\n");
	
	for(i=0;i<NUMORDERS;i++) {
		/* add to queue */
	}
	
	printf("<ProducerBot> All done.\n");
	/* wait for the consumers to finish */
	while(1) {
		int	j;	
		int 	done = 1;
		for(j=shmdata->POS_doneFlag;j<shmdata->POS_transmitCode;j++) {
			if(*((char*)SHM + j) == 0) {
				done = 0;
			}
		}
		if(done) {
			break;
		}
	}
	
	shmdt(SHM);
	_exit(0);
}

void consume(shmdata_t* shmdata, consumer_t* consumerData, order_t* orderData, int ID) {
	_exit(0);
}

void spawnChildren(shmdata_t* shmdata, consumer_t* consumerData, order_t* orderData) {
	pid_t		PID_producer;
	pid_t* 		PID_consumers;
	int		i;
	int		NUMCONSUMERS;
	
	NUMCONSUMERS = sizeof(consumerData)/sizeof(consumer_t);
	PID_consumers = cleanMalloc(NUMCONSUMERS * sizeof(pid_t));
	
	/* first spawn the producer */
	PID_producer = fork();
	if(PID_producer == -1) {
		printf("\e[31m<OVERSEER> Error in forking of producer child process.\e[0m\n");
	}
	if(PID_producer == 0) {
		/* we are now in the child process! */
		printf("Hi mom!\n");
		produce(shmdata,consumerData,orderData);
	}else{
		printf("<OVERSEER> Spawned child process \"ProducerBot\" with id %d.\n", PID_producer);
	}
	
	/* now spawn all child processes. */
	for(i=0;i<NUMCONSUMERS;i++) {
		PID_consumers[i] = fork();
		if(PID_consumers[i] == -1 ) {
			printf("\e[31m<OVERSEER> Error in forking of consumer process \"%s\" (%d).\e[0m\n", consumerData[i].name, i);
		}
		if(PID_consumers[i] == 0) {
			/* child process for this consumer */
			printf("Hi mom!\n");
			consume(shmdata,consumerData,orderData,i);
		}else{
			printf("<OVERSEER> Spawned child process \"%s\" with id %d.\n", consumerData[i].name, PID_consumers[i]);
		}
		
	}
	
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
	shmdata_t* 	shmdata;	/* Shared Memory data */
	void* 		SHMCHUNK;	/* main function's pointer to the shared memory chunk */
	
	numCat = 0;
	if(argc != 4) {
		fprintf(stderr,"\e[31mERROR: Invalid number of arguments.\n\tPlease see readme for usage information.\e[0m\n");
		return FAIL;
	}
	
	DATABASE = fopen(argv[1], "r+");
	if(DATABASE == 0) {
		fprintf(stderr,"\e[31mERROR: Database input file \'%s\' does not exist.\e[0m\n", argv[1]);
		return FAIL;
	}
	ORDER = fopen(argv[2], "r+");
	if(ORDER == 0) {
		fprintf(stderr,"\e[31mERROR: Book order file \'%s\' does not exist.\e[0m\n", argv[2]);
		return FAIL;
	}
	
	catList = processCatagories(&numCat, argv[3]);
	if(numCat == 0) {
		fprintf(stderr,"\e[31mERROR: Invalid list of catagories.\e[0m\n");
		return FAIL;
	}
	if(processDatabase(DATABASE, consumerData)) 		{ return FAIL; }
	if(processOrders(ORDER, orderData, catList, numCat)) 	{ return FAIL; }
	
	printData(consumerData, orderData, catList, numCat);
		
	shmdata = setup_shm(consumerData, orderData);
	
	
	SHMCHUNK = shmat(shmdata->SHMID, NULL, 0);
	
	printf("<OVERSEER> Arise, my minions!\n");
	spawnChildren(shmdata, consumerData, orderData);
	printf("<OVERSEER> All initialization completed successfully.\n");
	printf("<OVERSEER> Starting processes...\n");
	
	/* THE ALL IMPORTANT HOLY GOD LINE */
	*((char*)(SHMCHUNK + shmdata->POS_START)) = 1;
	/* END ALL IMPORTANT HOLD GOD LINE */
	
	return SUCCESS;
}
