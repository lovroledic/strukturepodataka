//2. Definirati strukturu osoba(ime, prezime, godina roðenja) i napisati program koji :
//A.dinamièki dodaje novi element na poèetak liste,
//B.ispisuje listu,
//C.dinamièki dodaje novi element na kraj liste,
//D.pronalazi element u listi(po prezimenu),
//E.briše odreðeni element iz liste,
//U zadatku se ne smiju koristiti globalne varijable.

//3. Prethodnom zadatku dodati funkcije :
//A.dinamièki dodaje novi element iza odreðenog elementa,
//B.dinamièki dodaje novi element ispred odreðenog elementa,
//C.sortira listu po prezimenima osoba, ( OVO NE )
//D.upisuje listu u datoteku,
//E.èita listu iz datoteke.

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 50
#define EXIT_SUCCESS 0

struct _person;
typedef struct _person* Position;

typedef struct _person {
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	int birthYear;
	Position next;
} Person;

Position createPerson();
int addToFrontOfTheList(Position head);
int addToEndOfTheList(Position head);
int printList(Position current);
Position findPerson(Position current, char* surname);
int printPerson(Position person);
int deletePerson(Position head, char* surname);
int addAfter(Position current, Position person);
int addBefore(Position current, Position person);

void main() {

	char surname[MAX_SIZE];
	Person head = { .next = NULL,.name = {0}, .surname = {0}, .birthYear = 0 };

	addToFrontOfTheList(&head);
	addToFrontOfTheList(&head);
	addToEndOfTheList(&head);

	printList(head.next);

	printf("\nUnesi prezime koje zelis pronaci: ");
	scanf(" %s", surname);
	findPerson(head.next, surname) ? printPerson(findPerson(head.next, surname)) : printf("Osoba ne postoji\n");

	deletePerson(&head, surname);
	printList(head.next);

	printf("\nUnesi prezime ispred kojeg zelis umetnuti osobu: ");
	scanf(" %s", surname);
	addBefore(&head, surname);
	printList(head.next);
	
	printf("\nUnesi prezime iza kojeg zelis umetnuti osobu: ");
	scanf(" %s", surname);
	addAfter(&head, surname);
	printList(head.next);

	// person ? printPerson() : print(ne postoji)

	// 3.c ne treba! nece bit na kolokviju (jedino moze bit sortirani unos)

	return EXIT_SUCCESS;
}

Position createPerson() {
	Position person;
	person = (Position)malloc(sizeof(Person));

	printf("Unesi ime: ");
	scanf(" %s", person->name);

	printf("Unesi prezime: ");
	scanf(" %s", person->surname);

	printf("Unesi godinu rodenja: ");
	scanf(" %d", &person->birthYear);

	return person;
}

int addToFrontOfTheList(Position head) {
	Position newPerson;

	printf("\nUnos na pocetak:\n");
	newPerson = createPerson();

	if (newPerson) {
		newPerson->next = head->next;
		head->next = newPerson;
	}

	return EXIT_SUCCESS;
}

int addToEndOfTheList(Position head) {
	Position newPerson;

	printf("\nUnos na kraj:\n");
	newPerson = createPerson();

	if (newPerson) {

		while (head->next != NULL) {
			head = head->next;
		}

		newPerson->next = head->next;
		head->next = newPerson;
	}

	return EXIT_SUCCESS;
}

int printList(Position current) {
	
	printf("\nIspis:\n");
	if (!current)
		printf("Empty list!\n");
	for (; current != NULL; current = current->next)
		printPerson(current);
	
	return EXIT_SUCCESS;
}

Position findPerson(Position current, char* sur)
{
	while (current != NULL && strcmp(current->surname, sur))
		current = current->next;

	return current;
}

int printPerson(Position person)
{
	printf("%s %s, roden(a) %d. godine\n", person->name, person->surname, person->birthYear);

	return EXIT_SUCCESS;
}


int deletePerson(Position p, char* sur)
{
	Position temp;
	temp = (Position)malloc(sizeof(Person));

	// postavlja p na prethodnika elementa kojeg želimo izbrisati
	while (p->next != NULL && strcmp(p->next->surname, sur))
		p = p->next;

	if (p->next != NULL)
	{
		temp = p->next;
		p->next = temp->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}

int addAfter(Position current, char* sur)
{
	Position newPerson;

	printf("\nUnos iza %s:\n", sur);
	newPerson = createPerson();

	current = findPerson(current, sur);

	newPerson->next = current->next;
	current->next = newPerson;

	return EXIT_SUCCESS;
}


int addBefore(Position current, char* sur)
{
	Position newPerson;

	printf("\nUnos prije %s:\n", sur);
	newPerson = createPerson();

	while (current->next != NULL && strcmp(current->next->surname, sur))
		current = current->next;

	newPerson->next = current->next;
	current->next = newPerson;

	return EXIT_SUCCESS;
}