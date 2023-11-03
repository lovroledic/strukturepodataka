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

#define MAX_LINE 1024
#define MAX_SIZE 50
#define EXIT_SUCCESS 0
#define FILE_ERROR_OPEN -1

struct _person;
typedef struct _person* Position;

typedef struct _person {
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	int birthYear;
	Position next;
} Person;

Position createPerson();
int addToFrontOfTheList(Position);
int addToEndOfTheList(Position);
int printList(Position);
Position findPerson(Position, char*);
int printPerson(Position);
int deletePerson(Position, char*);
int deleteList(Position);
int addAfter(Position, Position);
int addBefore(Position, Position);
int fileWrite(Position);
int _fileRead();
int fileRead(Position);

void main() {

	char in;
	FILE* filePointer = NULL;

	char surname[MAX_SIZE];
	Person head = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };
	Person headR = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };

	printf("f - add person to front of the list\ne - add person to end of the list\na - add person after person with surname\nb - add person before person with surname\np - print list\ns - find person with surname\nd - delete person with surname\nw - write list into file\nr - read list from file\n");

	do
	{
		printf("\nChoose action: ");
		scanf(" %c", &in);
		getchar();

		switch (in)
		{
			case 'f':
				printf("Add to front of the list\n");
				addToFrontOfTheList(&head);
				break;
			case 'e':
				printf("Add to end of the list\n");
				addToEndOfTheList(&head);
				break;
			case 'a':
				printf("Add after person with surname: ");
				scanf(" %s", surname);
				addAfter(&head, surname);
				break;
			case 'b':
				printf("Add before person with surname: ");
				scanf(" %s", surname);
				addBefore(&head, surname);
				break;
			case 'p':
				printf("Printing list:\n");
				printList(head.next);
				break;
			case 's':
				printf("Find person with surname: ");
				scanf(" %s", surname);
				findPerson(head.next, surname) ? printPerson(findPerson(head.next, surname)) : printf("\tPerson with surname '%s' was not found.\n", surname);
				break;
			case 'd':
				printf("Delete person with surname: ");
				scanf(" %s", surname);
				deletePerson(&head, surname);
				break;
			case 'w':
				printf("Writing list into file...\n");
				fileWrite(head.next);
				break;
			case 'r':
				printf("Reading list from file:\n");
				fileRead(&headR);
				printList(headR.next);
				break;
			case '0':
				// End program
				break;
			default:
				printf("\tInvalid input. Try again.\n");
				break;
		}
	} while (in != '0');

	deleteList(&head);
	deleteList(&headR);

	return EXIT_SUCCESS;
}

Position createPerson()
{
	Position person;
	person = (Position)malloc(sizeof(Person));

	printf("\tName: ");
	scanf(" %s", person->name);

	printf("\tSurname: ");
	scanf(" %s", person->surname);

	printf("\tBirth year: ");
	scanf(" %d", &person->birthYear);

	return person;
}

int addToFrontOfTheList(Position head)
{
	Position newPerson;
	newPerson = createPerson();

	if (newPerson) {
		newPerson->next = head->next;
		head->next = newPerson;
	}

	return EXIT_SUCCESS;
}

int addToEndOfTheList(Position head)
{
	Position newPerson;
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

int printList(Position p)
{
	if (!p)
		printf("\tEmpty list!\n");

	while (p != NULL)
	{
		printPerson(p);
		p = p->next;
	}
	
	return EXIT_SUCCESS;
}

Position findPerson(Position p, char* sur)
{
	while (p != NULL && strcmp(p->surname, sur))
		p = p->next;

	return p;
}

Position findPersonPrev(Position p, char* sur)
{
	while (p->next != NULL && strcmp(p->next->surname, sur))
		p = p->next;

	if (p->next == NULL) return NULL;

	return p;
}

int printPerson(Position p)
{
	printf("\t%s %s, roden(a) %d. godine\n", p->name, p->surname, p->birthYear);

	return EXIT_SUCCESS;
}

int deletePerson(Position p, char* sur)
{
	Position temp;
	temp = (Position)malloc(sizeof(Person));

	// sets p to predecessor of person we want to delete
	p = findPersonPrev(p, sur);

	if (p->next != NULL)
	{
		temp = p->next;
		p->next = temp->next;
		free(temp);

		printf("\tPerson deleted.\n");
	}
	else printf("\tPerson with surname '%s' was not found.\n", sur);

	return EXIT_SUCCESS;
}

// Delete all persons in list
int deleteList(Position head)
{
	Position temp;

	while (head->next != NULL)
	{
		temp = head->next;
		head->next = head->next->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}

int addAfter(Position p, char* sur)
{
	Position newPerson;

	p = findPerson(p, sur);

	if (p != NULL)
	{
		newPerson = createPerson();

		newPerson->next = p->next;
		p->next = newPerson;
	}
	else printf("\tPerson with surname '%s' was not found.\n", sur);

	return EXIT_SUCCESS;
}

int addBefore(Position p, char* sur)
{
	Position newPerson;

	p = findPersonPrev(p, sur);
	
	if (p != NULL)
	{
		newPerson = createPerson();

		newPerson->next = p->next;
		p->next = newPerson;
	}
	else printf("\tPerson with surname '%s' was not found.\n", sur);
	
	return EXIT_SUCCESS;
}

// Write list content into file
int fileWrite(Position p)
{
	FILE* filePointer = NULL;
	filePointer = fopen("persons.txt", "w");

	if (filePointer == NULL)
	{
		printf("\nFile could not be opened.\n");
		return FILE_ERROR_OPEN;
	}

	while (p != NULL)
	{
		fprintf(filePointer, "%s %s %d\n", p->name, p->surname, p->birthYear);
		p = p->next;
	}

	fclose(filePointer);

	printf("\tSuccess!\n");

	return EXIT_SUCCESS;
}

// Reads and prints file content
int _fileRead()
{
	char c;
	FILE* fp = NULL;
	fp = fopen("persons.txt", "r");

	if (fp == NULL)
	{
		printf("File could not be opened.\n");
		return FILE_ERROR_OPEN;
	}

	c = getc(fp);
	while (c != EOF)
	{
		printf("%c", c);
		c = getc(fp);
	}
		
	fclose(fp);

	return EXIT_SUCCESS;
}

// Reads and stores file content into list
int fileRead(Position head)
{
	FILE* filePointer = NULL;
	filePointer = fopen("persons.txt", "r");

	if (filePointer == NULL)
	{
		printf("File could not be opened.\n");
		return FILE_ERROR_OPEN;
	}


	Position prev, current;

	deleteList(head); // so repeated reading won't append file content to previous content
	prev = head;

	while (!feof(filePointer))
	{
		current = (Position)malloc(sizeof(Person)); // pointer to newly read person
		fscanf(filePointer, " %s %s %d ", current->name, current->surname, &current->birthYear);

		// add new person after previous
		current->next = prev->next;
		prev->next = current;

		prev = current; // newly read person will be the previous person in next iteration
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}