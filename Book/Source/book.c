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
	int		j;
	
	database_size = getFileLength(DATABASE);
	file_string = cleanMalloc(database_size + 1);
	fread(file_string, 1, database_size, DATABASE);
	fclose(DATABASE);
	
	tk = TKCreate(file_string, 1, "\n");
	lines = tk->tokens;
	numlines = tk->num_tok;
	*NC = numlines;
	consumerData = cleanMalloc((numlines + 1) * sizeof(consumer_t));
	
	j=0;	
	for(i=0;i<numlines;i++) {
		tk = TKCreate(lines[i], 1, "|");
		items = tk->tokens;
		if(tk->num_tok == 0) {
			fprintf(stderr,"\e[33mWARNING: Blank/contentless line %d of database file.\e[0m\n", i);
			continue;
		}
		if(tk->num_tok != 6) { 
			fprintf(stderr,"\e[31mERROR: Invalid line number %d in database file.\e[0m\n", i);
			return 0; 
		}
		consumerData[j].name 	= trim(clean(items[0]));
		consumerData[j].id   	= atoi(clean(items[1]));
		consumerData[j].credit 	= atof(clean(items[2]));
		consumerData[j].address = trim(clean(items[3]));
		consumerData[j].state 	= trim(clean(items[4]));
		consumerData[j].zip	= trim(clean(items[5]));
			
		if(consumerData[j].id == 0) {
			printf("\e[33mWARNING: Customer id on line %d of database file has id 0. Possibly an invalid integer.\e[0m\n", i);
		}
		if(consumerData[j].id < 0) {
			fprintf(stderr,"\e[31mERROR: Customer id one line %d of database file is negative.\e[0m\n", i);
			return 0;
		}
		
		if(consumerData[j].credit == 0) {
			printf("\e[33mWARNING: Customer's credit balance on line %d of order file (%s) is $ 0.00. Possibly a parse error.\e[0m\n",i,consumerData[j].name);	
		}
		if(consumerData[j].credit < 0) {
			fprintf(stderr,"\e[31mERROR: Customer %s's credit balance (line %d) is negative.\e[0m\n",consumerData[j].name,i);
			return 0;
		}




		j++;
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

order_t* processOrders(FILE* ORDER, consumer_t* consumerData, catagory_t* catList, size_t numCat, int* NO, int NC) {
	char* 		file_string;	/* string containing all the contents of the database */
	int 		order_size;	/* size (number of characters) of the database */
	TokenizerT* 	tk;		/* tokenizer for the database */
	char** 		lines;		/* list of lines in the database */
	char** 		items;		/* reused array for each line; the properties of each consumer */
	int 		numlines;	/* number of lines */
	int 		i;		/* iterator */
	order_t*	orderData;
	int		j;
	int		c;
	int 		fail;
			
	order_size = getFileLength(ORDER);
	file_string = cleanMalloc(order_size + 1);
	fread(file_string, 1, order_size, ORDER);
	fclose(ORDER);
	
	tk = TKCreate(file_string, 1, "\n");
	lines = tk->tokens;
	numlines = tk->num_tok;
	*NO = numlines;
	orderData = cleanMalloc((numlines + 1) * sizeof(order_t));
	
	j = 0;
	for(i=0;i<numlines;i++) {
		tk = TKCreate(lines[i], 1, "|");
		items = tk->tokens;
		if(tk->num_tok == 1) {
			fprintf(stderr,"\e[33mWARNING: Blank/contentless line %d of order file.\e[0m\n", i);
			continue;
		}
		if(tk->num_tok != 4) { 
			fprintf(stderr,"\e[31mERROR: Invalid line number %d in order file.\e[0m\n", i);
			return 0; 
		}
		orderData[j].title 	= trim(clean(items[0]));
		orderData[j].cost   	= atof(clean(items[1]));
		orderData[j].customerId	= atoi(clean(items[2]));
		orderData[j].catagory 	= sArraySearch(catList, clean(items[3]), numCat);
		if(orderData[j].catagory == -1) {
			fprintf(stderr,"\e[31mERROR: Invalid catagory \'%s\' on line %d of order file.\e[0m\n", clean(items[3]), i);
			return 0;
		}
		orderData[j].catagory 	= catList[orderData[j].catagory];
		
		if(orderData[j].cost == 0) {
			printf("\e[33mWARNING: The order on line %d is $ 0.00. Check for bad formatting.\e[0m\n", i);
		}
		if(orderData[j].cost < 0) {
			fprintf(stderr,"\e[31mERROR: The order on line %d has a negative dollar value.\e[0m\n",i);
			return 0;
		}
		/*check to make sure that customer is in database */
		fail = 1;
		for(c=0;c<NC;c++) {
			if(consumerData[c].id == orderData[j].customerId) {
				fail = 0;
				break;
			}
		}	
		if(fail) {
			fprintf(stderr,"\e[31mERROR: The order on line %d of the order file is for a customer \'%d\' that does not exist.\e[0m\n",i,orderData[j].customerId);
			return 0;
		}

		j++;
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
	int 		tries;
		
	LOCKNAME = "shmlock";	
	CHECK = fopen(LOCKNAME,"r");
	if(CHECK == 0) {
		fprintf(stderr,"\e[33mWARNING: File \"%s\" used to anchor shared memory is not present (creating).\e[0m\n", LOCKNAME);
		CHECK = fopen(LOCKNAME,"w+");
	}
	fclose(CHECK);
	printf("File lock found.\n");
	shmdata = cleanMalloc(sizeof(shmdata_t));
	
	/* RESERVED:
	 * 0 = START
	 * 1 = STOP
	 */

	shmdata->NC = NUMCONSUMER;
	shmdata->NO = NUMORDER;
	shmdata->POS_readyFlag = 2;	
	shmdata->POS_doneFlag = shmdata->POS_readyFlag + 1 + NUMCONSUMER + 1;
	shmdata->POS_errorFlag = shmdata->POS_doneFlag + 1;
	shmdata->POS_queue = shmdata->POS_errorFlag + 1;
	shmdata->POS_money = shmdata->POS_queue + 4 + NUMORDER*sizeof(int) + 10;
	shmdata->POS_response = shmdata->POS_money + NUMCONSUMER*sizeof(double) + 5;	
	req_size = shmdata->POS_response + NUMORDER + 10;
	shmdata->size = req_size;

	printf("Initialized positions in shared memory data struct.\n");
	printf("Required size of shared memory: %d (0x%X) bytes\n", req_size, req_size);
	SHMID = -1;
	startingKey = 0;
	tries = 0;
	while(SHMID == -1) {
		SHM_KEY = -1;
		for(KEY_ID = startingKey; SHM_KEY == -1; KEY_ID++) {
			SHM_KEY = ftok(LOCKNAME, KEY_ID);
		}
		startingKey = KEY_ID + 1;
		SHMID = shmget(SHM_KEY, req_size, 0700 | IPC_CREAT | IPC_EXCL);
		tries++;
	}
	printf("Acquired SHMID %d after %d tries.\n",SHMID,tries);
	shmdata->SHMID = SHMID;
	printf("Shared memory fully initilized.\n");
	return shmdata;	
}

void produce(shmdata_t* shmdata, consumer_t* consumerData, order_t* orderData) {
	void*		SHM;
	int		i;
	int 		NUMORDERS;
	int 		NUMCONSUMERS;
	
	printf("<\e[34mProducerBot\e[0m> Producer has arrived.\n");
		
	NUMCONSUMERS = shmdata->NC;
	NUMORDERS = shmdata->NO;
	
	SHM = shmat(shmdata->SHMID, NULL, 0);
	memset(SHM,0,shmdata->size); /* EXTREMELY HACKY FIX... PATCH THIS UP LATER? */
	
		
	printf("<\e[34mProducerBot\e[0m> Signaled ready. Waiting for start.\n");
	while(*((char*)SHM) == 0) { 
		*((char*)SHM + shmdata->POS_readyFlag) = 1;	
		*((int*)SHM +  shmdata->POS_queue) = -1;
	}	
	
	printf("<\e[34mProducerBot\e[0m> Starting production.\n");
	
	*((int*)SHM +  shmdata->POS_queue) = -1;
	
	for(i=0;i<NUMORDERS;i++) {
		/* add to queue */
		if(i == NUMORDERS) {
			break;
		}
		*((int*)SHM + shmdata->POS_queue + 4 + 4*i) = i;	
		if(*((int*)SHM +  shmdata->POS_queue) == -1) {
			*((int*)SHM +  shmdata->POS_queue) = 0;
		}
			
	}

	
	printf("<\e[34mProducerBot\e[0m> All done!.\n");

	while(*((char*)SHM + 1)	== 0) { } /* wait for stop flag */
	
	shmdt(SHM);
	_exit(0);
}

void consume(shmdata_t* shmdata, consumer_t* consumerData, order_t* orderData, int ID, int trueID) {
	void*		SHM;
	int 		i;
	int		current_order;
	double		credit;
	
	/* init etc */	
	printf("<\e[36m%s\e[0m> Consumer %d reporting for duty, sir.\n", consumerData[ID].name, ID);
	
	credit = consumerData[ID].credit;
	SHM = shmat(shmdata->SHMID, NULL, 0);
	memset(SHM,0,shmdata->size); /* EXTREMELY HACKY FIX... PATCH THIS UP LATER? */
	
	
	*((char*)SHM + shmdata->POS_readyFlag) += 1;
	printf("<\e[36m%s\e[0m> Signaled ready. Waiting for start.\n", consumerData[ID].name);
	while(*((char*)SHM) == 0) {
		*((char*)SHM + shmdata->POS_readyFlag + ID + 1) = 1;
	}
		
	printf("<\e[36m%s\e[0m> Starting.\n", consumerData[ID].name);
	
	/* wait for the producer to start producing, prevents a false start */
	while(*((int*)SHM + shmdata->POS_queue) == -1) {
	}
	while(1) {
		if(*((int*)SHM + shmdata->POS_queue) >= shmdata->NO) {
			break;
		}
		
		current_order = *((int*)SHM + shmdata->POS_queue + *((int*)SHM + shmdata->POS_queue)*4 + 4);
		if(current_order < 0) {
			break;
		}
		if(orderData[current_order].customerId == trueID) {
			if(orderData[current_order].cost <= credit){	
				*((int*)SHM +  shmdata->POS_queue) += 1;
			 	*((int*)SHM + shmdata->POS_response + current_order) = -1; /* success */
				printf("<\e[32m%s\e[0m> Successfully ordered order %d.\n",consumerData[ID].name,current_order);
				credit -= orderData[current_order].cost;
				*((double*)SHM + shmdata->POS_money + ID*sizeof(double)) = credit;
				if(current_order >= shmdata->NO-1) { break; }
			}else{
				
				*((int*)SHM +  shmdata->POS_queue) += 1;
				printf("<\e[32m%s\e[0m> Failed ordering order %d!\n",consumerData[ID].name,current_order);
			 	*((int*)SHM + shmdata->POS_response + current_order) = -2; /* failiure */
				if(current_order >= shmdata->NO-1) { break; }
			}
		}
		if(shmdata->NO-1 <= current_order) {
			break;
		}
	}
	printf("<\e[36m%s\e[0m> Done.\n", consumerData[ID].name);	

	
	*((char*)SHM + shmdata->POS_doneFlag) += 1;
	
	while(*((char*)SHM + 1)	== 0) { } /* wait for stop flag */
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
		printf("<%sOVERSEER%s> Spawned child process \"ProducerBot\" with id %d.\n",CY, CR,PID_producer);
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
			consume(shmdata,consumerData,orderData,i,consumerData[i].id);
		}else{
			printf("<%sOVERSEER%s> Spawned child process \"%s\" with id %d.\n", CY,CR,consumerData[i].name, PID_consumers[i]);
		}
		
	}
	return SUCCESS;	
}

