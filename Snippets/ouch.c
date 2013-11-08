#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void ouch(int sig) {
    printf("OUCH - I got a signal %d\n", sig);
    (void) signal(SIGINT, SIG_DFL);
}

void winchange(int sig) {
    printf("windows changed \n");
}

int main() {
    
    (void) signal(SIGINT, ouch);
    (void) signal(SIGWINCH, winchange);
    
    while (1) {
        printf("Hello World!\n");
        sleep(1);
    }
}