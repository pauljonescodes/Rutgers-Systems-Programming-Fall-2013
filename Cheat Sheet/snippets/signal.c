FILE *temp_file;
void leave(int sig) {
	fprintf(temp_file,"\nInterrupted..\n");
	fclose(temp_file);
	exit(sig); }
main() {
	(void) signal(SIGINT,leave);
	temp_file = fopen("tmp","w");
	for(;;) {
	        printf("Ready...\n");
	        (void)getchar();
	}
	exit(EXIT_SUCCESS); }