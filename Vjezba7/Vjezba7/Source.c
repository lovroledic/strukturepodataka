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
int pop(LevelStackPosition);
int addDirectory(DirectoryPosition, DirectoryPosition);
int changeDirectory(LevelStackPosition, char*);
int printDirectory(DirectoryPosition, int);

int main()
{
	char input = 0;
	char directoryName[MAX_LENGTH] = { 0 };

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
			printf("md ");
			scanf(" %s ", directoryName);
			addDirectory(headLevelStack.next->directoryLevel, createDirectory(directoryName));
			break;
		case '2':
			// cd <dir> - change directory
			printf("cd ");
			scanf(" %s ", directoryName);
			changeDirectory(&headLevelStack, directoryName);
			break;
		case '3':
			// cd.. - go to parent directory
			printf("cd..\n");
			pop(&headLevelStack);
			break;
		case '4':
			// dir - view directory contents
			printf("Printing contents of %s:\n", headLevelStack.next->directoryLevel->name);
			printDirectory(headLevelStack.next->directoryLevel, 0);
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

int addDirectory(DirectoryPosition superdirectory, DirectoryPosition subdirectory) {

	subdirectory->next = superdirectory->subdirectoryPosition;
	superdirectory->subdirectoryPosition = subdirectory;

	return 0;
}

int changeDirectory(LevelStackPosition headLevelStack, char* directoryName) {

	DirectoryPosition current = headLevelStack->next->directoryLevel->subdirectoryPosition;

	while (current != NULL && strcmp(current->name, directoryName) != 0)
		current = current->next;

	if (current != NULL)
		push(headLevelStack, current);
	else
		printf("Directory not found.\n");

	return 0;
}

int printDirectory(DirectoryPosition current, int depth) {

	int i = 0;

	if (current->subdirectoryPosition != NULL) {
		DirectoryPosition currentSub = current->subdirectoryPosition;
		while (currentSub != NULL) {
			for (i = 0; i < depth; i++)
				printf(" ");
			printf("%s\n", currentSub->name);
			printDirectory(currentSub, depth + 1);
			currentSub = currentSub->next;
		}
	}

	return 0;
}