void handler ( void *ptr );
sem_t mutex;
int counter; 
int main() {
	int i[2];i[0]=0;i[1]=1;
	pthread_t a;pthread_t b;
	sem_init(&mutex, 0, 1);
	pthread_create(&a,NULL,(void*)&h,(void*)&i[0]);
	pthread_create(&b,NULL,(void*)&h,(void*)&i[1]);
	pthread_join(a, NULL);
	pthread_join(b, NULL);
	sem_destroy(&mutex);	
	exit(0); }
void h ( void *ptr ) {
	int x; 
	x = *((int *) ptr);
	sem_wait(&mutex);
	counter++; 
	sem_post(&mutex);
	pthread_exit(0); }

