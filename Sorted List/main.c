/*
 * sorted-list.c
 */

#include <string.h>
#include "sorted-list.h"

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

SortedListPtr SLCreate(CompareFuncT cf) {
	SortedListPtr sl = malloc(sizeof(struct SortedList));
	memset(sl,0,sizeof(struct SortedList));
	sl->cf = cf;
	return sl;
}

void destroyLinkedList(Node* head){ 
	if(head->next == 0) {
		free(head);
		return;
	}
	destroyLinkedList(head->next);
	free(head);
}

void SLDestroy(SortedListPtr sl) {
	destroyLinkedList(sl->head);
	free(sl);
}

int linkedListAdd(Node* head, CompareFuncT cf, Node* n) {
	Node * i;
	Node * p;
	int result;

	i = head->next;
	p = head;
	result = 0;

	while(1) {
		if(i == 0) { /* add to the tail */
			p->next = n;
			return 1;
		}
		if((cf)(i->dataPtr,n->dataPtr) < 0) {
			p->next = n;
			n->next = i;
		}
		p=i;
		i=i->next;
	}

	return result;
}

int SLInsert(SortedListPtr sl, void* o) {
	Node* n = malloc(sizeof(Node));
	n->next = 0;
	n->dataPtr = o;
	sl->numItems++;
	if(sl->head == 0) { /* there is nothing in the list! */
		sl->head = n;
		return 1;
	}
	if((sl->cf)((sl->head)->dataPtr,o) < 0) { /* the new item deserves to be king! */
		n->next = sl->head;
		sl->head = n;
		return 1;
	}
	if(sl->numItems==2) { /* only one node in the list */
		(sl->head)->next = n;
		return 1;
	}
	return linkedListAdd(sl->head,sl->cf,n);
}

int linkedListRemove(Node* head, CompareFuncT cf, Node* n) {
	Node * i;
	Node * p;
	int result;

	i=head->next;
	p=head;
	result = 0;

	while(1) {
		if(i == 0) {
			return 0;
		}
		if((cf)(i->dataPtr,n->dataPtr) < 0) {
			p->next = i->next;
			free(i);
			return 1;
		}
		p=i;
		i=i->next;
	}

	return result;
}

int SLRemove(SortedListPtr sl, void* o) {
	if((sl->cf)((sl->head)->dataPtr,o) == 0) {
		sl->head = (sl->head)->next;
		return 1;
	}
	return linkedListRemove(sl->head,sl->cf,o);
}

SortedListIteratorPtr SLCreateIterator(SortedListPtr sl) {
	SortedListIteratorPtr ret;
	ret = malloc(sizeof(struct SortedListIterator));
	ret->item = sl->head;
	return ret;
}

void SLDestroyIterator(SortedListIteratorPtr si) {
	free(si);
}

void* SLNextItem(SortedListIteratorPtr si) {
	void* ret = 0;
	if(si->item == 0) {
		return ret;
	}
	ret = (si->item)->dataPtr;
	si->item = (si->item)->next;
	return ret;
}

int main() {
		
		return 0;
}
