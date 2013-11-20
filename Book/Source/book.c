#include "book.h"

void printData(consumer_t* consumerData, order_t* orderData, catagory_t* catList, size_t numCat) {
	int 		i;		/* iterator */
		
	printf("-PARSED DATA-\nConsumers:\n");
	for(i=0;consumerData[i].name != 0;i++) {
		printf("Consumer %d: Name = \'\e[36m%s\e[0m\' Address = \'%s\' State = \'%s\' Zip = \'%s\' Money = $ %.2f\n", consumerData[i].id, consumerData[i].name, consumerData[i].address, consumerData[i].state, consumerData[i].zip, consumerData[i].credit);
	}
	printf("\nOrders:\n");
	for(i=0;orderData[i].title != 0;i++) {
		printf("Order by %d: Title = \'%s\' Cost = $ %.2f Catagory = \'%s\'\n", orderData[i].customerId, orderData[i].title, orderData[i].cost, orderData[i].catagory);
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
	s[strlen(s)] = 0;
	return s;
}

char* clean(char* s) {
 	while(s[0] == ' ') {
		s++;
	}
	while(!isAlphanumeric(s[strlen(s) - 1])) {
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

consumer_t* processDatabase(FILE* DATABASE, int* NC) {
	char* 		file_string;	/* string containing all the contents of the database */
	int 		database_size;	/* size (number of characters) of the database */
	TokenizerT* 	tk;		/* tokenizer for the database */
	char** 		lines;		/* list of lines in the database */
	char** 		items;		/* reused array for each line; the properties of each consumer */
	int 		numlines;	/* number of lines */
	int 		i;		/* iterator */
	consumer_t* 	consumerData;
	
	database_size = getFileLength(DATABASE);
	file_string = cleanMalloc(database_size + 1);
	fread(file_string, 1, database_size, DATABASE);
	fclose(DATABASE);
	
	tk = TKCreate(file_string, 1, "\n");
	lines = tk->tokens;
	numlines = tk->num_tok;
	*NC = numlines;
	consumerData = cleanMalloc((numlines + 1) * sizeof(consumer_t));
	
	for(i=0;i<numlines;i++) {
		tk = TKCreate(lines[i], 1, "|");
		items = tk->tokens;
		if(tk->num_tok != 6) { 
			fprintf(stderr,"\e[31mERROR: Invalid line number %d in database file.\e[0m\n", i);
			return 0; 
		}
		consumerData[i].name 	= trim(clean(items[0]));
		consumerData[i].id   	= atoi(clean(items[1]));
		consumerData[i].credit 	= atof(clean(items[2]));
		consumerData[i].address = trim(clean(items[3]));
		consumerData[i].state 	= trim(clean(items[4]));
		consumerData[i].zip	= trim(clean(items[5]));
	}
	
	return consumerData;
}

void dump_string(char* s) {
	int 		i;

	for(i=0;s[i] != 0;i++) {
		if(isAlphanumeric(s[i])) {
			printf("%c",s[i]);
		}else{
			printf("[%d]",s[i]);
		}
	}
	printf("\n");
	return;
}

order_t* processOrders(FILE* ORDER, catagory_t* catList, size_t numCat, int* NO) {
	char* 		file_string;	/* string containing all the contents of the database */
	int 		order_size;	/* size (number of characters) of the database */
	TokenizerT* 	tk;		/* tokenizer for the database */
	char** 		lines;		/* list of lines in the database */
	char** 		items;		/* reused array for each line; the properties of each consumer */
	int 		numlines;	/* number of lines */
	int 		i;		/* iterator */
	order_t*	orderData;
	
	order_size = getFileLength(ORDER);
	file_string = cleanMalloc(order_size + 1);
	fread(file_string, 1, order_size, ORDER);
	fclose(ORDER);
	
	tk = TKCreate(file_string, 1, "\n");
	lines = tk->tokens;
	numlines = tk->num_tok;
	*NO = numlines;
	orderData = cleanMalloc((numlines + 1) * sizeof(order_t));
	
	for(i=0;i<numlines;i++) {
		tk = TKCreate(lines[i], 1, "|");
		items = tk->tokens;
		if(tk->num_tok != 4) { 
			fprintf(stderr,"\e[31mERROR: Invalid line number %d in order file.\e[0m\n", i);
			fprintf(stderr,"LINE = %s\n",lines[i]);
			return 0; 
		}
		orderData[i].title 	= trim(clean(items[0]));
		orderData[i].cost   	= atof(clean(items[1]));
		orderData[i].customerId	= atoi(clean(items[2]));
		orderData[i].catagory 	= (catagory_t) catList[sArraySearch(catList, clean(items[3]), numCat)];
		if(orderData[i].catagory == -1) {
			fprintf(stderr,"\e[31mERROR: Invalid catagory \'%s\' on line %d of order file.\e[0m\n", items[3], i);
			return 0;
		}
	}
	return orderData;

}

shmdata_t* setup_shm(consumer_t* consumerData, order_t* orderData, int NUMCONSUMER, int NUMORDER) {
	key_t 		SHM_KEY;	
	int 		SHMID;
	int 		KEY_ID;
	shmdata_t*	shmdata;
	FILE* 		CHECK;	
	char*		LOCKNAME;
	int 		startingKey;
	int 		req_size;
	
	LOCKNAME = "shmlock";	
	CHECK = fopen(LOCKNAME,"r");
	if(CHECK == 0) {
		fprintf(stderr,"\e[31mERROR: file \"%s\" used to lock shared memory is not present. Please create.\e[0m\n", LOCKNAME);
		return 0;
	}
	fclose(CHECK);
	printf("File lock found.\n");
	printf("nc = %d no = %d\n",NUMCONSUMER, NUMORDER);	
	shmdata = cleanMalloc(sizeof(shmdata_t));
	
	/* RESERVED:
	 * 0 = START
	 * 1 = STOP
	 */

	shmdata->NC = NUMCONSUMER;
	shmdata->NO = NUMORDER;
	shmdata->POS_readyFlag = 2;	
	shmdata->POS_doneFlag = shmdata->POS_readyFlag + NUMCONSUMER;
	shmdata->POS_errorFlag = shmdata->POS_doneFlag + NUMCONSUMER;
	shmdata->POS_queue = shmdata->POS_errorFlag + NUMCONSUMER;
	
	req_size = shmdata->POS_queue + NUMORDER*4 + 10;
	shmdata->size = req_size;

	printf("Initialized positions in shared memory data struct.\n");
	printf("Required size of shared memory: %d (%x) bytes\n", req_size, req_size);
	SHMID = -1;
	startingKey = 0;
	while(SHMID == -1) {
		SHM_KEY = -1;
		for(KEY_ID = startingKey; SHM_KEY == -1; KEY_ID++) {
			SHM_KEY = ftok(LOCKNAME, KEY_ID);
			printf("Tried key ID %d, got back key %d.\n", KEY_ID, SHM_KEY);
		}
		startingKey = KEY_ID + 1;
		SHMID = shmget(SHM_KEY, req_size, 0700 | IPC_CREAT | IPC_EXCL);
		printf("Tried key %d, got shmid %d\n",SHM_KEY,SHMID);
	}
	
	shmdata->SHMID = SHMID;
	printf("Shared memory fully initilized.\n");
	return shmdata;	
}

void produce(shmdata_t* shmdata, consumer_t* consumerData, order_t* orderData) {
	void*		SHM;
	int		i;
	int 		NUMORDERS;
	int 		NUMCONSUMERS;
	
	printf("<ProducerBot> Producer has risen!\n");
	
	SHM = shmat(shmdata->SHMID, NULL, 0);
	
	NUMCONSUMERS = shmdata->NC;
	NUMORDERS = shmdata->NO;
	*((char*)SHM) = 1;
	printf("<ProducerBot> Ready for duty, sir.\n");
	
	/* wait for overseer to send the start signal */
	while(*((char*)SHM) == 0) { }
	
	printf("<ProducerBot> Beginning production.\n");
	
	for(i=0;i<NUMORDERS;i++) {
		/* add to queue */
	}
	
	printf("<ProducerBot> All done.\n");
	/* wait for the consumers to finish */ /*
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
	}*/
	
	printf("<ProducerBot> Committing sepechu in the name of memory management!\n");
	shmdt(SHM);
	_exit(0);
}

void consume(shmdata_t* shmdata, consumer_t* consumerData, order_t* orderData, int ID) {
	void*		SHM;
	int 		i;

	/* init etc */	
	printf("<\e[36m%s\e[0m> Consumer %d reporting for duty, sir.\n", consumerData[ID].name, ID);
	
	SHM = shmat(shmdata->SHMID, NULL, 0);
	memset(SHM,0,shmdata->size); /* EXTREMELY HACKY FIX... PATCH THIS UP LATER? */
	for(i=0;i<shmdata->size;i++) {
		printf("%d ",*((char*)SHM+ i));
	}	printf("\n");

	*((char*)SHM + shmdata->POS_readyFlag + ID) = 1;
	printf("<\e[36m%s\e[0m> Signaled ready. Waiting for start.\n", consumerData[ID].name);
	while(*((char*)SHM) == 0) { }
		
	printf("<\e[36m%s\e[0m> Starting.\n", consumerData[ID].name);
	/* do funny consumer stuff here... weird people, those consumers */
	
	*((char*)SHM + shmdata->POS_doneFlag + ID) = 1;
	
	while(*((char*)SHM + 1)	== 0) { } /* wait for stop flag */
	printf("<\e[36m%s\e[0m> Stopping.\n", consumerData[ID].name);
	
	shmdt(SHM);

	_exit(0);
}

int spawnChildren(shmdata_t* shmdata, consumer_t* consumerData, order_t* orderData) {
	pid_t		PID_producer;
	pid_t* 		PID_consumers;
	int		i;
	int		NUMCONSUMERS;
	
	NUMCONSUMERS = shmdata->NC;
	PID_consumers = cleanMalloc(NUMCONSUMERS * sizeof(pid_t));
	/* first spawn the producer */
	PID_producer = fork();
	if(PID_producer == -1) {
		printf("\e[31m<OVERSEER> Error in forking of producer child process.\e[0m\n");
		return FAIL;
	}
	if(PID_producer == 0) {
		/* we are now in the child process! */
		produce(shmdata,consumerData,orderData);
	}else{
		printf("<OVERSEER> Spawned child process \"ProducerBot\" with id %d.\n", PID_producer);
	}
	/* now spawn all child processes. */
	for(i=0;i<NUMCONSUMERS;i++) {
		PID_consumers[i] = fork();
		if(PID_consumers[i] == -1 ) {
			printf("\e[31m<OVERSEER> Error in forking of consumer process \"%s\" (%d).\e[0m\n", consumerData[i].name, i);
			return FAIL;
		}
		if(PID_consumers[i] == 0) {
			/* child process for this consumer */
			consume(shmdata,consumerData,orderData,i);
		}else{
			printf("<OVERSEER> Spawned child process \"%s\" with id %d.\n", consumerData[i].name, PID_consumers[i]);
		}
		
	}
	return SUCCESS;	
}

void stopAll(shmdata_t* shmdata, void* SHM) {
	*((char*)SHM + 1) = 1;
}

int ready(shmdata_t* shmdata, void* SHM) {
	int		i;
	
	for(i = shmdata->POS_readyFlag; i < shmdata->POS_doneFlag; i++) {
		if(*((char*)SHM + i) == 0) {
			return 0;
		}
	}
	
	return 1;
}

int done(shmdata_t* shmdata, void* SHM) {
	int		i;
	
	for(i = shmdata->POS_doneFlag; i < shmdata->POS_errorFlag; i++) {
		if(*((char*)SHM + i) == 0) {
			return 0;
		}
	}
	
	return 1;
}

int error(shmdata_t* shmdata, void* SHM) {
	int		i;
	
	for(i = shmdata->POS_errorFlag; i < shmdata->POS_queue; i++) {
		if(*((char*)SHM + i) == 1) {
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
	shmdata_t* 	shmdata;	/* Shared Memory data */
	void* 		SHMCHUNK;	/* main function's pointer to the shared memory chunk */
	int 		NUMCONSUMER;
	int 		NUMORDER;
		
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
	
	NUMCONSUMER = 0;
	NUMORDER = 0;
	
	consumerData = processDatabase(DATABASE, &NUMCONSUMER);
	if(consumerData == 0) { return FAIL; }

	orderData = processOrders(ORDER, catList, numCat, &NUMORDER);
	if(orderData == 0) { return FAIL; }
	
	printData(consumerData, orderData, catList, numCat);
	shmdata = setup_shm(consumerData, orderData, NUMCONSUMER, NUMORDER);
	if(shmdata == 0) { return FAIL; }
	SHMCHUNK = shmat(shmdata->SHMID, NULL, 0);
	
	for(i=0;i<shmdata->size;i++) {
		printf("%d ",*((char*)SHMCHUNK + i));
	}	printf("\n");
	
	printf("<OVERSEER> Arise, my minions!\n");
	if(spawnChildren(shmdata, consumerData, orderData)) {
		printf("<OVERSEER> A miscarrige occured :(\n");
		return 0;
	}
	for(i=0;i<shmdata->size;i++) {
		printf("%d ",*((char*)SHMCHUNK + i));
	}	printf("\n");
	sleep(1); /* artificially wait for all processes to initialize, will cause problems if your machine is horrificly slow. */
		
	
	printf("<OVERSEER> All initialization completed successfully.\n");
	printf("<OVERSEER> Starting processes...\n");
	for(i=0;i<shmdata->size;i++) {
		printf("%d ",*((char*)SHMCHUNK + i));
	}	printf("\n");
	sleep(1);
	while(!ready(shmdata,SHMCHUNK));
	printf("ALL ARE READY!\n");	
	/* THE ALL IMPORTANT HOLY GOD LINE */
	*((char*)SHMCHUNK) = 1;
	/* END ALL IMPORTANT HOLY GOD LINE */
for(i=0;i<shmdata->size;i++) {
		printf("%d ",*((char*)SHMCHUNK + i));
	}	printf("\n");

	sleep(1); /* wait for the processes to run a bit... give em a chance! */
	for(i=0;i<shmdata->size;i++) {
		printf("%d ",*((char*)SHMCHUNK + i));
	}	printf("\n");

	while(1) {
		if(done(shmdata, SHMCHUNK)) {
			printf("<OVERSEER> All done!\n");
			break;
		}
		if(error(shmdata, SHMCHUNK)) {
			fprintf(stderr,"\e[31m<OVERSEER> ERROR: One of the consumers signaled an error, stop the presses!\e[0m\n");
			stopAll(shmdata, SHMCHUNK);
			shmdt(SHMCHUNK);	
			return FAIL;
		}
	}
for(i=0;i<shmdata->size;i++) {
		printf("%d ",*((char*)SHMCHUNK + i));
	}	printf("\n");

	stopAll(shmdata, SHMCHUNK);
for(i=0;i<shmdata->size;i++) {
		printf("%d ",*((char*)SHMCHUNK + i));
	}	printf("\n");

	sleep(1); /* wait for all children to die, and remove the memory chunk */
	shmdt(SHMCHUNK);
	return SUCCESS;
}
