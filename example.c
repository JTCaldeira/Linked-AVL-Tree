#include <stdio.h>
#include <stdlib.h>

#include "avl.h"


#define NUM_PEOPLE	5


typedef struct {
	int id;
	char *name;
	char *activity;
} person_t;


int compare_people(const void *item1, const void *item2)
{
	return ((person_t*)item1)->id - ((person_t*)item2)->id;
}


void print_person(void *item)
{
	person_t *person = (person_t*)item;

	printf("ID:\t\t%d\nName:\t\t%s\nActivity:\t%s\n", person->id, person->name, person->activity);
}


int main(void)
{
	person_t **people = malloc(NUM_PEOPLE * sizeof(person_t*));
	char *names[NUM_PEOPLE] = { "Matthew", "Tobias", "James", "Joe", "Ronnie" };
	char *activities[NUM_PEOPLE] = { "Seize the day", "Perform a Ritual", "Battery", "Search for the whales", "Dive holily" };
	int i;

	// create a linked AVL tree
	avl_t *tree = linked_avl_create();
	
	// create people_t objects
	for (i = 0; i < NUM_PEOPLE; ++i) {
		people[i] = malloc(sizeof(person_t));
		people[i]->id = i + 1;
		people[i]->name = names[i];
		people[i]->activity = activities[i];
	}

	// insert the objects into the linked AVL tree
	for (i = 0; i < NUM_PEOPLE; ++i) {
		linked_avl_append(tree, people[i], compare_people);
	}

	// print the created objects
	for (i = 0; i < NUM_PEOPLE; ++i) {
		printf("===========\n");
		print_person((void*)people[i]);
	}
	printf("\n-------------------------------------\n\n");

	// test the linked AVL tree traverse function
	linked_avl_traverse(tree, print_person);

	// clean up object memory
	for (i = 0; i < NUM_PEOPLE; ++i) {
		free(people[i]);
	}
	free(people);

	return EXIT_SUCCESS;
}
