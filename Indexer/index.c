#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>

void get_files_in(const char * root_name)
{
    DIR * root;
    struct dirent * current_entry;
    const char * directory_name;
    char next_root[PATH_MAX];
    
    if ((root = opendir(root_name))) {
        while ((current_entry = readdir(root))) {
            directory_name = current_entry->d_name;
            
            if (strcmp (directory_name, "..") != 0 &&
                strcmp (directory_name, ".") != 0) {
                printf ("%s/%s\n", root_name, directory_name);
                
                snprintf (next_root, PATH_MAX, "%s/%s", root_name, directory_name);
                
                get_files_in(next_root);
            }
        }
    }
    
}

int main(int argc, char **argv) {
    
    get_files_in(argv[1]);
    
    return 0;
}



