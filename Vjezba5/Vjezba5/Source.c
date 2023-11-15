#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

typedef struct stackItem {
	int value;
	struct stackItem* next;
} StackItem;

typedef struct {
	StackItem* top;
} Stack;

int push(Stack* stack, int x)
{
	StackItem* item = NULL;
	item = (StackItem*)malloc(sizeof(StackItem));
	if (item == NULL) {
		printf("malloc error.\n");
		return -1;
	}

	item->value = x;

	item->next = stack->top;
	stack->top = item;

	return 0;
}

int pop(Stack* stack, int* x)
{
	StackItem* top;
	top = stack->top;
	if (top == NULL) return -1;

	*x = top->value;

	stack->top = top->next;
	free(top);

	return 0;
}

int main()
{
	int size = 0;
	int count = 0;
	int firstOperand = 0, secondOperand = 0, result = 0;
	char currItem[10] = { 0 };
	char postfix[MAX_LINE] = { 0 };
	char* postfixPointer = postfix;
	FILE* filePointer = NULL;
	Stack stack = { .top = NULL };

	filePointer = fopen("postfix.txt", "r");
	fgets(postfix, MAX_LINE, filePointer);

	printf("Postfix: %s\n", postfix);

	while (strlen(postfixPointer) > 0)
	{
		count = sscanf(postfixPointer, " %s %n ", currItem, &size);
		if (count != 1) {
			printf("sscanf error.\n");
			return -1;
		}

		if (currItem[0] >= '0' && currItem[0] <= '9') {
			if (push(&stack, atoi(currItem))) {
				printf("malloc error.\n");
				return -1;
			}
		}
		else if (currItem[0] == '+' || currItem[0] == '-' || currItem[0] == '*')
		{
			// Item on top of stack is going to be the second operand (important for subtraction)
			if (pop(&stack, &secondOperand) || pop(&stack, &firstOperand)) {
				printf("Reading from empty stack.\n");
				return -1;
			}

			if (currItem[0] == '+') {
				if (push(&stack, firstOperand + secondOperand)) {
					printf("malloc error.\n");
					return -1;
				}
			}
			else if (currItem[0] == '-') {
				if (push(&stack, firstOperand - secondOperand)) {
					printf("malloc error.\n");
					return -1;
				}
			}
			else /* '*' */ {
				if (push(&stack, firstOperand * secondOperand)) {
					printf("malloc error.\n");
					return -1;
				}
			}
		}
		else
		{
			printf("Invalid character in postfix.\n");
			return -1;
		}

		postfixPointer += size;
	}

	if (pop(&stack, &result))
	{
		printf("Reading from empty stack.\n");
		return -1;
	}

	printf("Result: %d\n", result);

	return 0;
}