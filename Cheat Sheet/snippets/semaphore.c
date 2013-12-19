void handler ( void *ptr );
sem_t mutex;
int counter; 
int main() {
    int i[2];i[0] = 0; i[1] = 1;
    pthread_t thread_a;
    pthread_t thread_b;
    sem_init(&mutex, 0, 1);                              
    pthread_create (&thread_a, NULL, (void *) &handler, (void *) &i[0]);
    pthread_create (&thread_b, NULL, (void *) &handler, (void *) &i[1]);
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);
    sem_destroy(&mutex);
    exit(0); }
void handler ( void *ptr ) {
    int x; 
    x = *((int *) ptr);
    sem_wait(&mutex);
    counter++; 
    sem_post(&mutex);
    pthread_exit(0); }

