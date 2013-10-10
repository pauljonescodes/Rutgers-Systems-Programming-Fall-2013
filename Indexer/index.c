#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include "tokenizer.h"

void process_file(const char *fname, int filesize)
{
    char *file_contents;
    FILE *input_file = fopen(fname, "rb");
    TokenizerT * tokenizer;
    char * token;
    
    fseek(input_file, 0, SEEK_END);
    rewind(input_file);
    file_contents = malloc(filesize * (sizeof(char)));
    fread(file_contents, sizeof(char), filesize, input_file);
    fclose(input_file);
    
    tokenizer = TKCreate(" \0\a\b\n\t\n\v\f\r!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~", file_contents);
    
    printf("%s\n", fname);
    
    while ((token = TKGetNextToken(tokenizer))) {
        printf("%s\n", token);
    }
    
    printf("\n\nakjsnd\n\n");
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
            
            if (strcmp (directory_name, "..") != 0 &&
                strcmp (directory_name, ".") != 0) {
                
                snprintf (next_root, PATH_MAX, "%s/%s", root_name, directory_name);
                
                stat(next_root,&file_stat);
                
                if (!S_ISDIR(file_stat.st_mode)) {
                    process_file(directory_name, file_stat.st_size);
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



