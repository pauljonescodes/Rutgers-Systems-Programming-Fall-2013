
/*
 * To store input directory
 */

typedef struct content_word_node {
	char * word;
	int frequency;
	word_node * next;
} word_node;

typedef struct file_node {
	char * filename;
	file_node * next;
	content_word_node * contents;
} file_node;

typedef struct directory_node {
	directory_node * directories;
	file_node * files;
} directory_node;

/*
 * To store output reverse index
 */

typedef struct output_file_node {
	char * file_name;
	int frequency;
	output_file_node * next;
} output_file_node;

typedef struct output_word_node {
	char * word;
	output_file_node * file;
	output_word_node * next;
	
} output_word_node;

int main(int argc, char **argv) {
	directory_node * directory = get_root_directory_node(argv[1]);
	output_word_node * words = convert_directory_to_output(directory);
	char * output = get_output_string(words);
	write(output, argv[0]);
}

directory_node * get_root_directory_node (char * for_file_name) {
	
	return NULL;
}

output_word_node * convert_directory_to_output (directory_node * for_directory_node) {
	
	return NULL;
}

char * get_output_string (output_word_node * word) {
	
	return "";
}

char * get_output_file_string (output_file_node * file) {
	
	return "";
}

void write(char * string, char * to_file) {
	
}