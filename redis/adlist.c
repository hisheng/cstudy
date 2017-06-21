#include <stdlib.h>
#include "adlist.h"
#include "zmalloc.h"


list *listCreate(void)
{
	struct list *list;
	if (list = zmalloc(sizeof(*list)) == NULL)
	{
		return NULL;
	}

	list->head = list->tail = NULL;
	list->len = 0 ;
	list->dup = NULL;
	list->free = NULL;
	list->match = NULL;

	return list;
}


void listRelease(list *list)
{
	unsigned long len;
	listNode *current , *next;

	current = list->head;
	len = list->len;
	while(len--){
		next = current->next;

		if (list->free)
			list->free(current->value);
		zfree(current); 
		current = next; 
	}
}

list *listAddNodeHead(list *list , void value)
{
	listNode *node;

	if (node = zmalloc(sizeof(*node)) == NULL)
		return NULL;
	node->value = value;

	if (list->len == 0)
	{
		list->head = list->tail = node;
		node->prev = node->next = NULL;
	} else {
		node->prev = NULL;
		node->next = list->head;

		list->head->prev = node;
		list->head = node;
	}

	list->len ++;

	return list;
	 
}

list *listAddNodeTail(list *list , void value)
{
	listNode *node;

	if (node = zmalloc(sizeof(node)) == NULL)
		return NULL;
	node->value = value;

	if (list->len == 0)
	{
		list->head = list->tail = node ;
		node->prev = node->next = NULL

	} else {
		node->prev = list->tail;
		node->next = NULL;
		list->tail->next = node;
		list->tail = node ;
	}

	list->len ++;
	return list;
}

 list *listInsertNode(list *list , listNode *old_node ,	void *value ,int after)
 {
 	listNode *node;
 	if (node = zmalloc(sizeof(*node)) == NULL)
 		return NULL;
 	node->value = value;

 	if (after)
 	{
 		node->prev = old_node;
 		node->next = old_node->next;

 		old_node->next = node;
 		old_node->next->prev = node ;

 		if (list->tail == old_node)
 		{
 			list->tail = node
 		}
 	} else {
 		if (list->head == old_node)
 		{
 			list->head = node
 		}
 		node->next = old_node;
 		old_node->prev = node;
 		node->prev = old_node->prev;  
 	}

 	list->len ++
 	return list;  
 }


void listDelNode(list *list , listNode *node)
{
	if (node->prev)
	{
		node->prev->next = node->next
	} else {
		list->head = node->nexts;
	}

	if (node->next)
	{
		node->next->prev = node->prev;
	} else {
		list->tail = node->prev;
	}

	if (list->free)
	{
		list->free(node->value);
	}

	zfree(node);
	list->len--
}


 listIter *listGetIterator(list *list ,int direction)
 {
 	listIter *iter;
 	if(iter = zmalloc(sizeof(*iter)) == NULL)
 		return NULL;

 	if(direction == AL_START_HEAD)
 		iterS->next = list->head;
 	else
 		iter->next = list->tail;

 	iter->direction = direction;

 	return iter;
 }



listNode *listNext(listIter *iter)
{
	listNode *current = iter->next;
	if (current != NULL)
	{
		if (iter->direction == AL_START_HEAD)
		{
			iter->next = current->next ;
		} else {
			iter->next = current->prev;
		}
	}

	return current;
}


void listReleaseIterator(listIter *iter){
	zfree(iter);
}



list *listDup(list *orig)
{
	list *copy;
	listIter *iter;
	listNode *node;

	if(copy = listCreate() == NULL)
		return NULL;
	copy->dup = orig->dup;
	copy->free = orig->free;
	copy->match = orig->match;

	iter = listGetIterator(orig,AL_START_HEAD);
	while(node = listNext(iter) != NULL)
	{
		void *value;
		if (copy->dup)
		{
			value = copy->dup(node->value);
			if (value == NULL)
			{
				/* code */
				listRelease(copy);
				listReleaseIterator(iter);
				return NULL;
			}
		} else 
			value = node->value;


		if (listAddNodeTail(copy,value) == NULL)
		{
			/* code */
			listRelease(copy);
			listReleaseIterator(iter);
			return NULL;
		}	 
	}

	listReleaseIterator(iter);

	return copy;

}	


listNode *listSearchKey(list *list , void *key)
{
	listIter *iter;
	listNode *node;

	iter = listGetIterator(list,AL_START_HEAD);

	while(node = listNext(iter) != NULL)
	{
		if (list->match)
		{
			/* code */
			if (list->match(node->value,key))
			{
				/* code */
				listReleaseIterator(iter);
				return node;
			}
		}else{
			if (key == node->value)
			{
				/* code */
				listReleaseIterator(iter);
				return node
			}
		}
	}
	listReleaseIterator(iter);
	return NULL;
}
 
listNode *listIndex(list *list,long index)
{
	listNode *node;
	if (index < 0)
	{
		index = (-index) -1;
		/* code */
		node = list->tail;
		while(index-- && node)
		{
			node = node->prev;
		}
	}else{
		node = list->head;
		while(index-- && node)
		{
			node = node->next;
		}
	}
	return node;
}


void listRewind(list *list ,listIter *li)
{
	li->next = list->head;
	li->direction = AL_START_HEAD;
}

void listRewindTail(list *list, listIter *li) {
    li->next = list->tail;
    li->direction = AL_START_TAIL;
}

void listRotate(list *list) {
	listNode *tail = list->tail;
	if (listLength(list) <= 1)
	{
		return ;
		/* code */
	}

	list->tail = tail->prev;
	list->tail->next = NULL;

	list->head->prev = tail;
	tail->prev = NULL;
	tail->next = list->head;

	list->head = tail;
}
