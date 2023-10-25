//Napisati program koji prvo pročita koliko redaka ima datoteka, 
//tj.koliko ima studenata zapisanih u datoteci. Nakon toga potrebno 
//je dinamički alocirati prostor za niz strukturastudenata(ime, prezime, bodovi) 
//i učitati iz datoteke sve zapise.Na ekran ispisati ime, prezime, apsolutni i
//relativni broj bodova.Napomena: Svaki redak datoteke sadrži ime i prezime studenta, 
//te broj bodova na kolokviju.relatvan_br_bodova = br_bodova / max_br_bodova * 100

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 50
#define MAX_LINE 1024
#define MAX_POINTS 15
#define EXIT_SUCCESS 0
#define FILE_ERROR_OPEN -1

typedef struct _student {
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	double points;
} Student;

int readNoRowsInFile() {
	int counter = 0;
	FILE* filePointer = NULL;
	char buffer[MAX_LINE] = { 0 };

	filePointer = fopen("students.txt", "r");
	if (!filePointer) {
		printf("File is not open!\n");
		return FILE_ERROR_OPEN;
	}

	while (!feof(filePointer)) {
		fgets(buffer, MAX_LINE, filePointer);
		counter++;
	}

	fclose(filePointer);

	return counter;
}

int main() {
	
	int i, noRows = 0;
	noRows = readNoRowsInFile();

	if (noRows > 0) {

		FILE* filePointer = NULL;
		filePointer = fopen("students.txt", "r");

		Student* stud;
		stud = (Student*)malloc(noRows * sizeof(Student));

		if (stud == NULL) {
			printf("Error. File not opened.\n");
			return FILE_ERROR_OPEN;
		}

		for (i = 0; i < noRows; i++) {
			fscanf(filePointer, " %s %s %lf ", stud[i].name, stud[i].surname, &stud[i].points);
		}

		for (i = 0; i < noRows; i++) {
			printf("%s %s %.2lf %.2lf\%\n", stud[i].name, stud[i].surname, stud[i].points, stud[i].points / MAX_POINTS * 100);
		}

		fclose(filePointer);
		free(stud);
	}

	return EXIT_SUCCESS;
}