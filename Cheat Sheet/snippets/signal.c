/* registering to catch */
void (*signal(int sig, void (*func)(int)))(int);
/* sending to process, even self */
int kill(pid_t pid, int sig);