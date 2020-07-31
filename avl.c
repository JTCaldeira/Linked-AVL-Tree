/* =============================================================================
 *
 * avl.c
 *
 * =============================================================================
 */


#include "avl.h"


static int
pointer_cmp(const void *a, const void *b)
{
	return a - b;
}


avl_t *
linked_avl_create()
{
	avl_t *new = malloc(1 * sizeof(avl_t));

	new->root = NULL;
	new->head = NULL;
	new->tail = NULL;
	new->num_nodes = 0;

	return new;
}


static avl_node_t *
linked_avl_node_create(void *data)
{
	avl_node_t *new = malloc(1 * sizeof(avl_node_t));

	new->left = NULL;
	new->right = NULL;
	new->prev = NULL;
	new->next = NULL;
	new->data = data;
	new->height = 1;

	return new;
}

static int
avl_height(avl_node_t *node)
{
	return (node == NULL) ? 0 : node->height;
}


static int
avl_balance_factor(avl_node_t *node)
{
	return (node == NULL) ? 0 : avl_height(node->left) - avl_height(node->right);
}


static void
avl_update_height(avl_node_t *node)
{
	int left_height = avl_height(node->left);
	int right_height = avl_height(node->right);

	node->height = (left_height > right_height) ? left_height + 1 : right_height + 1;
}


static avl_node_t *
avl_rotate_left(avl_node_t *node)
{
	avl_node_t *right_node = node->right;
	node->right = right_node->left;
	right_node->left = node;

	avl_update_height(node);
	avl_update_height(right_node);

	return right_node;
}


static avl_node_t *
avl_rotate_right(avl_node_t *node)
{
	avl_node_t *left_node = node->left;
	node->left = left_node->right;
	left_node->right = node;
	
	avl_update_height(node);
	avl_update_height(left_node);

	return left_node;
}


static avl_node_t *
avl_rotate_left_right(avl_node_t *node)
{
	if (node == NULL)	return node;

	node->left = avl_rotate_left(node->left);

	return avl_rotate_right(node);
}


static avl_node_t *
avl_rotate_right_left(avl_node_t *node)
{
	if (node == NULL)	return node;

	node->right = avl_rotate_right(node->right);

	return avl_rotate_left(node);
}


static avl_node_t *
avl_balance(avl_node_t *node)
{
	int balance_factor;

	if (node == NULL)	return node;

	balance_factor = avl_balance_factor(node);

	if (balance_factor > 1) {
		if (avl_balance_factor(node->right) >= 0)	node = avl_rotate_right(node);
		else							node = avl_rotate_left_right(node);
	} else if (balance_factor < -1) {
		if (avl_balance_factor(node->left) <= 0)	node = avl_rotate_left(node);
		else							node = avl_rotate_right_left(node);
	} else {
		avl_update_height(node);
	}

	return node;
}


static void
linked_list_insert_after(avl_t *tree, avl_node_t *node, avl_node_t *new_node)
{
	new_node->prev = node;

	if (node->next == NULL) {
		new_node->next = NULL;
		tree->tail = new_node;
	} else {
		new_node->next = node->next;
		node->next->prev = new_node;
	}

	node->next = new_node;
}


static void
linked_list_insert_before(avl_t *tree, avl_node_t *node, avl_node_t *new_node)
{
	new_node->next = node;

	if (node->prev == NULL) {
		new_node->prev = NULL;
		tree->head = new_node;
	} else {
		new_node->prev = node->prev;
		node->prev->next = new_node;
	}

	node->prev = new_node;
}


static void
linked_list_prepend(avl_t *tree, avl_node_t *new_node)
{
	if (tree->head == NULL) {
		tree->head = new_node;
		tree->tail = new_node;
		new_node->prev = NULL;
		new_node->next = NULL;
	} else {
		linked_list_insert_before(tree, tree->head, new_node);
	}
}


static void
linked_list_append(avl_t *tree, avl_node_t *new_node)
{
	if (tree->tail == NULL)
		linked_list_prepend(tree, new_node);
	else
		linked_list_insert_after(tree, tree->tail, new_node);
}


void
linked_avl_append(avl_t *tree, void *data, cmp_function function)
{
	avl_node_t *tmp = tree->root;
	avl_node_t *prev = NULL;
	bool is_left_child;

	if (tmp == NULL) {
		tree->root = linked_avl_node_create(data);
		linked_list_prepend(tree, tree->root);
	} else {
		while (tmp != NULL) {
			if (data < tmp->data) {
				is_left_child = true;
				prev = tmp;
				tmp = tmp->left;
			} else {
				is_left_child = false;
				prev = tmp;
				tmp = tmp->right;
			}
		}

		tmp = linked_avl_node_create(data);
		
		if (is_left_child)	prev->left = tmp;
		else				prev->right = tmp;
		
		tmp = avl_balance(tmp);
		linked_list_append(tree, tmp);
	}
}


void
linked_avl_traverse(avl_t *tree, traverse_function function)
{
	avl_node_t *tmp = tree->root;

	if (function == NULL)
		return;

	while (tmp != NULL) {
		function(tmp->data);
		tmp = tmp->next;
	}
}


static void
linked_list_remove(avl_t *tree, avl_node_t *node)
{
	if (node->prev == NULL)
		tree->head = node->next;
	else
		node->prev->next = node->next;

	if (node->next == NULL)
		tree->tail = node->prev;
	else
		node->next->prev = node->prev;
}
