#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

struct treeNode;
typedef struct treeNode* TreePosition;
typedef struct treeNode {
	int data;
	TreePosition left;
	TreePosition right;
} TreeNode;

struct queueNode;
typedef struct queueNode* QueuePosition;
typedef struct queueNode {
	TreePosition treeNode;
	QueuePosition next;
} QueueNode;

QueuePosition rear = NULL;

TreePosition insert(int x, TreePosition root);
TreePosition search(int x, TreePosition root);
int printPreorder(TreePosition root, int level);
int printInorder(TreePosition root, int level);
int printPostorder(TreePosition root, int level);

QueuePosition enqueue(TreePosition levelItem);
TreePosition dequeue();
int printLevelorder(TreePosition root);

int main()
{
	TreePosition root = NULL;

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

	printf("Levelorder ispis:\n");
	printLevelorder(root);

	return 0;
}

TreePosition insert(int x, TreePosition root)
{
	// Prazno stablo => postavi kao root element
	if (root == NULL)
	{
		root = (TreePosition)malloc(sizeof(TreeNode));
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

TreePosition search(int x, TreePosition root)
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
int printPreorder(TreePosition root, int level)
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
int printInorder(TreePosition root, int level)
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
int printPostorder(TreePosition root, int level)
{
	int i = 0;

	if (root->left != NULL) printPostorder(root->left, level + 1);
	if (root->right != NULL) printPostorder(root->right, level + 1);

	for (i; i < level; i++) printf("   ");
	printf("%d\n", root->data);

	return 0;
}


QueuePosition enqueue(TreePosition treeNode)
{
	QueuePosition queueNode;
	queueNode = (QueuePosition)malloc(sizeof(QueueNode));
	if (queueNode == NULL) {
		printf("Malloc error.\n");
		return NULL;
	}
	queueNode->treeNode = treeNode;

	if (rear == NULL) {
		queueNode->next = queueNode;
		return queueNode;
	}

	queueNode->next = rear->next;
	rear->next = queueNode;
	rear = queueNode;

	return queueNode;
}

TreePosition dequeue()
{
	if (rear == NULL) {
		return NULL;
	}

	//QueuePosition front = rear->next;

	TreePosition levelItem;
	if (rear == rear->next) {
		levelItem = rear->treeNode;
		free(rear);
		rear = NULL;
	}
	else {
		QueuePosition front = rear->next;
		levelItem = front->treeNode;
		rear->next = front->next;
		free(front);
	}

	return levelItem;
}

int printLevelorder(TreePosition root)
{
	rear = enqueue(root);

	while (rear != NULL) {
		TreePosition treeNode = dequeue();
		printf("%d ", treeNode->data);
		if (treeNode->left != NULL) rear = enqueue(treeNode->left);
		if (treeNode->right != NULL) rear = enqueue(treeNode->right);
	}

	return 0;
}
