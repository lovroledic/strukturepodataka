#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE 1024
#define MAX_SIZE 100

#define EXIT_SUCCESS 0
#define FILE_ERROR_OPEN -1
#define MALLOC_ERROR -2

typedef struct monomial {
	int coef;
	int exp;
	struct monomial* next;
} Mono;

int strToPoly(Mono*, char*);
int polyAdd(Mono*, Mono*);
int printMono(Mono*);
int printPoly(Mono*);

int main()
{
	Mono poly1 = {.coef = 0, .exp = 0, .next = NULL};
	Mono poly2 = { .coef = 0, .exp = 0, .next = NULL };

	int i = 0, size = 0;
	char buffer[MAX_LINE] = { 0 };

	FILE* filePointer = NULL;
	filePointer = fopen("polynomials.txt", "r");

	if (filePointer == NULL)
	{
		printf("File could not be opened.\n");
		return FILE_ERROR_OPEN;
	}

	// Reads 1st line and coverts in into poly1
	fgets(buffer, MAX_LINE, filePointer);
	strToPoly(&poly1, buffer);

	// Reads 2nd line and coverts in into poly2
	fgets(buffer, MAX_LINE, filePointer);
	strToPoly(&poly2, buffer);

	printPoly(poly1.next);
	printPoly(poly2.next);


	fclose(filePointer);

	deletePoly(&poly1);
	deletePoly(&poly2);
	
	return EXIT_SUCCESS;
}

// Sorted input to polynomial
int polyAdd(Mono* head, Mono* mono)
{
	Mono* prev = head;
	Mono* temp = NULL;

	if (mono->coef == 0)
	{
		free(mono);
		return EXIT_SUCCESS;
	}

	// Find predecessor of first monomial with exponent less than or equal to exponent of new monomial
	while (prev->next != NULL && prev->next->exp > mono->exp)
		prev = prev->next;

	// If the next monomial has the same exponent as new monomial, add their coefficients
	if (prev->next != NULL && mono->exp == prev->next->exp)
	{
		prev->next->coef += mono->coef;

		if (prev->next->coef == 0)
		{
			temp = prev->next;
			prev->next = prev->next->next;
			free(temp);
		}
	}
		
	// Else, put new monomial after the predecessor
	else
	{
		mono->next = prev->next;
		prev->next = mono;
	}

	return EXIT_SUCCESS;
}

// Convert string of a polynomial into a polynomial
int strToPoly(Mono* head, char* str)
{
	int i = 0, size = 0;
	Mono* mono = NULL;

	while (strlen(str) > 0)
	{
		mono = (Mono*)malloc(sizeof(Mono));

		if (mono == NULL) return MALLOC_ERROR;

		// %n reads how many characters were read until it was reached
		sscanf(str, " %dx^%d %n", &mono->coef, &mono->exp, &size);

		polyAdd(head, mono);

		str += size;
	}

	return EXIT_SUCCESS;
}

int printMono(Mono* mono)
{
	mono->coef > 0 ? printf("%d", mono->coef) : printf("- %d", -mono->coef);

	if (mono->exp == 1) printf("x ");
	else if (mono->exp != 0) printf("x^%d ", mono->exp);

	// If exp == 0, then there won't be an x

	return EXIT_SUCCESS;
}

int printPoly(Mono* current)
{
	// First monomial shouldn't have '+' in front of it
	if (current != NULL)
	{
		printMono(current);
		current = current->next;
	}

	while (current != NULL)
	{
		if (current->coef > 0) printf("+ ");
		printMono(current);
		current = current->next;
	}

	printf("\n");

	return EXIT_SUCCESS;
}

int deletePoly(Mono* head)
{
	Mono* temp = NULL;

	while (head->next != NULL)
	{
		temp = head->next;
		head->next = head->next->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}