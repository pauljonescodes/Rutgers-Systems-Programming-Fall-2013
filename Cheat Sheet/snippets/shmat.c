key_t key; /* key to be passed to shmget() */ 
int shmflg; /* shmflg to be passed to shmget() */ 
int shmid; /* return value from shmget() */ 
int size; /* size to be passed to shmget() */ 
char *shm; /* data to be passed */

if ((shmid=shmget (key, size, shmflg)) == -1){}
if ((shm=shmat(shmid, NULL, 0))==(char *) -1){}
if (shmdt(shmid) == -1){}