void stopAll(shmdata_t* shmdata, void* SHM) {
	*((char*)SHM + 1) = 1;
}

int ready(shmdata_t* shmdata, void* SHM) {
	int		i;
	int		t;
	
	t = 0;
	for(i=shmdata->POS_readyFlag;i<shmdata->POS_doneFlag;i++) {
		t += *((char*)SHM + i);
	}
	return t >= shmdata->NC + 1 ? 1 : 0;
}

int done(shmdata_t* shmdata, void* SHM) {
	int		i;
	
	return *((unsigned char*)SHM + shmdata->POS_doneFlag) >= (char)shmdata->NC ? 1 : 0;	
}

int error(shmdata_t* shmdata, void* SHM) {
	int		i;

	return *((unsigned char*)SHM + shmdata->POS_errorFlag) > 0 ? 1 : 0;	
}

void finalReport(shmdata_t* data, consumer_t* consumerData, order_t* orderData, void* SHM) {
	int 		i;
	int		j;
	double		sim;
	FILE*		f;
	
	f = fopen("finalreport.txt", "w");	
	for(i=0;i<data->NC;i++) {
		sim = consumerData[i].credit;
		fprintf(f,"=== BEGIN CUSTOMER INFO ===\n");
		fprintf(f,"### BALANCE ###\n");
		fprintf(f,"Customer name: %s\n",consumerData[i].name);
		fprintf(f,"Customer id number: %d\n",consumerData[i].id);
		fprintf(f,"Remaining credit balance after all purchases (a dollar amount): %.2f\n", *((double*)SHM + data->POS_money + i*sizeof(double)));
		/* HEY PAUL
		 * YOU MIGHT NOTICE THAT THE FINAL REPORT IS BLANK ON THE ORDERS LISTS
		 * WELL THIS IS THE PROBELM
		 * THE RESPONSES FROM THE CONSUMERS ARE BEING OVERWRITTEN OR SOMETHING
		 * FIX IT UP PLS!
		 */
		fprintf(f,"### SUCCESSFUL ORDERS ###\n");
		for(j=0;j<data->NO;j++) {
			if(orderData[j].customerId == consumerData[i].id) {
				if(*((int*)SHM + data->POS_response + j) == -1) {
					sim -= orderData[j].cost;
					fprintf(f,"\"%s\"| %.2f| %.2f\n",orderData[j].title, orderData[j].cost, sim);
				}
			}
		}
		fprintf(f,"### REJECTED ORDERS ###\n"); 
		for(j=0;j<data->NO;j++) {
			if(orderData[j].customerId == consumerData[i].id) {
				if(*((int*)SHM + data->POS_response + j) == -2) {
					fprintf(f,"\"%s\"| %.2f\n",orderData[j].title, orderData[j].cost);
				}
			}
		}
		fprintf(f,"=== END CUSTOMER INFO ===\n\n");

	}
	fclose(f);
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

	orderData = processOrders(ORDER, consumerData, catList, numCat, &NUMORDER, NUMCONSUMER);
	if(orderData == 0) { return FAIL; }
	
	/*printData(consumerData, orderData, catList, numCat);*/
	shmdata = setup_shm(consumerData, orderData, NUMCONSUMER, NUMORDER);
	if(shmdata == 0) { return FAIL; }
	SHMCHUNK = shmat(shmdata->SHMID, NULL, 0);
	
	
	printf("<%sOVERSEER%s> Arise, my minions!\n",CY,CR);
	if(spawnChildren(shmdata, consumerData, orderData)) {
		printf("\e[31m<OVERSEER> A miscarrige occured :(\e[0m\n");
		return 0;
	}
		
	
	printf("<%sOVERSEER%s> All initialization completed successfully.\n",CY,CR);
	printf("<%sOVERSEER%s> Waiting for ready...\n",CY,CR);
	while(!ready(shmdata,SHMCHUNK));
	printf("<%sOVERSEER%s> Starting processes...\n",CY,CR);
	*((int*)SHMCHUNK + shmdata->POS_queue) = -1;
	/* THE ALL IMPORTANT HOLY GOD LINE */
	*((char*)SHMCHUNK) = 1;
	/* END ALL IMPORTANT HOLY GOD LINE */
	

	while(1) {
		if(done(shmdata, SHMCHUNK)) {
			printf("<%sOVERSEER%s> All done!\n",CY,CR);
			break;
		}
	}
	finalReport(shmdata, consumerData, orderData, SHMCHUNK);
	stopAll(shmdata, SHMCHUNK);
	shmdt(SHMCHUNK);
	shmctl(shmdata->SHMID, IPC_RMID, 0); /* remove the segment */
	return SUCCESS;
}
