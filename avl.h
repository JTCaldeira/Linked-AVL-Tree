#ifndef AVL_H
#define AVL_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/* type definition for a generic compare function */
typedef int (*cmp_function)(const void*, const void*);

/* type definition for a generic function to apply to an element of the linked AVL tree */
typedef void (*traverse_function)(void*);


/* type definition for a tree node */
typedef struct avl_node {
	struct avl_node *left;
	struct avl_node *right;
	struct avl_node *prev;
	struct avl_node *next;
	void *data;
	unsigned char height;
} avl_node_t;


/* type definition for the AVL tree */
typedef struct avl {
	avl_node_t *root;
	avl_node_t *head;
	avl_node_t *tail;
	size_t num_nodes;
} avl_t;


avl_t *
linked_avl_create();


void
linked_avl_append(avl_t*, void*, cmp_function);


void
linked_avl_traverse(avl_t*, traverse_function);



#endif /* AVL_H */
