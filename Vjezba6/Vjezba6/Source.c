/*
Napisati program koji èita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini raèun. Na poèetku svake datoteke je zapisan datum u kojem vremenu je
raèun izdat u formatu YYYY-MM-DD. Svaki sljedeæi red u datoteci predstavlja artikl u formatu
naziv, kolièina, cijena. Potrebno je formirati vezanu listu raèuna sortiranu po datumu. Svaki èvor
vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
omoguæiti upit kojim æe korisnik saznati koliko je novaca sveukupno potrošeno na specifièni
artikl u odreðenom vremenskom razdoblju i u kojoj je kolièini isti kupljen.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2

struct article;
typedef struct article* ArticlePosition;
typedef struct article {
	char name[20];
	int quantity;
	int price;
	ArticlePosition next;
} Article;

struct receipt;
typedef struct receipt* ReceiptPosition;
typedef struct receipt {
	char date[10];
	Article headArticle;
	ReceiptPosition next;
} Receipt;

int addArticle(ArticlePosition head, ArticlePosition article);
int addReceipt(ReceiptPosition head, ReceiptPosition receipt);
int readReceipt(char* fileName, ReceiptPosition headReceipt);
int printArticles(ArticlePosition article);
int printReceipts(ReceiptPosition receipt);
int getIncomeOfArticleInPeriod(ReceiptPosition currentReceipt, char* article, char* startDate, char* endDate);
int deleteArticles(ArticlePosition articleHead);
int deleteReceipts(ReceiptPosition receiptHead);

int main()
{
	char article[100] = { 0 };
	char startDate[20] = { 0 }, endDate[20] = { 0 };
	char currReceipt[MAX_LINE] = { 0 };
	Receipt headReceipt = {
		.date = {0},
		.headArticle = NULL,
		.next = NULL
	};
	FILE* fptr = NULL;

	fptr = fopen("racuni.txt", "r");

	if (fptr == NULL) {
		printf("Error opening file.\n");
		return FILE_OPEN_ERROR;
	}

	while (!feof(fptr)) {
		fscanf(fptr, "%s ", currReceipt);
		printf("%s\n", currReceipt);
		readReceipt(currReceipt, &headReceipt);
	}

	printReceipts(headReceipt.next);

	printf("Enter article name: ");
	scanf("%s", article);
	printf("Enter start date (YYYY-MM-DD): ");
	scanf("%s", startDate);
	printf("Enter end date (YYYY-MM-DD): ");
	scanf("%s", endDate);

	printf("Income: %d", getIncomeOfArticleInPeriod(&headReceipt, article, startDate, endDate));


	deleteReceipts(&headReceipt);
	fclose(fptr);

	return 0;
}

int addArticle(ArticlePosition head, ArticlePosition article)
{
	ArticlePosition current = head;

	while (current->next != NULL && strcmp(current->next->name, article->name) == -1)
		current = current->next;

	article->next = current->next;
	current->next = article;

	return 0;
}

int addReceipt(ReceiptPosition head, ReceiptPosition receipt)
{
	ReceiptPosition current = head;

	while (current->next != NULL && strcmp(current->next->date, receipt->date) <= 0)
		current = current->next;

	receipt->next = current->next;
	current->next = receipt;

	return 0;
}

int readReceipt(char* fileName, ReceiptPosition headReceipt)
{
	char dateString[20] = { 0 };
	char articleString[MAX_LINE] = { 0 };
	int count = 0;
	FILE* fptr = NULL;
	ReceiptPosition newReceipt = NULL;
	ArticlePosition newArticle = NULL;

	fptr = fopen(fileName, "r");

	if (fptr == NULL) {
		printf("Error opening file.\n");
		return FILE_OPEN_ERROR;
	}

	newReceipt = (ReceiptPosition)malloc(sizeof(Receipt));
	if (newReceipt == NULL) {
		printf("Error allocating memory.\n");
		return MALLOC_ERROR;
	}

	fscanf(fptr, "%s ", dateString);
	strcpy(newReceipt->date, dateString);
	newReceipt->headArticle.next = NULL;

	while (!feof(fptr)) {
		newArticle = (ArticlePosition)malloc(sizeof(Article));
		if (newArticle == NULL) {
			printf("Error allocating memory.\n");
			return MALLOC_ERROR;
		}

		count = fscanf(fptr, "%s %d %d ", newArticle->name, &newArticle->quantity, &newArticle->price);
		if (count != 3) {
			printf("fscanf error.\n");
			return -1;
		}
		addArticle(&newReceipt->headArticle, newArticle);
	}

	addReceipt(headReceipt, newReceipt);


	fclose(fptr);

	return 0;
}

int printArticles(ArticlePosition current)
{
	while (current != NULL) {
		printf("\t%s, %d, %d\n", current->name, current->quantity, current->price);
		current = current->next;
	}

	return 0;
}

int printReceipts(ReceiptPosition current)
{
	while (current != NULL) {
		printf("Receipt from date %s:\n", current->date);
		printArticles(current->headArticle.next);
		current = current->next;
	}

	return 0;
}

int getIncomeOfArticleInPeriod(ReceiptPosition currentReceipt, char* article, char* startDate, char* endDate)
{
	int income = 0;

	// Move to first receipt with date equal to or greater than startDate
	while (currentReceipt != NULL && strcmp(currentReceipt->date, startDate) == -1)
		currentReceipt = currentReceipt->next;

	while (currentReceipt != NULL && strcmp(currentReceipt->date, endDate) <= 0) {
		ArticlePosition currentArticle = currentReceipt->headArticle.next;
		while (currentArticle != NULL) {
			if (strcmp(currentArticle->name, article) == 0) {
				income += currentArticle->quantity * currentArticle->price;
			}
			currentArticle = currentArticle->next;
		}
		currentReceipt = currentReceipt->next;
	}

	return income;
}

int deleteArticles(ArticlePosition articleHead)
{
	ArticlePosition temp;

	while (articleHead->next != NULL) {
		temp = articleHead->next;
		articleHead->next = temp->next;
		free(temp);
	}

	return 0;
}
int deleteReceipts(ReceiptPosition receiptHead)
{
	ReceiptPosition temp;

	while (receiptHead->next != NULL) {
		temp = receiptHead->next;
		receiptHead->next = temp->next;
		deleteArticles(&temp->headArticle);
		free(temp);
	}

	return 0;
}