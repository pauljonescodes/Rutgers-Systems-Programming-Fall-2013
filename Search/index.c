#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#ifdef __APPLE__
#include <limits.h>
#else
#include <linux/limits.h>
#endif

#include <sys/stat.h>
#include "tokenizer.h"
#include "sorted-list.h"
#include "index.h"
#include <libgen.h>

#define DEV 0

SortedListPtr sl;
int compareWordNode(void* f1, void* f2) {
    wordListNode * wl1 = f1;
    wordListNode * wl2 = f2;
    
    return strcmp(wl2->word, wl1->word);
}

int compareFileNode(void* f1, void* f2) {
    fileListNode * fl1 = f1;
    fileListNode * fl2 = f2;
    
    return strcmp(fl1->fileName, fl2->fileName);
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
    
    if (DEV) {
        printf("printLists()");
    }
    
	while(wn != 0) {
		printWordNode(*wn);
		wn = SLNextItem(si);
	}
}


void process_word(char * word, char* dname) {
    wordListNode * wln;
    fileListNode * fln;
    void * opaqueWLN;
    void * opaqueFLN;
    
    if (DEV) {
        printf("process_word(%s, %s)\n", word, dname);
    }
    
    wln = malloc(sizeof(* wln));
    fln = malloc(sizeof(* fln));
    wln->word = word;
    fln->fileName = dname;
    fln->count = 1;
    
    opaqueWLN = SLFind(sl, wln);
    
    if (opaqueWLN != NULL) { /* word already exists */
        wln = (wordListNode *) opaqueWLN;
        
        opaqueFLN = SLFind(wln->fileList, fln);
        
        if (opaqueFLN != NULL) { /* word already in this file */
            fln = (fileListNode *) opaqueFLN;
            fln->count++;
        } else { /*  */
            SLInsert(wln->fileList, fln);
        }
        
    } else { /* word is new SLInsert(sl,wln); */
        wln->fileList = SLCreate(compareFileNode);
        SLInsert(wln->fileList, fln);
        SLInsert(sl,wln);
    }
}

void process_file(const char *fname, int filesize, char* directory_name)
{
    char *file_contents;
    FILE *input_file = fopen(fname, "rb");
    TokenizerT * tokenizer;
    char * token;
    
    if (DEV) {
        printf("process_file(%s, %i, %s)\n", fname, filesize, directory_name);
    }
    
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
    
    if (DEV) {
        printf("get_files_in(%s)\n", root_name);
    }
    
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

void print_structure() {
    wordListNode * p;
    fileListNode * f;
    SortedListIteratorPtr si;
    SortedListIteratorPtr sw;
    
    si = SLCreateIterator(sl);
	
    while(1) {
		p = SLNextItem(si);
		if(p == NULL) {
			break;
		}
		
        printf("\"%s\" -> ",p->word);
        
        sw = SLCreateIterator(p->fileList);
        
        while (1) {
            f = SLNextItem(sw);
            
            if (f == NULL)
                break;
            
            printf("(\"%s\", %i),", f->fileName, f->count);
        }
        
        SLDestroyIterator(sw);
        
        printf("\n");
	}
    
    SLDestroyIterator(si);
}

void write_to_file(char * filename) {
    FILE *file = fopen(filename, "w");
    wordListNode * p;
    fileListNode * f;
    SortedListIteratorPtr si;
    SortedListIteratorPtr sw;
    
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    
    si = SLCreateIterator(sl);
	
    while(1) {
		p = SLNextItem(si);
		if(p == NULL) {
			break;
		}
		
        fprintf(file, "<list> %s\n", p->word);
        
        sw = SLCreateIterator(p->fileList);
        
        while (1) {
            f = SLNextItem(sw);
            
            if (f == NULL)
                break;
            
            fprintf(file, "%s %i ", f->fileName, f->count);
        }
        
        SLDestroyIterator(sw);
        
        fprintf(file, "\n</list>\n");
	}
    
    SLDestroyIterator(si);
    
    fclose(file);
}

/*
int main(int argc, char **argv) {
    sl = SLCreate(compareWordNode);
    get_files_in(argv[2]);
    write_to_file(argv[1]);
    return 0;
}*/



