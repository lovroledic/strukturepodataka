#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

struct node;
typedef struct node* Position;
typedef struct node {
	int data;
	Position left;
	Position right;
} Node;

Position insert(int x, Position root);
Position search(int x, Position root);
int printPreorder(Position root);
int printInorder(Position root);
int printPostorder(Position root);

int main()
{
	Position root = NULL;

	return 0;
}

Position insert(int x, Position root)
{
	// Prazno stablo => postavi kao root element
	if (root == NULL)
	{
		root = (Position)malloc(sizeof(Node));
		if (root == NULL) return NULL;

		root->data = x;
		root->left = NULL;
		root->right = NULL;
	}

	// Element je manji od root elementa => dodaj ga lijevom djetetu
	else if (x < root->data)
	{

	}

	// Element je veci od root elementa => dodaj ga desnom djetetu
	else if (x > root->data)
	{

	}

	return root;
}

Position search(int x, Position root)
{
	if (root == NULL) return NULL;

	// Prolazak po binarnom stablu pretrazivanja (lijevo su manji, desno veci elementi...)

	return root;
}

int printPreorder(Position root)
{
	// kao za ispis sadrzaja direktorija
	// 1. ispis cvora, 2. rekurzivni ispis lijevog djeteta, 3. rekurzivni ispis desnog djeteta

	return 0;
}

int printInorder(Position root)
{
	// kao za ispis stabla proracuna
	// 1. rekurzivni ispis lijevog djeteta, 2. ispis cvora, 3. rekurzivni ispis desnog djeteta

	return 0;
}

int printPostorder(Position root)
{
	// obrnuto od ispisa direktorija
	// 1. rekurzivni ispis lijevog djeteta, 2. rekurzivni ispis desnog djeteta, 3. ispis cvora

	return 0;
}