#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50

#define MALLOC_ERROR -1

struct directory;
typedef struct directory* DirectoryPosition;
typedef struct directory {
	char name[MAX_LENGTH];
	DirectoryPosition subdirectoryPosition;
	DirectoryPosition next;
} Directory;

struct levelStack;
typedef struct levelStack* LevelStackPosition;
typedef struct levelStack {
	DirectoryPosition directoryLevel;
	LevelStackPosition next;
} LevelStack;

DirectoryPosition createDirectory(char* directoryName);
int push(LevelStackPosition, DirectoryPosition);

int main()
{
	char input = 0;

	Directory headDirectory = { .name = {0}, .subdirectoryPosition = NULL, .next = NULL };

	DirectoryPosition rootDirectory = createDirectory("C:");
	headDirectory.next = rootDirectory;

	LevelStack headLevelStack = { .directoryLevel = NULL, .next = NULL };
	push(&headLevelStack, rootDirectory);


	do {

		scanf(" %c", &input);

		switch (input) {
		case '1':
			// md - make directory

			break;
		case '2':
			// cd <dir> - change directory

			break;
		case '3':
			// cd.. - go to parent directory

			break;
		case '4':
			// dir - view directory contents

			break;
		case '5':
			// end of program
			printf("\n*End of program.*\n");
			break;
		default:
			printf("Invalid input.\n");
			break;
		}

	} while (input != '5');


	return 0;
}

DirectoryPosition createDirectory(char* directoryName) {

	DirectoryPosition newDirectory = NULL;
	newDirectory = (DirectoryPosition)malloc(sizeof(Directory));

	if (newDirectory != NULL) {
		strcpy(newDirectory->name, directoryName);
		newDirectory->subdirectoryPosition = NULL;
		newDirectory->next = NULL;
	}

	return newDirectory;
}

int push(LevelStackPosition headLevelStack, DirectoryPosition currentDirectory) {

	LevelStackPosition newLevelStack = NULL;
	newLevelStack = (LevelStackPosition)malloc(sizeof(LevelStack));

	if (newLevelStack == NULL) return MALLOC_ERROR;

	newLevelStack->directoryLevel = currentDirectory;

	newLevelStack->next = headLevelStack->next;
	headLevelStack->next = newLevelStack;

	return 0;
}

int pop(LevelStackPosition headLevelStack) {

	LevelStackPosition temp;

	if (headLevelStack->next == NULL) return 1; // Stack is empty

	temp = headLevelStack->next;
	headLevelStack->next = headLevelStack->next->next;
	free(temp);

	return 0;
}