#include "book.h"

int main(int argc, char** argv) {
	int 		i;

	printf("Exec successful:\n");
	printf("Running consumer with args\n");
	
	for(i=0;i<argc;i++) {
		printf("argv[%d] = \"%s\"\n", i, argv[i]);
	}
	return SUCCESS;
}
