#include<stdio.h>
#define MAX_SIZE 50
#define EXIT_SUCCESS 0

struct _person;
typedef struct _person* Position;
typedef struct _person {
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	int birthYear;
	Position next;
} person;

void main() {
	person head = { .next = NULL,.name = {0},
					.surname = {0},.birthYear = 0 };


	
}

int menu(Position head) {
	char choice = '\0';
	Position person = NULL;
	while (1) {

	}
}

int addToFrontOfTheList(Position head) {
	Position newPerson = NULL;

	newPerson = createPerson();

	if (newPerson) {
		newPerson->next = head->next;
		head->next = newPerson;
	}

	return EXIT_SUCCESS;
}

int addToEndOfTheList(Position head) {
	Position newPerson = NULL;
	Position last = NULL;

	newPerson = createPerson();

	if (newPerson) {
		last = findLast(head);
		newPerson->next = last->next;
		last->next = newPerson;
	}

	return EXIT_SUCCESS;
}

Position findLast(Position head) {
	Position current = head;

	while (current->next != NULL) {
		current = current->next;
	}

	return current;
}

int printList(Position firstElement) {
	Position current = firstElement;
	if (!firstElement)
		printf("Empty list!\n");
	for (; current != NULL; current = current->next) {
		// printf("
		printPerson(current);
	}
}