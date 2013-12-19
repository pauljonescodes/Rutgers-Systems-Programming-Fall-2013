pthread_t tid[2];int count;pthread_mutex_t lock;
void* doSomeThing(void *arg) {
	pthread_mutex_lock(&lock);
	unsigned long i = 0; count += 1;
	for(i=0; i<(0xFFFFFFFF);i++);
	pthread_mutex_unlock(&lock);
	return NULL;
} int main(void) {
	int i = 0; int err;
	if (pthread_mutex_init(&lock, NULL) != 0) {
		return 1; }
	while(i < 2) {
		err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
		i++; }
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	pthread_mutex_destroy(&lock);
	return 0; }