pid_t pid = fork();
switch (pid) {
case -1: perror("fork failed\n"); exit(1);
case 0: /* This is the child" */ break;
default: /* "This is the parent" */ break; }