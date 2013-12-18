#define _OPEN_THREADS                                                           
#include <pthread.h>                                                            
#include <stdio.h>                                                              
#include <stdlib.h>
#include <unistd.h>                                                             
                                                                                
void *thread(void *arg) {                                                       
  /* A simple loop with only puts() would allow a thread to write several       
  lines in a row.                                                               
  With pthread_yield(), each thread gives another thread a chance before        
  it writes its next line */                                                    
  while (1) {                                                                   
    puts((char*) arg);                                                          
    pthread_yield(NULL);                                                        
    sleep(1);
  }                                                                             
}                                                                               
                                                                                
int main() {                                                                        
  pthread_t t1, t2, t3;                                                         
                                                                                
  if (pthread_create(&t1, NULL, thread, "thread 1") != 0) {                     
    perror("pthread_create() error");                                           
    exit(1);                                                                    
  }                                                                             
                                                                                
  if (pthread_create(&t2, NULL, thread, "thread 2") != 0) {                     
    perror("pthread_create() error");                                           
    exit(2);                                                                    
  }                                                                             
                                                                                
  if (pthread_create(&t3, NULL, thread, "thread 3") != 0) {                     
    perror("pthread_create() error");                                           
    exit(3);                                                                    
  }                                                                             
                                                                                
  sleep(6);
                                                                                
  exit(0); /* this will tear all threads down */                                
}                                                                               
/**
Output:

thread 1
thread 3
thread 2
thread 1
thread 3
thread 2
thread 1
thread 3
thread 2
thread 1
thread 3
*/
