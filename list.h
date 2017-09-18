/******************************************************************************
 * Copyright (C) Dean Zou 2016-2017
 * file:    list.h
 * author:  Dean Zou <deboot@msn.cn>
 * created: 2017-08-02 16:29:41
 * updated: 2017-08-02 16:29:41
 *****************************************************************************/
#ifndef _LIST_H
#define _LIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct list_node {
	void *data;
	struct list_node *next;
} list_node;

/* linked list */
list_node* list_create(void *data);
void list_destroy(list_node **list);
list_node* list_insert_after(list_node *node, void *data);
list_node* list_insert_beginning(list_node *list, void *data);
list_node* list_insert_end(list_node *list, void *data);
void list_remove(list_node **list, list_node *node);
void list_remove_by_data(list_node **list, void *data);
list_node* list_find_node(list_node *list, list_node *node);
list_node* list_find_by_data(list_node *list, const void *data);
list_node* list_find(list_node *list, int(*func)(list_node*,const void*), const void *data);
void list_foreach(list_node *list, int(*func)(list_node*,void*), void *data);

#ifdef __cplusplus
}
#endif
#endif
