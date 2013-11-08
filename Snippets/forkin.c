#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t child_pid;
    int status;
    child_pid = fork();
    switch (child_pid) {
        case -1:
            printf("-1\n");
            break;
        
        case 0:
            printf("0\n");
            break;
            
        default:
            printf("errthng else %i", child_pid);
            break;
    }
}