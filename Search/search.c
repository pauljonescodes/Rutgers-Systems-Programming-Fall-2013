#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "sorted-list.h"
#include "index.h"
#include "search.h"

#define VERBOSE 1

int compareStrings(void* s1, void* s2) {
	return strcmp((char*)s1,(char*)s2);
}

SortedListPtr search(TokenizerT* tk, SortedListPtr sl, int op) {
	char* t = 0;
	SortedListPtr l = SLCreate(compareFileNodes);
	wordListNode * wl = malloc(sizeof(wordListNode));
	wordListNode * il = malloc(sizeof(wordListNode));
	memset(il,0,sizeof(wordListNode));
	memset(wl,0,sizeof(wordListNode));
	while(1) {
		SortedListIteratorPtr fl;
		t = TKGetNextToken(tk);
		if(t==0) { break; }
		wl->word = t;
		il = SLFind(sl,wl);
		if(il == 0) { continue; }
		fl = SLCreateIterator(il->fileList);
		while(1){
			fileListNode* f = SLNextItem(fl);
			fileListNode* item = SLFind(l,f->fileName);
			if(item == 0) {
				SLInsert(l,f->fileName);
			}
			item->count++;
		}
	}	
	return l;
}

int loop(SortedListPtr sl) {
	char* input;
	char* command;
	int op;
	SortedListPtr sl = SLCreate(compareStrings);
	SortedListIteratorPtr si;
	TokenizerT* tk;
	input = malloc(1000);
	memset(input,0,1000);

	while(1) {
		SortedListPtr r;
		fileListNode fn;
		SortedListIteratorPtr i;
		scanf("%[^\n]s",input);
		if(strcmp(input,"q")==0) {
			printf("Exiting.\n");
			return 0;
		}
		tk = TKCreate(input);
		if(tk==0) {
			printf("Error in tokenizing input.\n");
			return 1;
		}
		command = TKGetNextToken(tk);
		if(strcmp(command,"sa")==0) {
			op = 0;
		}else if(strcmp(command,"so")==0) {
			op = 1;
		}else{
			printf("Error: Invalid command.\n");
		}
		printf("Searching for %s, op = %d\n",input,op);
		r = search(tk,sl,op);
		i = SLCreateIterator(r);
		while(1) {
			fn = SLNextItem(i);
			if(fn == 0) { break; }
			if(SLFind(sl,fn->filename) == 0) {
				SLInsert(sl,fn->filename);
			}
		}
	}
	si = SLCreateIterator(sl);
	while(1) {
		char* s = SLNextItem(si);
		if(s==0) {break;}
		printf("%s ",s);
	}
	return 0;
	
}

char *substring(const char* str, int beg, int n)
{
    char *ret = malloc(n+1);
    strncpy(ret, (str + beg), n);
    *(ret+n) = '0';
    
    return ret;
}

void parse_index_file(char * index_file, SortedListPtr sl) {
    if (VERBOSE) {
        printf("parsing file %s\n", index_file);
    }
    
    FILE* file = fopen(index_file, "r"); /* should check the result */
    char line[1024];
    char * pch;
    int is_frequency = 0;
    
    wordListNode * wln;
    fileListNode * fln;
    
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "<list>") != NULL) { /* start of a new word. */
            char* word = substring(line, strlen("<list> "), strlen(line) - 3);
            printf("word: %s", word);
            
            wln = malloc(sizeof(*wln));
            wln->word = word;
            wln->fileList = SLCreate(compareFileNode);
            
        } else if (strstr(line, "</list>") != NULL) { /* end of a new word */
            
        } else { /* file list */
            pch = strtok (line," \n");
            is_frequency = 0;
            
            while (pch != NULL)
            {
                if (is_frequency) {
                    printf("freq: %s \n", pch);
                    is_frequency = 0;
                    
                    fln->count = atoi(pch);
                    SLInsert(wln->fileList, fln);
                } else {
                    printf("file: %s \n", pch);
                    is_frequency = 1;
                    
                    fln = malloc(sizeof(*fln));
                    fln->fileName = pch;
                }
                
                pch = strtok (NULL, " \n");
            }
            
            SLInsert(sl, wln);
        }
    }
}

int main(int argc, char **argv) {
	SortedListPtr sl;

	if(argc!=2) {
		printf("ERROR: Invalid number of arguments. Usage should be \n\t$ ./search <query>\n");
		return 1;
	}
    
    parse_index_file(argv[1], sl);
    
	/*add reader stuff here */
	return loop(sl);	
}

