/*
Napisati program koji èita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih država. Uz
ime države u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
države. Svaka datoteka koja predstavlja državu sadrži popis gradova u formatu naziv_grada,
broj_stanovnika.

a) Potrebno je formirati sortiranu vezanu listu država po nazivu. Svaki èvor vezane liste
sadrži stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.

b) Potrebno je formirati stablo država sortirano po nazivu. Svaki èvor stabla sadrži vezanu
listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.

Nakon formiranja podataka potrebno je ispisati države i gradove te omoguæiti korisniku putem
tastature pretragu gradova odreðene države koji imaju broj stanovnika veæi od unosa na
tastaturi.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1;
#define SCANF_ERROR -2
#define MALLOC_ERROR -3

struct cityTree;
typedef struct cityTree* CityTreePosition;
typedef struct cityTree {
	int population;
	char name[20];
	CityTreePosition left;
	CityTreePosition right;
} CityTree;

struct countryList;
typedef struct countryList* CountryListPosition;
typedef struct countryList {
	char name[20];
	CountryListPosition next;
} CountryList;

struct cityList;
typedef struct cityList* CityListPosition;
typedef struct cityList {
	int population;
	char name[20];
	CityTreePosition next;
} CityTree;

struct countryTree;
typedef struct countryTree* CountryTreePosition;
typedef struct countryTree {
	char name[20];
	CountryTreePosition left;
	CountryTreePosition right;
} CountryTree;

int addCountryToList(CountryListPosition current, CountryListPosition newCountry);
CountryTreePosition addCountryToTree(CountryTreePosition current, CountryTreePosition newCountry);

int main()
{
	CountryList countryListHead = { .name = { 0 }, .next = NULL };
	CountryTreePosition countryTree = NULL;
	char buffer[1024] = { 0 };
	char country[20] = { 0 }, countryFilePath[24] = { 0 }, city[20] = { 0 };
	int population = 0;
	FILE* countryFilePointer = NULL;
	FILE* filePointer = NULL;

	filePointer = fopen("drzave.txt", "r");
	if (filePointer == NULL) {
		printf("Could not open file 'drzave.txt'\n");
		return FILE_OPEN_ERROR;
	}

	while (!feof(filePointer)) {
		fgets(buffer, 1024, filePointer);
		if (sscanf(buffer, "%[^,], %s ", country, countryFilePath) != 2) {
			printf("sscanf error.\n");
			return SCANF_ERROR;
		}

		CountryListPosition newCountryList = NULL;
		newCountryList = (CountryListPosition)malloc(sizeof(CountryList));
		if (newCountryList == NULL) {
			printf("malloc error.\n");
			return MALLOC_ERROR;
		}
		strcpy(newCountryList->name, country);

		CountryTreePosition newCountryTree = NULL;
		newCountryTree = (CountryTreePosition)malloc(sizeof(CountryTree));
		if (newCountryTree == NULL) {
			printf("malloc error.\n");
			return MALLOC_ERROR;
		}
		strcpy(newCountryTree->name, country);

		addCountryToList(countryListHead.next, newCountryList);
		countryTree = addCountryToTree(countryTree, newCountryTree);

		countryFilePointer = fopen(countryFilePath, "r");
		while (!feof(countryFilePointer)) {
			fgets(buffer, 1024, filePointer);
			if (sscanf(buffer, "%[^,], %d ", city, population) != 2) {
				printf("sscanf error.\n");
				return SCANF_ERROR;
			}
		}

		fclose(countryFilePointer);
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}

int addCountryToList(CountryListPosition current, CountryListPosition newCountry)
{
	while (current->next != NULL && strcmp(newCountry->name, current->next->name) > 0) current = current->next;

	newCountry->next = current->next;
	current->next = newCountry;

	return EXIT_SUCCESS;
}

CountryTreePosition addCountryToTree(CountryTreePosition current, CountryTreePosition newCountry)
{
	if (current == NULL) {
		current = newCountry;
		current->left = NULL;
		current->right = NULL;
	}

	if (strcmp(newCountry->name, current->name) < 0) current->left = addCountryToTree(current->left, newCountry);
	else if (strcmp(newCountry->name, current->name) > 0) current->right = addCountryToTree(current->right, newCountry);

	return current;
}