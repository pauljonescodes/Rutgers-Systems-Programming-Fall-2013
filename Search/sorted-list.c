/*
 * sorted-list.c
 */

#include 	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	"sorted-list.h"

#define DEV 0

SortedListPtr SLCreate(CompareFuncT cf) {
	SortedListPtr sl = malloc(sizeof(struct SortedList));
    
    if (DEV) {
        printf("SLCreate\n");
    }
    
	memset(sl,0,sizeof(struct SortedList));
	sl->cf = cf;
	return sl;
}

void destroyLinkedList(Node* head){
    if (DEV) {
        printf("destroyLinkedList\n");
    }
    
	if(head->next == 0) {
		free(head);
		return;
	}
	destroyLinkedList(head->next);
	free(head);
}

void SLDestroy(SortedListPtr sl) {
    if (DEV) {
        printf("SLDestroy\n");
    }
    
	destroyLinkedList(sl->head);
	free(sl);
}

int linkedListAdd(Node* head, CompareFuncT cf, Node* n) {
	Node* i, *p;
	
    if (DEV) {
        printf("linkedListAdd\n");
    }
    
	i = head->next;
	p = head;
	while(1) {
		if(i == 0) { /* add to the tail */
			p->next = n;
			return 1;
		}
		if((cf)(i->dataPtr,n->dataPtr) < 0) {
			p->next = n;
			n->next = i;
			return 1;
		}
		p=i;
		i=i->next;
	}
}

void SLPrint(SortedListPtr);

int SLInsert(SortedListPtr sl, void* o) {
	Node* n = malloc(sizeof(Node));
    
    if (DEV) {
        printf("SLInsert %p %p\n", sl, o);
    }
    
	n->next = 0;
	n->refCount = 1;
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

void* linkedListFind(Node* head, CompareFuncT cf, void* o) {
	Node* i, *p;
    
    if (DEV) {
        printf("linkedListFind\n");
    }
    
	i = (Node*)head->next;
	p = head;
	
	while(1) {
		if(i == 0) {
			return 0;
		}
		if((cf)(i->dataPtr,o) == 0) {
			return i->dataPtr;
		}
		p=i;
		i=i->next;
	}
}
void* SLFind(SortedListPtr sl, void* o) {
	Node* n;
    
    if (DEV) {
        printf("SLFind, sl->numItems: %i\n", sl->numItems);
    }
    if(sl->head == 0) {
	return 0;
    }
    
	if((sl->cf)((sl->head)->dataPtr,o) == 0) {
		return (sl->head)->dataPtr;
	}
    
	return linkedListFind(sl->head,sl->cf,o);
}

int linkedListRemove(Node* head, CompareFuncT cf, void* o) {
	Node* i, *p;
	i = (Node*)head->next;
	p = head;
	
    if (DEV) {
        printf("linkedListRemove\n");
    }
    
	while(1) {
		if(i == 0) {
			return 0;
		}
		if((cf)(i->dataPtr,o) == 0) {
			p->next = i->next;
			if(i->next!=0) {
				((Node*)(i->next))->refCount++;
			}
			i->refCount--;
			if(i->refCount==0) {
				if(i->next!=0) {
					((Node*)(i->next))->refCount--;
				}
				free(i);
			}
			return 1;
		}
		p=i;
		i=i->next;
	}
}

int SLRemove(SortedListPtr sl, void* o) {
	Node* n;
    
    if (DEV) {
        printf("SLRemove\n");
    }
    
	if((sl->cf)((sl->head)->dataPtr,o) == 0) {
		(sl->head)->refCount--;
		n = sl->head;
		sl->head = (sl->head)->next;
		if(n->refCount==0) {
			free(n);
		}
		return 1;
	}
	return linkedListRemove(sl->head,sl->cf,o);
}

void SLPrint(SortedListPtr sl) {
	Node* n = sl->head;
    
    if (DEV) {
        printf("SLPrint\n");
    }
    
	for(;n!=0;n=n->next) {
		printf("%d,",*((int*)n->dataPtr));
	}
	printf("\n");
}

SortedListIteratorPtr SLCreateIterator(SortedListPtr sl) {
	SortedListIteratorPtr ret;
    
	ret = malloc(sizeof(struct SortedListIterator));
    
    if (DEV) {
        printf("SLCreateIterator\n");
    }
    
    if (ret != NULL) {
        ret->item = sl->head;
        (sl->head)->refCount++;
        return ret;
    } else return NULL;
}

void SLDestroyIterator(SortedListIteratorPtr si) {
    
    if (DEV) {
        printf("SLDestroyIterator\n");
    }
    
	free(si);
}

void* SLNextItem(SortedListIteratorPtr si) {
	void* ret = 0;
	Node* n;
	int f0 = 0; /* flags */
    
    if (DEV) {
        printf("SLNextItem\n");
    }
    
	if(si->item == 0) {
		return ret;
	}
	n = si->item;
	ret = (si->item)->dataPtr;
	if((si->item)->refCount == 1) {
		f0 = 1;
	}
	si->item = (si->item)->next;
	n->refCount--;
	if(f0) {
		free(n);
	}
	if(si->item != 0) {
		(si->item)->refCount++;
	}
	return ret;
}
