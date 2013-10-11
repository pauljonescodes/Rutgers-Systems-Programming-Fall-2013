#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include "tokenizer.h"
#include "sorted-list.h"
#include "index.h"

void compareFileNode(void* f1, void* f2) {

}

void compareWordNode(void* f1, void* f2) {

}

void process_word(char * word) {
    printf("%s\n", word);
}

void process_file(const char *fname, int filesize)
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
        process_word(token);
    }
}

void get_files_in(const char * root_name)
{
    DIR * root;
    struct dirent * current_entry;
    const char * directory_name;
    char next_root[PATH_MAX];
    struct stat file_stat;
    
    if ((root = opendir(root_name))) {
        while ((current_entry = readdir(root))) {
            directory_name = current_entry->d_name;
            
            if (*directory_name != '.') {
                
                snprintf (next_root, PATH_MAX, "%s/%s", root_name, directory_name);
                
                
                stat(next_root,&file_stat);
                
                if (!S_ISDIR(file_stat.st_mode)) {
                    process_file(next_root, file_stat.st_size);
                }
                
                get_files_in(next_root);
            }
        }
    }
    
}

int main(int argc, char **argv) {
    
    get_files_in(argv[1]);
    
    return 0;
}



