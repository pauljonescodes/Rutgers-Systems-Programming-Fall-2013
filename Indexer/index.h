#ifndef INDEX_H
#define INDEX_H

typedef struct _FileListNode {
	char* fileName;
	int count;
}fileListNode;

typedef struct _WordListNode {
	char* word;
	SortedListPtr fileList;
}wordListNode;

int compareWordNode(void* f1, void* f2);
int compareFileNode(void* f1, void* f2);


#endif
