//Şükrü Gümüştaş
//Written in CLion with Cygwin64
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node {
    char *word;
    struct node *next;
} NODE;

bool search(NODE *head, char *word);

char *getWord(FILE *file);

NODE *findCommonWords(NODE **a, NODE **b);

NODE *find2Angrams(NODE **a, NODE **b);

void concat(char *destination, char *first, char *second);

void insert(NODE **head, char *word);

void printList(NODE *head);

void sort(NODE **node);

void swap(char **a, char **b);

int main() {

    //I created file pointer.
    FILE *file;
    /*	I created four different struct variables, two of them are for getting
    *	input and two of them are for results.*/
    NODE *firstFile = NULL, *secondFile = NULL, *commonWords, *twoAngrams;
    //This word pointer is for taking input word by word.
    char *word, *file1 = "file1.txt", *file2 = "file2.txt";
    double a = 0, b = 0;
    //Getting inputs.
    if (!(file = fopen(file1, "r"))) {
        perror("Error: ");
        printf("%s", file1);
        return EXIT_FAILURE;
    }
    while ((word = getWord(file))) {
        insert(&firstFile, word);
        a++;
    }
    if (fclose(file) == EOF) {
        perror("Error: ");
        printf("%s", file1);
        return EXIT_FAILURE;
    }
    if (!(file = fopen(file2, "r"))) {
        perror("Error: ");
        printf("%s", file2);
        return EXIT_FAILURE;
    }
    while ((word = getWord(file))) {
        insert(&secondFile, word);
        b++;
    }
    if (fclose(file) == EOF) {
        perror("Error: ");
        printf("%s", file2);
        return EXIT_FAILURE;
    }

    //Finding common words and returning new common word list with function.
    commonWords = findCommonWords(&firstFile, &secondFile);
    printf("The common words are: \n\n");
    //Printing the list.
    printList(commonWords);
    //Finding 2-angrams.
    twoAngrams = find2Angrams(&firstFile, &secondFile);
    printf("The 2-angrams are: \n\n");
    //Printing it.
    printList(twoAngrams);
    free(file1);
    free(file2);
    free(firstFile);
    free(secondFile);
    free(commonWords);
    free(twoAngrams);
    return 0;
}

//This is a simple bubble sort algorithm for sorting linked list.
void sort(NODE **node) {
    NODE *head = *node, *second = NULL;
    while (head) {
        second = head->next;
        while (second) {
            if (strcasecmp(head->word, second->word) > 0) {
                swap(&(head->word), &(second->word));
            }
            second = second->next;
        }
        head = head->next;
    }
    free(head);
    free(second);
}

void swap(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}

/*	In this function, we are finding common words in two inner loops. We are checking
*	if a word in a node is equal to another word in other list's node. We are doing this
*	for all words in both lists. If there ara more than one matches, we search our result
*	list. If that match is already in the list, we don't insert. To checking equality
*	of two words, we use strcmp() function.*/
NODE *findCommonWords(NODE **a, NODE **b) {
    NODE *temp1 = *a, *temp2 = NULL, *result = NULL;
    int counter = 0;
    while (temp1) {
        temp2 = *b;
        while (temp2) {
            if (strcasecmp(temp1->word, temp2->word) != 0 || search(result, temp1->word)) {
                temp2 = temp2->next;
            } else {
                insert(&result, temp1->word);
                counter++;
                break;
            }
        }
        temp1 = temp1->next;
    }
    //Before returning result, we are sorting so in main function we don't have to sort again.
    sort(&result);
    printf("The number of common words is: %d\n\n", counter);
    free(temp1);
    free(temp2);
    return result;
}


/*	In this function, we are finding 2-angrams. To do so, we are checking if a word is in a node
*	equal to the word in other node of other list and we are doing same thing for the words in next
*	nodes at the same time. If following two words are same, we are merging those words with a space
*	between them. To merge, first we copy first word to a temp char with strcpy() then we concat temp
*	with space char with strcat(), then we concat temp with second word again. If this merged text is
*	already in 2-angrams list, we don't have to insert that so we search if it is in that. If it is not
*	we insert that text to our list. We are doing this until all two words are compared and inserted.*/
NODE *find2Angrams(NODE **a, NODE **b) {
    NODE *temp1 = *a, *temp2 = NULL, *result = NULL;
    int counter = 0;
    char destination[1000];
    while (temp1->next) {
        temp2 = *b;
        while (temp2->next) {
            if (!strcmp(temp1->word, temp2->word) && !strcmp(temp1->next->word, temp2->next->word)) {
                concat(destination, temp1->word, temp1->next->word);
                if (!search(result, destination)) {
                    insert(&result, destination);
                    counter++;
                } else {
                    break;
                }
            }
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }
    //Before returning result, we are sorting so in main function we don't have to sort again.
    sort(&result);
    printf("\nThe number of 2-angrams is: %d\n\n", counter);
    free(temp1);
    free(temp2);
    return result;
}

void concat(char *destination, char *first, char *second) {
    int i = 0, j = 0, length1 = (int) strlen(first), length2 = (int) strlen(second);
    while (i < length1) {
        destination[i] = first[i];
        i++;
    }
    destination[i++] = ' ';
    while (j < length2) {
        destination[i++] = second[j++];
    }
    destination[i] = '\0';
}


/*	Here, we are searching words in linked list. If a word is already in
*	our result lists (common words or 2angrams), then we don't have to
*	insert that word again so we eliminate that word thus there will be
*	no duplicate words.*/
bool search(NODE *head, char *word) {
    NODE *current = head;
    while (current) {
        if (strcasecmp(current->word, word) == 0) return true;
        current = current->next;
    }
    free(current);
    return false;
}

//We are printing the list to the screen.
void printList(NODE *head) {
    NODE *current = head;
    while (current) {
        printf("%s\n", current->word);
        current = current->next;
    }
    free(current);
}

//In this function, we are inserting words to the end of the linked list.
void insert(NODE **head, char *word) {
    NODE *newNode = (NODE *) malloc(sizeof(NODE)), *last = *head;
    newNode->word = strdup(word);
    newNode->next = NULL;
    if (*head != NULL) {
        while (last->next) {
            last = last->next;
        }
        last->next = newNode;
    } else {
        *head = newNode;
    }
}


/*	In this function, we are taking our inputs word by word. To do so,
*	we are taking every single letter as character. Then if a character
*	is not alphabetic, we ignore that character. At the end when we get a
*	word, we are returning the duplicate of that word with strdup() function
*	so we can continue from where we left.*/
char *getWord(FILE *file) {
    char word[100];
    int ch, i = 0;
    while (EOF != (ch = fgetc(file)) && !isalpha(ch));
    if (ch != EOF) {
        do {
            word[i++] = tolower(ch);
        } while (EOF != (ch = fgetc(file)) && isalpha(ch));
        word[i] = '\0';
        return strndup(word, strlen(word));
    } else {
        return NULL;
    }
}