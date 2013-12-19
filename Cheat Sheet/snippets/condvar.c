pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var = PTHREAD_COND_INITIALIZER;
int  count = 0;
#define COUNT_DONE 10, COUNT_HALT1 3, COUNT_HALT2 6
main() {
   pthread_t thread1, thread2;
   pthread_create( &thread1, NULL, &functionCount1, NULL);
   pthread_create( &thread2, NULL, &functionCount2, NULL);
   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL);
   exit(0); }
void *functionCount1() {
   for(;;) {
      pthread_mutex_lock( &count_mutex );
      pthread_cond_wait( &condition_var, &count_mutex );
      count++;
      pthread_mutex_unlock( &count_mutex );
      if(count >= COUNT_DONE) return(NULL); } }
void *functionCount2() {
    for(;;) {
    pthread_mutex_lock( &count_mutex );
    if( count < COUNT_HALT1 || count > COUNT_HALT2 ) {
       pthread_cond_signal( &condition_var ); }
    else {
       count++; }
    pthread_mutex_unlock( &count_mutex );
    if(count >= COUNT_DONE) return(NULL); } }