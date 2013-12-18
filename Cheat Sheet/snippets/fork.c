char * message; int n;

pid_t pid = fork();

switch (pid) {
    case -1:
        perror("fork failed\n");
        exit(1);
    case 0:
        message = "This is the child";
        n = 5;
        break;
    default:
        message = "This is the parent";
        n = 3;
        break;
}

for(; n > 0; n--) {
    puts(message);
    sleep(1);
}