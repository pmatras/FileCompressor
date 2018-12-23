#include "dynamicStructures.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ONCE 1

Huffman* findInList(Huffman* head, const char toFind) { 

	if (head == NULL) {

		return head;
	}

	if (head->next == NULL) {

		if (head->character == toFind) {

			return head;
		}
		
            return NULL;
		
	}

	while (head->character != toFind && head->next != NULL) {

		head = head->next;
	}

	if (head->character == toFind) {

        return head;
	}
		
        return NULL;
	
		
}

Huffman* addToList(const char* character, Huffman* head) {

	if (head == NULL) {

		Huffman* newNode = (Huffman*)malloc(sizeof(Huffman));

		newNode->character = *character;
		newNode->count = ONCE;
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->next = NULL;

		head = newNode;

		return head;

	}

	Huffman* found = findInList(head, *character);

	if (found == NULL) {

		Huffman* newNode = (Huffman*)malloc(sizeof(Huffman));

		newNode->character = *character;
		newNode->count = ONCE;
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->next = head;

		head = newNode;

		return head;

	}

	found->count++;


	return head;
}


int sortList(const Huffman* head) { 

	if (head == NULL || head->next == NULL) {

		printf("List is empty!\n");

		return 0;
	}

	Huffman* current1 = head;

	Huffman* current2 = head->next;

	int change = 1;

	while (change) {

		change = 0;

		current1 = head;
		current2 = head->next;
		
		while (current2 != NULL) {

			if (current1->count > current2->count) {

				int temp = current1->count;
				char tempC = current1->character;

				current1->character = current2->character;
				current1->count = current2->count;

				current2->character = tempC;
				current2->count = temp;

				change = 1;

			}


			current1 = current1->next;
			current2 = current2->next;

		}


	}

	return 1;

}

void printList(Huffman* head) { //to check if characters count is correct

	while (head) { 

		printf("%c: %d\n", head->character, head->count);

		head = head->next;
	}
}

Huffman* makeTreeFromList(Huffman* listRoot) { 

	if (listRoot == NULL) {

		return NULL;
	}

	Huffman* toConnect1;
	Huffman* toConnect2;  

	Huffman* current;

	while (listRoot->next != NULL) {

		toConnect1 = listRoot;
		toConnect2 = listRoot->next;

		listRoot = toConnect2->next;

		Huffman* connecting = (Huffman*)malloc(sizeof(Huffman));

		toConnect1->next = NULL;
		toConnect2->next = NULL;
		
		connecting->character = '@'; //adjuvant character
		connecting->count = toConnect1->count + toConnect2->count;
		connecting->left = toConnect1;      
		connecting->right = toConnect2;

		if (listRoot == NULL || (connecting->count <= listRoot->count))
		{
			connecting->next = listRoot;
			listRoot = connecting;

			continue;
			
		}
        
		current = listRoot;

		while (current->next != NULL && (connecting->count > current->next->count)) {

			current = current->next;
		}
		
		connecting->next = current->next;
		current->next = connecting;

	}

	return listRoot; 

} 

void convertIntToChars(int tempCode, LettersCodes* code) { 
	
	char temp[50] = "";

	int i = 0;

	while (tempCode > 0) { 

		if (tempCode % 10 == 2) { //int 2 = binary 1 

			temp[i] = '1';
		}
			
		else {

			temp[i] = '0'; //int 1 = binary 0
		}

		tempCode /= 10;



		++i;
	}


	temp[i] = '\0';

	int beginning = 0;

	for (int j = strlen(temp) - 1; j >= 0; --j) {

		code->code[beginning] = temp[j];

			beginning++;

	}

	
}

void makeCodesFromTree(const Huffman* root, const int lettersCount, int tempCode, LettersCodes* codes) { 

	if (root != NULL) {

		if ((root->left == NULL) || (root->right == NULL)) {		

			for (int i = 0; i < lettersCount; ++i) {

				if (codes[i].letter == '\0') {

					codes[i].letter = root->character;

					convertIntToChars(tempCode, &codes[i]);

					break;
				}
			}

		}

		if (root->left != NULL) {

			makeCodesFromTree(root->left, lettersCount, tempCode * 10 + 1, codes); //int 1 = binary 0
		}
		if (root->right != NULL) {

			makeCodesFromTree(root->right, lettersCount, tempCode * 10 + 2, codes); //int 2 = binary 1
		}

	}
	else {

		printf("\n\nError occured!\n\n");
	}


}

void printLettersCodes(const LettersCodes* codes, const int lettersCount) { //to check if every character has correct code assigned
	                                                                        
	for (int i = 0; i < lettersCount; ++i) {

		printf("%c: %s\n", codes[i].letter, codes[i].code);
	}
}

Huffman* readLetterFromTree(Huffman* current, const uint8_t bit) {

	if (bit == 1) {

		current = current->right;
	}
	else {

		current = current->left;
	}
	

	return current;
}

void deallocateTree(Huffman* root) { //DFS

	if (root == NULL) {

		return 0;
	}

	
	deallocateTree(root->right);

	deallocateTree(root->left);

	
	free(root);

	
}



