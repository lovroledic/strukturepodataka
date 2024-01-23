/*
"BookHaven" je knjižnica koja želi unaprijediti svoj sustav praæenja knjiga, autora i korisnika.
Svaka knjiga ima informacije o naslovu, autoru, godini izdanja i dostupnim primjercima.
Korisnici mogu posuðivati knjige i vratiti ih nakon nekog vremena.

Program mora korisniku omoguæiti:

a) ispis svih knjiga abecedno (tako da se mogu vidjeti svi podatci o knjizi i korisnici koji
   trenutno posuðuju knjigu)

b) ispis svih korisnika abecedno (tako da se mogu vidjeti sve knjige koje je neki korisnik posudio)

c) pretraživanje knjiga po godini izdanja (tako da se mogu vidjeti sve knjige iz te godine i
   njihova kolièina)

d) pretraživanje knjiga po nazivu autora (tako da se mogu vidjeti sve knjige tog autora i
   njihova kolièina)

e) unos novog korisnika

f) posudba knjige korisniku

	- može više primjeraka iste knjige posuditi od puta

	- korisnik u sebe nikad ne smije imati više od 5 knjiga sve skupa

	- ako pokušava napravit posudbu koja æe ga dovesti preko tog broja program treba upozoriti
	  korisnika na to i ispisati mu broj koliko još knjiga posudit

		- ako korisnik pita za više primjeraka knjige, a knjižnica na stanju nema nema dovoljno
		  treba korisniku omoguæiti izbor hoæe li posudit to što ima ili ne.

g) povratak knjiga na stanje

h) spremi (u datoteku/datoteke spremiti trenutno stanje u knjižnici, tako da kad se iduæi put
   pokrene program moguæe nastaviti od spremljenog stanja)

Sami odaberite strukturu podataka i format datoteka koju æe te koristiti za ovo izvesti.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -2

struct book;
typedef struct book* BookPosition;
typedef struct book {
	int id;
	char title[100];
	char author[100];
	int publicationYear;
	int totalCount;
	int availableCount;
	char (*borrowedBy)[20];
	BookPosition next;
} Book;

struct user;
typedef struct user* UserPosition;
typedef struct user {
	char username[20];
	int borrowedCount;
	BookPosition borrowedBooks[5];
	UserPosition next;
} User;

int loadBooks(BookPosition);
int loadUsers(UserPosition, BookPosition);
int saveUsers(UserPosition);
BookPosition findBookById(BookPosition, int);
BookPosition findBookByTitle(BookPosition, char*);
UserPosition findUserByUsername(UserPosition, char*);
int printBook(BookPosition);
int printBooks(BookPosition);
int printBooksByYear(BookPosition, int);
int printBooksByAuthor(BookPosition, char*);
int printUser(UserPosition);
int printUsers(UserPosition);
int deleteBooks(BookPosition);
int deleteUsers(UserPosition);
UserPosition createUser(char*);

int main()
{
	int i = 0;
	char input = 0;

	Book booksHead = {
		.id = 0,
		.title = { 0 },
		.author = { 0 },
		.publicationYear = 0,
		.totalCount = 0,
		.availableCount = 0,
		.borrowedBy = NULL
	};
	User usersHead = {
		.username = { 0 },
		.borrowedCount = 0,
		.borrowedBooks = NULL
	};

	loadBooks(&booksHead);
	loadUsers(&usersHead, &booksHead);

	printf("\033[1;32mWelcome to \033[0;36mBookHaven\033[1;32m!\n");
	printf("Here's a list of everything you can do:\033[0m\n\n");
	printf("[0] - end program\n[1] - print all books alphabetically\n[2] - print all users alphabetically\n[3] - print all books published in selected year\n[4] - print all books by author\n[5] - register new user\n[6] - borrow book to user\n[7] - return book\n[8] - manual save of library data\n");
	
	do {
		printf("\n> ");
		if (scanf(" %c", &input) != 1) {
			printf("\033[0;31mscanf error.\033[0m\n");
			deleteBooks(&booksHead);
			deleteUsers(&usersHead);
			return SCANF_ERROR;
		}

		if (input == '0') {
			printf("\033[1;32mThank you for using \033[0;36mBookHaven\033[1;32m. Until next time!\033[0m\n");
		}
		else if (input == '1') printBooks(booksHead.next);
		else if (input == '2') printUsers(usersHead.next);
		else if (input == '3')
		{
			int year = 0;

			printf("Enter year:\n> ");
			if (scanf(" %d", &year) != 1) {
				printf("\033[0;31mscanf error.\033[0m\n");
				deleteBooks(&booksHead);
				deleteUsers(&usersHead);
				return SCANF_ERROR;
			}

			printBooksByYear(booksHead.next, year);
		}
		else if (input == '4')
		{
			char author[100] = { 0 };

			printf("Enter author:\n> ");
			getchar();
			fgets(author, 100, stdin);
			if ((strlen(author) > 0) && (author[strlen(author) - 1] == '\n'))
				author[strlen(author) - 1] = '\0';

			printBooksByAuthor(booksHead.next, author);
		}
		else if (input == '5')
		{
			char username[20] = { 0 };
			int validUsername = 1;

			printf("Enter username (3-20 characters, can contain: a-z, A-Z, 0-9 or _):\n");

			do {
				validUsername = 1;

				printf("> ");
				if (scanf(" %s", username) != 1) {
					printf("\033[0;31mscanf error.\033[0m\n");
					deleteBooks(&booksHead);
					deleteUsers(&usersHead);
					return SCANF_ERROR;
				}

				if (strlen(username) < 3) {
					printf("Username too short. Must be between 3 and 20 characters.\n");
					validUsername = 0;
					continue;
				}
				else if (strlen(username) > 20) {
					printf("Username too long. Must be between 3 and 20 characters.\n");
					validUsername = 0;
					continue;
				}

				// Checks if username has invalid characters
				for (i = 0; i < 20 && username[i] != '\0'; i++) {
					char c = username[i];
					if (!(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == '_')) {
						printf("Username cannot contain character \'%c\'. Try again.\n", c);
						validUsername = 0;
						continue;
					}
				}

				// Checks if user with entered username already exists
				UserPosition currentUser = usersHead.next;
				while (currentUser != NULL && strcmp(currentUser->username, username)) currentUser = currentUser->next;
				if (currentUser != NULL) {
					printf("User with that username already exists.\n");
					validUsername = 0;
					continue;
				}

			} while (!validUsername);


			UserPosition newUser = createUser(username);
			if (newUser == NULL) {
				deleteBooks(&booksHead);
				deleteUsers(&usersHead);
				return MALLOC_ERROR;
			}

			// Position new user inside users list
			UserPosition currentUser = &usersHead;
			while (currentUser->next != NULL && strcmp(currentUser->next->username, username) < 0)
				currentUser = currentUser->next;
			newUser->next = currentUser->next;
			currentUser->next = newUser;

			printf("\033[1;32mUser '\033[1;34m%s\033[1;32m' successfully registered!\033[0m\n", newUser->username);

			if (saveUsers(usersHead.next))
			{
				printf("\033[0;31mSaving users failed. It is recommended you try manually saving library data.\033[0m\n");
				deleteBooks(&booksHead);
				deleteUsers(&usersHead);
				return FILE_OPEN_ERROR;
			}
			else printf("\033[0;33mLibrary data updated!\033[0m\n");
		}
		else if (input == '6')
		{
			UserPosition user = NULL;
			BookPosition book = NULL;
			char username[20] = { 0 }, title[100] = { 0 };
			int amount = 0, amountCounter = 0;
			char choice = 0;


			printf("Choose user:\n");

			do {
				printf("> ");
				if (scanf(" %s", username) != 1) {
					printf("\033[0;31mscanf error.\033[0m\n");
					deleteBooks(&booksHead);
					deleteUsers(&usersHead);
					return SCANF_ERROR;
				}

				user = findUserByUsername(usersHead.next, username);
				if (user == NULL) {
					printf("No user with such username. Try again.\n");
				}

			} while (user == NULL);


			printf("\033[1;32mUser found! Select book to borrow to user:\033[0m\n");

			do {
				printf("> ");
				getchar();
				fgets(title, 100, stdin);
				if ((strlen(title) > 0) && (title[strlen(title) - 1] == '\n'))
					title[strlen(title) - 1] = '\0';

				book = findBookByTitle(booksHead.next, title);
				if (book == NULL) {
					printf("No such book. Try again.\n");
				}

			} while (book == NULL);


			printf("\033[1;32mBook found! Available copies: \033[1;34m%d/%d\033[1;32m. Choose amount:\033[0m\n", book->availableCount, book->totalCount);

			do {
				printf("> ");
				if (scanf(" %d", &amount) != 1) {
					printf("\033[0;31mscanf error.\033[0m\n");
					deleteBooks(&booksHead);
					deleteUsers(&usersHead);
					return SCANF_ERROR;
				}

				if (amount <= 0) {
					printf("You must enter a positive amount. Try again.\n");
					continue;
				}

				if (amount > (5 - user->borrowedCount)) {
					printf("User cannot borrow more than 5 books at once (already has %d books borrowed). Try choosing %d books instead.\n", user->borrowedCount, 5 - user->borrowedCount);
					amount = 0;
					continue;
				}

				if (amount > book->availableCount)
				{
					printf("More books chosen than is available. Choose available amount (%d) instead? (y/n)\n", book->availableCount);
					
					printf("> ");
					if (scanf(" %c", &choice) != 1) {
						printf("\033[0;31mscanf error.\033[0m\n");
						return SCANF_ERROR;
					}

					if (choice == 'y') amount = book->availableCount;
					else {
						printf("Choose some other amount.\n");
						amount = 0;
						continue;
					}
				}
			} while (amount <= 0);


			user->borrowedCount += amount;
			book->availableCount -= amount;

			// Add user's username to book's borrowedBy list
			amountCounter = amount;
			for (amountCounter; amountCounter > 0; amountCounter--) {
				for (int i = 0; i < book->totalCount; i++) {
					if (!strcmp(book->borrowedBy[i], "\0")) {
						strcpy(book->borrowedBy[i], user->username);
						break;
					}
				}
			}
			// Add book to user's borrowed books list 
			amountCounter = amount;
			for (amountCounter; amountCounter > 0; amountCounter--) {
				for (int i = 0; i < 5; i++) {
					if (user->borrowedBooks[i] == NULL) {
						user->borrowedBooks[i] = book;
						break;
					}
				}
			}

			printf("\033[1;32mBook \033[1;34m%dx\033[1;32m '\033[1;34m%s\033[1;32m' successfully borrowed to user '\033[0;36m%s\033[1;32m'!\033[0m\n", amount, book->title, user->username);

			if (saveBooks(booksHead.next) || saveUsers(usersHead.next))
			{
				printf("\033[0;31mSaving library data failed.\033[0m\n");
				deleteBooks(&booksHead);
				deleteUsers(&usersHead);
				return FILE_OPEN_ERROR;
			}
			else printf("\033[0;33mLibrary data updated!\033[0m\n");
		}
		else if (input == '7')
		{
			UserPosition user = NULL;
			char username[20] = { 0 };
			int bookChoice = 0;


			printf("Choose user:\n");

			do {
				printf("> ");
				if (scanf(" %s", username) != 1) {
					printf("\033[0;31mscanf error.\033[0m\n");
					return SCANF_ERROR;
				}

				user = findUserByUsername(usersHead.next, username);
				if (user == NULL) {
					printf("No user with such username. Try again.\n");
				}

			} while (user == NULL);


			if (user->borrowedCount == 0) 
				printf("User has not borrowed any books.\n");

			else
			{
				printf("\033[1;32mUser found! Select book to return (choose number):\033[0m\n");

				for (int i = 0; i < 5; i++) {
					if (user->borrowedBooks[i] != NULL) {
						printf("\t[%d] - %s (%d), %s\n", i, user->borrowedBooks[i]->author, user->borrowedBooks[i]->publicationYear, user->borrowedBooks[i]->title);
					}
				}

				do {
					printf("> ");
					if (scanf(" %d", &bookChoice) != 1) {
						printf("\033[0;31mscanf error.\033[0m\n");
						return SCANF_ERROR;
					}

					if (bookChoice < 0 || bookChoice >= 5 || user->borrowedBooks[bookChoice] == NULL) {
						printf("Invalid choice. Try again.\n");
						continue;
					}

				} while (bookChoice < 0 || bookChoice >= 5 || user->borrowedBooks[bookChoice] == NULL);


				// Removes user's username from book's borrowedBy list
				for (int i = 0; i < user->borrowedBooks[bookChoice]->totalCount; i++) {
					if (!strcmp(user->borrowedBooks[bookChoice]->borrowedBy[i], user->username)) {
						strcpy(user->borrowedBooks[bookChoice]->borrowedBy[i], "\0");
						break;
					}
				}

				user->borrowedBooks[bookChoice]->availableCount++;
				user->borrowedCount--;
				user->borrowedBooks[bookChoice] = NULL;

				printf("\033[1;32mBook successfully returned!\033[0m\n");


				if (saveBooks(booksHead.next) || saveUsers(usersHead.next))
				{
					printf("\033[0;31mSaving library data failed.\033[0m\n");
					deleteBooks(&booksHead);
					deleteUsers(&usersHead);
					return FILE_OPEN_ERROR;
				}
				else {
					printf("\033[0;33mLibrary data updated!\033[0m\n");
				}
			}
		}
		else if (input == '8')
		{
			if (saveBooks(booksHead.next) || saveUsers(usersHead.next))
			{
				printf("\033[0;31mSaving library data failed.\033[0m\n");
				deleteBooks(&booksHead);
				deleteUsers(&usersHead);
				return FILE_OPEN_ERROR;
			}
			else {
				printf("\033[0;33mLibrary data updated!\033[0m\n");
			}
		}
		else printf("Invalid input. Try again.\n");

	} while (input != '0');

	deleteBooks(&booksHead);
	deleteUsers(&usersHead);

	return EXIT_SUCCESS;
}

int loadBooks(BookPosition books)
{
	int i = 0, size = 0;
	char bookBuffer[MAX_LINE] = { 0 };
	char borrowedByBuffer[MAX_LINE] = { 0 };
	char borrowerBuffer[20] = { 0 };
	char* borrowedByPointer = NULL;
	BookPosition book = NULL;
	BookPosition booksHead = books;
	FILE* filePointer = NULL;

	filePointer = fopen("books.txt", "r");
	if (filePointer == NULL) {
		printf("fopen error.\n");
		return FILE_OPEN_ERROR;
	}

	while (!feof(filePointer))
	{
		fgets(bookBuffer, MAX_LINE, filePointer);

		book = (BookPosition)malloc(sizeof(Book));
		if (book == NULL) {
			printf("malloc error.\n");
			return MALLOC_ERROR;
		}

		if (sscanf(bookBuffer, "%d; %[^;]; %[^;]; %d; %d; %[^\n] ", 
				&book->id, book->title, book->author, &book->publicationYear, &book->totalCount, borrowedByBuffer) != 6) {
			printf("sscanf error.\n");
			// free(books) ??
			return SCANF_ERROR;
		}
		book->availableCount = book->totalCount;

		book->borrowedBy = malloc(book->totalCount * sizeof *book->borrowedBy);
		if (book->borrowedBy == NULL) {
			printf("malloc error.\n");
			return MALLOC_ERROR;
		}
		for (int i = 0; i < book->totalCount; i++) strcpy(book->borrowedBy[i], "\0");

		borrowedByPointer = borrowedByBuffer;
		while (sscanf(borrowedByPointer, "%[^,], %n", borrowerBuffer, &size) == 1)
		{
			for (i = 0; i < book->totalCount && strcmp(book->borrowedBy[i], "\0"); i++);
			strcpy(book->borrowedBy[i], borrowerBuffer);

			book->availableCount--;
			borrowedByPointer += size;
		}

		while (books->next != NULL && strcmp(books->next->title, book->title) < 0) books = books->next;
		book->next = books->next;
		books->next = book;

		books = booksHead;
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}
int loadUsers(UserPosition users, BookPosition books)
{
	int i = 0, size = 0, bookIdBuffer = 0;
	char userBuffer[MAX_LINE];
	char borrowedBuffer[MAX_LINE];
	char* borrowedPointer = NULL;
	UserPosition user = NULL;
	UserPosition usersHead = users;
	FILE* filePointer = NULL;

	filePointer = fopen("users.txt", "r");
	if (filePointer == NULL) {
		printf("fopen error.\n");
		return FILE_OPEN_ERROR;
	}
	
	fseek(filePointer, 0, SEEK_END);
	if (ftell(filePointer) != 0) {
		fseek(filePointer, 0, 0);
		while (!feof(filePointer))
		{
			fgets(userBuffer, MAX_LINE, filePointer);

			user = (UserPosition)malloc(sizeof(User));
			if (user == NULL) {
				printf("malloc error.\n");
				return MALLOC_ERROR;
			}

			if (sscanf(userBuffer, "%[^;]; %d; %[^\n] ",
				user->username, &user->borrowedCount, borrowedBuffer) != 3) {
				printf("sscanf error.\n");
				// free(users) ??
				return SCANF_ERROR;
			}

			for (i = 0; i < 5; i++) user->borrowedBooks[i] = NULL;
			borrowedPointer = borrowedBuffer;
			while (sscanf(borrowedPointer, "%d, %n", &bookIdBuffer, &size) == 1)
			{
				for (i = 0; i < 5 && user->borrowedBooks[i] != NULL; i++);
				user->borrowedBooks[i] = findBookById(books, bookIdBuffer);

				borrowedPointer += size;
			}

			while (users->next != NULL && strcmp(users->next->username, user->username) < 0) users = users->next;
			user->next = users->next;
			users->next = user;

			users = usersHead;
		}
	}
	

	fclose(filePointer);

	return EXIT_SUCCESS;
}

int saveBooks(BookPosition books)
{
	int i = 0;
	int emptyArray = 1;
	FILE* filePointer = NULL;

	filePointer = fopen("books.txt", "w");
	if (filePointer == NULL) {
		printf("\033[0;31mfopen error.\033[0m\n");
		return FILE_OPEN_ERROR;
	}

	while (books != NULL) {
		emptyArray = 1;
		if (ftell(filePointer) != 0) fprintf(filePointer, "\n");
		fprintf(filePointer, "%d; %s; %s; %d; %d; ", books->id, books->title, books->author, books->publicationYear, books->totalCount);
		for (i = 0; i < books->totalCount; i++) {
			if (strcmp(books->borrowedBy[i], "\0")) {
				fprintf(filePointer, "%s, ", books->borrowedBy[i]);
				emptyArray = 0;
			}
		}
		if (emptyArray) {
			fprintf(filePointer, ",");
		}
		books = books->next;
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}
int saveUsers(UserPosition users)
{
	int i = 0;
	FILE* filePointer = NULL;

	filePointer = fopen("users.txt", "w");
	if (filePointer == NULL) {
		printf("\033[0;31mfopen error.\033[0m\n");
		return FILE_OPEN_ERROR;
	}

	while (users != NULL) {
		if (ftell(filePointer) != 0) fprintf(filePointer, "\n");
		fprintf(filePointer, "%s; %d; ", users->username, users->borrowedCount);

		if (users->borrowedCount > 0) {
			for (i = 0; i < 5; i++) {
				if (users->borrowedBooks[i] != NULL) {
					fprintf(filePointer, "%d, ", users->borrowedBooks[i]->id);
				}
			}
		}
		else {
			fprintf(filePointer, ",");
		}
		users = users->next;
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}

BookPosition findBookById(BookPosition current, int id)
{
	while (current != NULL && current->id != id) current = current->next;

	return current;
}

int printBook(BookPosition book)
{
	int i = 0, isFirst = 1;

	if (book->availableCount == 0) printf("\033[0;31m");

	printf("%s (%d), %s, available: %d/%d", book->author, book->publicationYear, book->title, book->availableCount, book->totalCount);
	
	if (book->availableCount < book->totalCount) {
		printf(", borrowed by:");
		for (i = 0; i < book->totalCount; i++) {
			if (strcmp(book->borrowedBy[i], "\0")) {
				if (isFirst) isFirst = 0;
				else printf(",");
				printf(" %s", book->borrowedBy[i]);
			}
		}
	}

	printf("\033[0m\n");

	return EXIT_SUCCESS;
}
int printBooks(BookPosition current)
{
	printf("\033[1;32mList of all books:\033[0m\n");

	while (current != NULL) {
		printBook(current);
		current = current->next;
	}

	return EXIT_SUCCESS;
}
int printBooksByYear(BookPosition current, int year)
{
	printf("\033[1;32mList of all books published in \033[1;34m%d\033[1;32m:\n\033[0m", year);

	while (current != NULL) {
		if (current->publicationYear == year) printBook(current);
		current = current->next;
	}

	return EXIT_SUCCESS;
}
int printBooksByAuthor(BookPosition current, char* author)
{
	printf("\033[1;32mList of all books written by \033[1;34m%s\033[1;32m:\n\033[0m", author);

	while (current != NULL) {
		if (strstr(current->author, author) != NULL) printBook(current);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

BookPosition findBookByTitle(BookPosition current, char* title)
{
	while (current != NULL && strcmp(current->title, title)) current = current->next;

	return current;
}
UserPosition findUserByUsername(UserPosition current, char* username)
{
	while (current != NULL && strcmp(current->username, username)) current = current->next;

	return current;
}

int printUser(UserPosition user)
{
	int i = 0;

	printf("%s, %d/5 book(s) available to borrow", user->username, 5 - user->borrowedCount);

	if (user->borrowedCount > 0) {
		printf(", currently has borrowed:\n");
		for (i = 0; i < 5; i++) {
			if (user->borrowedBooks[i] != NULL) {
				printf("\t%s (%d), %s\n", user->borrowedBooks[i]->author, user->borrowedBooks[i]->publicationYear, user->borrowedBooks[i]->title);
			}
		}
	}
	else printf("\n");
	

	return EXIT_SUCCESS;
}
int printUsers(UserPosition current)
{
	printf("\033[1;32mList of all users:\033[0m\n");

	while (current != NULL) {
		printUser(current);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

int deleteBooks(BookPosition head)
{
	while (head->next != NULL) {
		BookPosition temp = head->next;
		head->next = temp->next;
		free(temp->borrowedBy);
		free(temp);
	}

	return EXIT_SUCCESS;
}
int deleteUsers(UserPosition head)
{
	while (head->next != NULL) {
		UserPosition temp = head->next;
		head->next = temp->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}

UserPosition createUser(char* username)
{
	UserPosition newUser = NULL;
	newUser = malloc(sizeof(User));
	if (newUser == NULL) {
		printf("malloc error.\n");
		return NULL;
	}

	strcpy(newUser->username, username);
	newUser->borrowedCount = 0;
	for (int i = 0; i < 5; i++) newUser->borrowedBooks[i] = NULL;

	return newUser;
}