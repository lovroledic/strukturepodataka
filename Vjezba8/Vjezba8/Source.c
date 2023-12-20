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
int printPreorder(Position root, int level);
int printInorder(Position root, int level);
int printPostorder(Position root, int level);

int main()
{
	Position root = NULL;

	root = insert(4, root);
	root = insert(9, root);
	root = insert(2, root);
	root = insert(10, root);
	root = insert(6, root);
	root = insert(3, root);
	root = insert(1, root);
	root = insert(5, root);
	root = insert(7, root);

	if (search(6, root))
		printf("6 je u stablu.\n");
	else
		printf("6 nije u stablu.\n");

	if (search(10, root))
		printf("10 je u stablu.\n");
	else
		printf("10 nije u stablu.\n");

	printf("Preorder ispis:\n");
	printPreorder(root, 0);

	printf("Inorder ispis:\n");
	printInorder(root, 0);

	printf("Postorder ispis:\n");
	printPostorder(root, 0);

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
		root->left = insert(x, root->left);

	// Element je veci od root elementa => dodaj ga desnom djetetu
	else if (x > root->data)
		root->right = insert(x, root->right);

	return root;
}

Position search(int x, Position root)
{
	if (root == NULL) return NULL;

	if (x < root->data)
		return search(x, root->left);

	else if (x > root->data)
		return search(x, root->right);

	return root;
}


// kao za ispis sadrzaja direktorija
// 1. ispis cvora, 2. rekurzivni ispis lijevog djeteta, 3. rekurzivni ispis desnog djeteta
int printPreorder(Position root, int level)
{
	int i = 0;

	for (i; i < level; i++) printf("   ");
	printf("%d\n", root->data);

	if (root->left != NULL) printPreorder(root->left, level + 1);
	if (root->right != NULL) printPreorder(root->right, level + 1);

	return 0;
}


// kao za ispis stabla proracuna
// 1. rekurzivni ispis lijevog djeteta, 2. ispis cvora, 3. rekurzivni ispis desnog djeteta
int printInorder(Position root, int level)
{
	int i = 0;

	if (root->left != NULL) printInorder(root->left, level + 1);

	for (i; i < level; i++) printf("   ");
	printf("%d\n", root->data);

	if (root->right != NULL) printInorder(root->right, level + 1);

	return 0;
}


// obrnuto od ispisa direktorija
// 1. rekurzivni ispis lijevog djeteta, 2. rekurzivni ispis desnog djeteta, 3. ispis cvora
int printPostorder(Position root, int level)
{
	int i = 0;

	if (root->left != NULL) printPostorder(root->left, level + 1);
	if (root->right != NULL) printPostorder(root->right, level + 1);

	for (i; i < level; i++) printf("   ");
	printf("%d\n", root->data);

	return 0;
}
