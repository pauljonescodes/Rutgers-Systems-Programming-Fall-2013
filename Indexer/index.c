#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include "tokenizer.h"
#include "sorted-list.h"
#include "index.h"

SortedListPtr sl;

int compareFileNode(void* f1, void* f2) {
	return strcmp(((fileListNode*)f1)->fileName, ((fileListNode*)f1)->fileName);
}

int compareWordNode(void* f1, void* f2) {
	return strcmp(((wordListNode*)f1)->word, ((wordListNode*)f1)->word);
}

void printFileNode(fileListNode* fn) {
	printf("(\"%s\", %d),", fn->fileName, fn->count);
}

void printWordNode(wordListNode wn) {
	SortedListIteratorPtr si = SLCreateIterator(wn.fileList);
	fileListNode* fn = SLNextItem(si);
	printf("\"%s\" -> ",wn.word);
	while(fn != 0) {
		printFileNode(fn);
		fn = SLNextItem(si);
	}
	printf("\n");
}

void printLists(SortedListPtr sl) {
	SortedListIteratorPtr si = SLCreateIterator(sl);
	wordListNode* wn = SLNextItem(si);
	while(wn != 0) {
		printWordNode(*wn);
		wn = SLNextItem(si);
	}
}

void process_word(char * word, char* dname) {
 	wordListNode* wn = (wordListNode*)SLFind(sl,word);
	SortedListPtr fList;
	fileListNode*  fn;
	if(wn == 0) {
		SLInsert(sl,word);
		return;
	}
	fList = wn->fileList;
	fn = SLFind(fList,dname);
	if(fn == 0) {
		SLInsert(fList,dname);
		fn = SLFind(fList,dname);
	}
	fn->count++;
}

void process_file(const char *fname, int filesize, char* directory_name)
{
    char *file_contents;
    FILE *input_file = fopen(fname, "rb");
    TokenizerT * tokenizer;
    char * token;
    
    if(input_file==0) {
        return;
    }
    fseek(input_file, 0, SEEK_END);
    file_contents = malloc(ftell(input_file)+5);
    memset(file_contents,0,ftell(input_file)+5);
    fseek(input_file, 0, SEEK_SET);
    fread(file_contents, sizeof(char), filesize, input_file);
    fclose(input_file);
    tokenizer = TKCreate(file_contents);
    while ((token = TKGetNextToken(tokenizer))) {
        process_word(token,directory_name);
    }
}

void get_files_in(const char * root_name)
{
    DIR * root;
    struct dirent * current_entry;
    char * directory_name;
    char next_root[PATH_MAX];
    struct stat file_stat;
    
    if ((root = opendir(root_name))) {
        while ((current_entry = readdir(root))) {
            directory_name = current_entry->d_name;
            
            if (*directory_name != '.') {
                
                sprintf (next_root,"%s/%s", root_name, directory_name);
                
                
                stat(next_root,&file_stat);
                
                if (!S_ISDIR(file_stat.st_mode)) {
                    process_file(next_root, file_stat.st_size,directory_name);
                }
                
                get_files_in(next_root);
            }
        }
    }
    
}

SortedListPtr SL_Init() {
	SortedListPtr s = SLCreate(compareWordNode);
	return s;
}

int main(int argc, char **argv) {
    sl = SL_Init();
    get_files_in(argv[1]);
    printLists(sl);
    return 0;
}



