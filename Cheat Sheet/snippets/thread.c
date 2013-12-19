void *threadFunc(void *arg) {
	char *str; int i = 0;
	str=(char*)arg;
	while(i < 110 ) {
		usleep(1);
		printf("threadFunc says: %s\n",str);
		++i; }
	return NULL; }
int main(void) {
	pthread_t pth; int i = 0;
	pthread_create(&pth,NULL,threadFunc,"foo");
	while(i < 100) {
		usleep(1);
		printf("main is running...\n");
		++i; }
	printf("main waiting for thread to terminate...\n");
	pthread_join(pth,NULL);
	return 0; }