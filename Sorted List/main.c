/*
 * main.c
 */


#include 	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	"sorted-list.h"

int compareInts(void *p1, void *p2) {
	int i1 = *(int*)p1;
	int i2 = *(int*)p2;

	return i1 - i2;
}

int compareDoubles(void *p1, void *p2) {
	double d1 = *(double*)p1;
	double d2 = *(double*)p2;

	return (d1 < d2) ? -1 : ((d1 > d2) ? 1 : 0);
}

int compareStrings(void *p1, void *p2) {
	char *s1 = p1;
	char *s2 = p2;

	return strcmp(s1, s2);
}

int main() {
	int x, *p, *v;
	p = malloc(sizeof(int));
	SortedListPtr sl = SLCreate(compareInts);
	SortedListIteratorPtr si;
	while(scanf("%d",&x)==1) {
		v = malloc(sizeof(int));
		*v = x;
		SLInsert(sl,v);
	}
	
	si = SLCreateIterator(sl);
	while(1) {
		p = SLNextItem(si);
		if(p == 0) {
			break;
		}
		printf("%d\n",*p);
	}
	return 0;
}
