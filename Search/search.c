#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "sorted-list.h"
#include "index.h"
#include "search.h"

#define VERBOSE 0
#define pls printf("%d\n",__LINE__);

int number_of_files;
char** file_list;

int compareStrings(void* s1, void* s2) {
	return strcmp((char*)s1,(char*)s2);
}

void search(TokenizerT* tk, SortedListPtr sl, int op) {
	int number_of_words = tk->num_tok - 1;
    char** table = malloc(number_of_words * number_of_files);
    int word_index = 0;
    int i,j;
    memset(table,0,number_of_files*number_of_words);
    pls
	printf("printing words:\n");
	SortedListIteratorPtr si = SLCreateIterator(sl);
	while(1) {
		wordListNode* wn = SLNextItem(si);
		if(wn == 0) { break; }
		printf("word: \'%s\'\n",wn->word);
	}
    while(1) {
        pls
        char* s = TKGetNextToken(tk);
        wordListNode* current_word;
        wordListNode* wn;
        fileListNode* fn;
        int i;
        if(s==0) { break; }
        pls
        wn = malloc(sizeof(wordListNode));
        memset(wn,0,sizeof(wordListNode));
        fn = malloc(sizeof(wordListNode));
        memset(fn,0,sizeof(wordListNode));
        pls
        wn->word = s;
    pls
	printf("word = %s\n",s);
        current_word = SLFind(sl,wn);
        if(current_word == 0) { continue; }
        printf("wordfoundn\n");
        for(i=0;i<number_of_files;i++) {
            fn->fileName = file_list[i];
		printf("filename = %s\n",file_list[i]);
            if(SLFind(current_word->fileList,fn) != 0) {
                printf("word found file = %s\n",file_list[i]);
		table[word_index][i] = 1;
            }
        }
        pls
        word_index++;
    }
    pls
    for(i=0;i<number_of_files;i++) {
        int total = 0;
        for(j=0;j<number_of_words;j++) {
            total += table[j][i];
        }
        if(op) {
            pls
            /*if(total > 0) {*/
                printf("%s ",file_list[i]);
            /*}*/
        }else{
            if(total == number_of_words) {
                printf("%s ",file_list[i]);
            }
        }
    }
}

int loop(SortedListPtr sl) {
	char* input;
	char* command;
	int op;
	TokenizerT* tk;
	input = malloc(1000);
	memset(input,0,1000);

	while(1) {
        
		fileListNode * fn;
		SortedListIteratorPtr i;
        printf("Enter command: ");
        fflush(stdin);
		scanf(" %[^\n]s", input);
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
            return 1;
		}
        
		printf("Searching for %s, op = %d\n",input,op);
        
		search(tk,sl,op);
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
    int i;
    wordListNode * wln;
    fileListNode * fln;
    int list_index = 0;
    
    file_list = malloc(sizeof(char*)*10);
    
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "<list>") != NULL) { /* start of a new word. */
            char* word = substring(line, strlen("<list> "), strlen(line) - 3);
            
            printf("%s", word);
            
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
                    is_frequency = 0;
                    fln->count = atoi(pch);
                    SLInsert(wln->fileList, fln);
                } else {
                    is_frequency = 1;
                    
                    fln = malloc(sizeof(*fln));
                    fln->fileName = pch;
                    
                    printf("%s\n", fln->fileName);
                }
                
                pch = strtok (NULL, " \n");
            }
          
            
            SLInsert(sl, wln);
        }
    }
    
    for(i=0;i<number_of_files;i++) {
        printf("FILE: %s\n",file_list[i]);
    }
}

int main(int argc, char **argv) {
	SortedListPtr sl = SLCreate(compareWordNode);
    
    number_of_files = 0;
    
	if(argc!=2) {
		printf("ERROR: Invalid number of arguments. Usage should be \n\t$ ./search <query>\n");
		return 1;
	}
    
    parse_index_file(argv[1], sl);
    printf("\t\t\t%i\n", number_of_files);
    
	/*add reader stuff here */
	return loop(sl);	
}

